// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_readback.h for the primary calling header

#include "Vtb_readback__pch.h"

VlCoroutine Vtb_readback___024root___eval_initial__TOP__Vtiming__0__5(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval_initial__TOP__Vtiming__0__5\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_readback__DOT__tbad;
    tb_readback__DOT__tbad = 0;
    IData/*31:0*/ tb_readback__DOT__nmemclk;
    tb_readback__DOT__nmemclk = 0;
    IData/*31:0*/ tb_readback__DOT__kk;
    tb_readback__DOT__kk = 0;
    IData/*31:0*/ tb_readback__DOT__npipe;
    tb_readback__DOT__npipe = 0;
    IData/*31:0*/ tb_readback__DOT__nras;
    tb_readback__DOT__nras = 0;
    IData/*31:0*/ tb_readback__DOT__ncas;
    tb_readback__DOT__ncas = 0;
    IData/*31:0*/ tb_readback__DOT__nwe;
    tb_readback__DOT__nwe = 0;
    IData/*31:0*/ tb_readback__DOT__nmx;
    tb_readback__DOT__nmx = 0;
    CData/*0:0*/ tb_readback__DOT__prasa;
    tb_readback__DOT__prasa = 0;
    CData/*0:0*/ tb_readback__DOT__pcasa;
    tb_readback__DOT__pcasa = 0;
    CData/*0:0*/ tb_readback__DOT__pwea;
    tb_readback__DOT__pwea = 0;
    CData/*0:0*/ tb_readback__DOT__pmx;
    tb_readback__DOT__pmx = 0;
    CData/*0:0*/ tb_readback__DOT__prp;
    tb_readback__DOT__prp = 0;
    CData/*0:0*/ tb_readback__DOT__pmr;
    tb_readback__DOT__pmr = 0;
    IData/*31:0*/ tb_readback__DOT__nrp;
    tb_readback__DOT__nrp = 0;
    IData/*31:0*/ tb_readback__DOT__nmr;
    tb_readback__DOT__nmr = 0;
    IData/*31:0*/ tb_readback__DOT__nms;
    tb_readback__DOT__nms = 0;
    IData/*31:0*/ tb_readback__DOT__nsq;
    tb_readback__DOT__nsq = 0;
    IData/*31:0*/ tb_readback__DOT__nsrc;
    tb_readback__DOT__nsrc = 0;
    IData/*31:0*/ tb_readback__DOT__nwr;
    tb_readback__DOT__nwr = 0;
    IData/*31:0*/ tb_readback__DOT__nnr;
    tb_readback__DOT__nnr = 0;
    IData/*31:0*/ tb_readback__DOT__nmrf;
    tb_readback__DOT__nmrf = 0;
    IData/*31:0*/ tb_readback__DOT__nsm;
    tb_readback__DOT__nsm = 0;
    IData/*31:0*/ tb_readback__DOT__nmw;
    tb_readback__DOT__nmw = 0;
    IData/*31:0*/ tb_readback__DOT__npsm;
    tb_readback__DOT__npsm = 0;
    IData/*31:0*/ tb_readback__DOT__nwmw;
    tb_readback__DOT__nwmw = 0;
    IData/*31:0*/ tb_readback__DOT__ng13;
    tb_readback__DOT__ng13 = 0;
    IData/*31:0*/ tb_readback__DOT__nxsm;
    tb_readback__DOT__nxsm = 0;
    IData/*31:0*/ tb_readback__DOT__nwpr;
    tb_readback__DOT__nwpr = 0;
    IData/*31:0*/ tb_readback__DOT__nrh;
    tb_readback__DOT__nrh = 0;
    IData/*31:0*/ tb_readback__DOT__nldp;
    tb_readback__DOT__nldp = 0;
    IData/*31:0*/ tb_readback__DOT__npha;
    tb_readback__DOT__npha = 0;
    IData/*31:0*/ tb_readback__DOT__ncra;
    tb_readback__DOT__ncra = 0;
    IData/*31:0*/ tb_readback__DOT__nha;
    tb_readback__DOT__nha = 0;
    IData/*31:0*/ tb_readback__DOT__nhb;
    tb_readback__DOT__nhb = 0;
    IData/*31:0*/ tb_readback__DOT__nwcr;
    tb_readback__DOT__nwcr = 0;
    IData/*31:0*/ tb_readback__DOT__nwar;
    tb_readback__DOT__nwar = 0;
    IData/*31:0*/ tb_readback__DOT__nfl;
    tb_readback__DOT__nfl = 0;
    IData/*31:0*/ tb_readback__DOT__nmp;
    tb_readback__DOT__nmp = 0;
    CData/*0:0*/ tb_readback__DOT__psq;
    tb_readback__DOT__psq = 0;
    CData/*0:0*/ tb_readback__DOT__psrc;
    tb_readback__DOT__psrc = 0;
    CData/*0:0*/ tb_readback__DOT__pwr;
    tb_readback__DOT__pwr = 0;
    CData/*0:0*/ tb_readback__DOT__pnr;
    tb_readback__DOT__pnr = 0;
    CData/*0:0*/ tb_readback__DOT__pmrf;
    tb_readback__DOT__pmrf = 0;
    CData/*0:0*/ tb_readback__DOT__psm;
    tb_readback__DOT__psm = 0;
    CData/*0:0*/ tb_readback__DOT__pmw;
    tb_readback__DOT__pmw = 0;
    CData/*0:0*/ tb_readback__DOT__ppsm;
    tb_readback__DOT__ppsm = 0;
    CData/*0:0*/ tb_readback__DOT__pwmw;
    tb_readback__DOT__pwmw = 0;
    CData/*0:0*/ tb_readback__DOT__pg13;
    tb_readback__DOT__pg13 = 0;
    CData/*0:0*/ tb_readback__DOT__pxsm;
    tb_readback__DOT__pxsm = 0;
    CData/*0:0*/ tb_readback__DOT__pwpr;
    tb_readback__DOT__pwpr = 0;
    CData/*0:0*/ tb_readback__DOT__prh;
    tb_readback__DOT__prh = 0;
    CData/*0:0*/ tb_readback__DOT__pldp;
    tb_readback__DOT__pldp = 0;
    CData/*0:0*/ tb_readback__DOT__ppha;
    tb_readback__DOT__ppha = 0;
    CData/*0:0*/ tb_readback__DOT__pcra;
    tb_readback__DOT__pcra = 0;
    CData/*0:0*/ tb_readback__DOT__pha;
    tb_readback__DOT__pha = 0;
    CData/*0:0*/ tb_readback__DOT__phb;
    tb_readback__DOT__phb = 0;
    CData/*0:0*/ tb_readback__DOT__pwcr;
    tb_readback__DOT__pwcr = 0;
    CData/*0:0*/ tb_readback__DOT__pwar;
    tb_readback__DOT__pwar = 0;
    CData/*0:0*/ tb_readback__DOT__pfl;
    tb_readback__DOT__pfl = 0;
    CData/*0:0*/ tb_readback__DOT__pmp;
    tb_readback__DOT__pmp = 0;
    IData/*31:0*/ tb_readback__DOT__runlen;
    tb_readback__DOT__runlen = 0;
    IData/*31:0*/ tb_readback__DOT__maxrun;
    tb_readback__DOT__maxrun = 0;
    IData/*31:0*/ tb_readback__DOT__nwin;
    tb_readback__DOT__nwin = 0;
    IData/*31:0*/ tb_readback__DOT__ndd;
    tb_readback__DOT__ndd = 0;
    IData/*31:0*/ tb_readback__DOT__nidle_lo;
    tb_readback__DOT__nidle_lo = 0;
    IData/*31:0*/ tb_readback__DOT__winat;
    tb_readback__DOT__winat = 0;
    CData/*0:0*/ tb_readback__DOT__pmfree;
    tb_readback__DOT__pmfree = 0;
    CData/*0:0*/ tb_readback__DOT__pmidle;
    tb_readback__DOT__pmidle = 0;
    IData/*31:0*/ tb_readback__DOT__nmfree_e;
    tb_readback__DOT__nmfree_e = 0;
    IData/*31:0*/ tb_readback__DOT__nmidle_e;
    tb_readback__DOT__nmidle_e = 0;
    IData/*31:0*/ tb_readback__DOT__nmfree_hi;
    tb_readback__DOT__nmfree_hi = 0;
    IData/*31:0*/ tb_readback__DOT__nrfsh;
    tb_readback__DOT__nrfsh = 0;
    IData/*31:0*/ tb_readback__DOT__npsh;
    tb_readback__DOT__npsh = 0;
    IData/*31:0*/ tb_readback__DOT__ncoin;
    tb_readback__DOT__ncoin = 0;
    IData/*31:0*/ tb_readback__DOT__nmwlo;
    tb_readback__DOT__nmwlo = 0;
    IData/*31:0*/ tb_readback__DOT__lastpsh;
    tb_readback__DOT__lastpsh = 0;
    IData/*31:0*/ tb_readback__DOT__lastcoin;
    tb_readback__DOT__lastcoin = 0;
    IData/*31:0*/ tb_readback__DOT__lastmwlo;
    tb_readback__DOT__lastmwlo = 0;
    IData/*31:0*/ tb_readback__DOT__lastmf00;
    tb_readback__DOT__lastmf00 = 0;
    CData/*0:0*/ tb_readback__DOT__pmf00;
    tb_readback__DOT__pmf00 = 0;
    CData/*0:0*/ tb_readback__DOT__pms3;
    tb_readback__DOT__pms3 = 0;
    IData/*31:0*/ tb_readback__DOT__nmf00on;
    tb_readback__DOT__nmf00on = 0;
    IData/*31:0*/ tb_readback__DOT__nms3on;
    tb_readback__DOT__nms3on = 0;
    CData/*0:0*/ tb_readback__DOT__pdd;
    tb_readback__DOT__pdd = 0;
    IData/*31:0*/ tb_readback__DOT__nmemst;
    tb_readback__DOT__nmemst = 0;
    IData/*31:0*/ tb_readback__DOT__nfree;
    tb_readback__DOT__nfree = 0;
    IData/*31:0*/ tb_readback__DOT__nmemfr;
    tb_readback__DOT__nmemfr = 0;
    IData/*31:0*/ tb_readback__DOT__nheld_nz;
    tb_readback__DOT__nheld_nz = 0;
    IData/*31:0*/ tb_readback__DOT__nmapst;
    tb_readback__DOT__nmapst = 0;
    IData/*31:0*/ tb_readback__DOT__nmapfn;
    tb_readback__DOT__nmapfn = 0;
    IData/*31:0*/ tb_readback__DOT__npsm2;
    tb_readback__DOT__npsm2 = 0;
    IData/*31:0*/ tb_readback__DOT__nsm2;
    tb_readback__DOT__nsm2 = 0;
    IData/*31:0*/ tb_readback__DOT__nload;
    tb_readback__DOT__nload = 0;
    IData/*31:0*/ tb_readback__DOT__ncnt;
    tb_readback__DOT__ncnt = 0;
    IData/*31:0*/ tb_readback__DOT__nd0;
    tb_readback__DOT__nd0 = 0;
    IData/*31:0*/ tb_readback__DOT__nwim;
    tb_readback__DOT__nwim = 0;
    IData/*31:0*/ tb_readback__DOT__nx10;
    tb_readback__DOT__nx10 = 0;
    IData/*31:0*/ tb_readback__DOT__nmti;
    tb_readback__DOT__nmti = 0;
    IData/*31:0*/ tb_readback__DOT__nwm;
    tb_readback__DOT__nwm = 0;
    IData/*31:0*/ tb_readback__DOT__nall3;
    tb_readback__DOT__nall3 = 0;
    IData/*31:0*/ tb_readback__DOT__nrw;
    tb_readback__DOT__nrw = 0;
    IData/*31:0*/ tb_readback__DOT__nwp;
    tb_readback__DOT__nwp = 0;
    IData/*31:0*/ tb_readback__DOT__ndty;
    tb_readback__DOT__ndty = 0;
    IData/*31:0*/ tb_readback__DOT__nevn;
    tb_readback__DOT__nevn = 0;
    IData/*31:0*/ tb_readback__DOT__nckw;
    tb_readback__DOT__nckw = 0;
    IData/*31:0*/ tb_readback__DOT__nprf;
    tb_readback__DOT__nprf = 0;
    IData/*31:0*/ tb_readback__DOT__nthi;
    tb_readback__DOT__nthi = 0;
    IData/*31:0*/ tb_readback__DOT__nmt;
    tb_readback__DOT__nmt = 0;
    IData/*31:0*/ tb_readback__DOT__nmtp;
    tb_readback__DOT__nmtp = 0;
    IData/*31:0*/ tb_readback__DOT__nmras;
    tb_readback__DOT__nmras = 0;
    IData/*31:0*/ tb_readback__DOT__nmcas;
    tb_readback__DOT__nmcas = 0;
    IData/*31:0*/ tb_readback__DOT__nmrd;
    tb_readback__DOT__nmrd = 0;
    IData/*31:0*/ tb_readback__DOT__nmwr;
    tb_readback__DOT__nmwr = 0;
    IData/*31:0*/ tb_readback__DOT__nd13w;
    tb_readback__DOT__nd13w = 0;
    CData/*0:0*/ tb_readback__DOT__pmras;
    tb_readback__DOT__pmras = 0;
    CData/*0:0*/ tb_readback__DOT__pmcas;
    tb_readback__DOT__pmcas = 0;
    IData/*31:0*/ tb_readback__DOT__ntnia;
    tb_readback__DOT__ntnia = 0;
    IData/*31:0*/ tb_readback__DOT__nff0;
    tb_readback__DOT__nff0 = 0;
    IData/*31:0*/ tb_readback__DOT__nsamp;
    tb_readback__DOT__nsamp = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_wpr;
    tb_readback__DOT__nff0_wpr = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_cr;
    tb_readback__DOT__nff0_cr = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_alt;
    tb_readback__DOT__nff0_alt = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_fl;
    tb_readback__DOT__nff0_fl = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_a1;
    tb_readback__DOT__nff0_a1 = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_ign;
    tb_readback__DOT__nff0_ign = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_a0;
    tb_readback__DOT__nff0_a0 = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_ffok;
    tb_readback__DOT__nff0_ffok = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_bad;
    tb_readback__DOT__nff0_bad = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_fs;
    tb_readback__DOT__nff0_fs = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_fm;
    tb_readback__DOT__nff0_fm = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_mia;
    tb_readback__DOT__nff0_mia = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_mib;
    tb_readback__DOT__nff0_mib = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_fsp;
    tb_readback__DOT__nff0_fsp = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_ech;
    tb_readback__DOT__nff0_ech = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_st;
    tb_readback__DOT__nff0_st = 0;
    IData/*31:0*/ tb_readback__DOT__nff0_hcd;
    tb_readback__DOT__nff0_hcd = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_wv;
    tb_readback__DOT__nvc_wv = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_dv;
    tb_readback__DOT__nvc_dv = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_fdm;
    tb_readback__DOT__nvc_fdm = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_fia;
    tb_readback__DOT__nvc_fia = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_vip;
    tb_readback__DOT__nvc_vip = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_via;
    tb_readback__DOT__nvc_via = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_ios;
    tb_readback__DOT__nvc_ios = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_wia;
    tb_readback__DOT__nvc_wia = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_wim;
    tb_readback__DOT__nvc_wim = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_wimem;
    tb_readback__DOT__nvc_wimem = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_smc;
    tb_readback__DOT__nvc_smc = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_sec;
    tb_readback__DOT__nvc_sec = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_smc_e;
    tb_readback__DOT__nvc_smc_e = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_sec_e;
    tb_readback__DOT__nvc_sec_e = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_coin;
    tb_readback__DOT__nvc_coin = 0;
    IData/*31:0*/ tb_readback__DOT__nvc_coin2;
    tb_readback__DOT__nvc_coin2 = 0;
    CData/*2:0*/ tb_readback__DOT__pms;
    tb_readback__DOT__pms = 0;
    CData/*3:0*/ tb_readback__DOT__ppa;
    tb_readback__DOT__ppa = 0;
    CData/*0:0*/ tb_readback__DOT__pmc;
    tb_readback__DOT__pmc = 0;
    IData/*31:0*/ tb_readback__DOT__n0a;
    tb_readback__DOT__n0a = 0;
    IData/*31:0*/ tb_readback__DOT__j2;
    tb_readback__DOT__j2 = 0;
    SData/*15:0*/ tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv;
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv = 0;
    SData/*15:0*/ tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv;
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv = 0;
    IData/*31:0*/ tb_readback__DOT__unnamedblk13__DOT__zi;
    tb_readback__DOT__unnamedblk13__DOT__zi = 0;
    IData/*31:0*/ tb_readback__DOT__unnamedblk14__DOT__mi2;
    tb_readback__DOT__unnamedblk14__DOT__mi2 = 0;
    IData/*31:0*/ tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28;
    tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28 = 0;
    IData/*31:0*/ tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29;
    tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L0__1762__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L0__1762__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L0__1762__idx;
    __Vfunc_tb_readback__DOT__rd_L0__1762__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R0__1763__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R0__1763__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R0__1763__idx;
    __Vfunc_tb_readback__DOT__rd_R0__1763__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L1__1764__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L1__1764__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L1__1764__idx;
    __Vfunc_tb_readback__DOT__rd_L1__1764__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R1__1765__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R1__1765__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R1__1765__idx;
    __Vfunc_tb_readback__DOT__rd_R1__1765__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L2__1766__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L2__1766__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L2__1766__idx;
    __Vfunc_tb_readback__DOT__rd_L2__1766__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R2__1767__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R2__1767__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R2__1767__idx;
    __Vfunc_tb_readback__DOT__rd_R2__1767__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L3__1768__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L3__1768__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L3__1768__idx;
    __Vfunc_tb_readback__DOT__rd_L3__1768__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R3__1769__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R3__1769__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R3__1769__idx;
    __Vfunc_tb_readback__DOT__rd_R3__1769__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L0__1770__idx;
    __Vfunc_tb_readback__DOT__rd_L0__1770__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L1__1771__idx;
    __Vfunc_tb_readback__DOT__rd_L1__1771__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L2__1772__idx;
    __Vfunc_tb_readback__DOT__rd_L2__1772__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_L3__1773__idx;
    __Vfunc_tb_readback__DOT__rd_L3__1773__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R0__1774__idx;
    __Vfunc_tb_readback__DOT__rd_R0__1774__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R1__1775__idx;
    __Vfunc_tb_readback__DOT__rd_R1__1775__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R2__1776__idx;
    __Vfunc_tb_readback__DOT__rd_R2__1776__idx = 0;
    SData/*15:0*/ __Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout;
    __Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__rd_R3__1777__idx;
    __Vfunc_tb_readback__DOT__rd_R3__1777__idx = 0;
    SData/*11:0*/ __Vtask_tb_readback__DOT__manifold__1778__word;
    __Vtask_tb_readback__DOT__manifold__1778__word = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1779__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1779__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1779__n;
    __Vfunc_tb_readback__DOT__WT__1779__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1780__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1780__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1780__n;
    __Vfunc_tb_readback__DOT__WT__1780__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1781__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1781__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1781__n;
    __Vfunc_tb_readback__DOT__WT__1781__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1782__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1782__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1782__n;
    __Vfunc_tb_readback__DOT__WT__1782__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1783__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1783__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1783__n;
    __Vfunc_tb_readback__DOT__WT__1783__n = 0;
    SData/*11:0*/ __Vtask_tb_readback__DOT__manifold__1784__word;
    __Vtask_tb_readback__DOT__manifold__1784__word = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1785__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1785__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1785__n;
    __Vfunc_tb_readback__DOT__WT__1785__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1786__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1786__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1786__n;
    __Vfunc_tb_readback__DOT__WT__1786__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1787__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1787__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1787__n;
    __Vfunc_tb_readback__DOT__WT__1787__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1788__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1788__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1788__n;
    __Vfunc_tb_readback__DOT__WT__1788__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1789__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1789__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1789__n;
    __Vfunc_tb_readback__DOT__WT__1789__n = 0;
    SData/*15:0*/ __Vtask_tb_readback__DOT__set_cpreg_tilde__1790__v;
    __Vtask_tb_readback__DOT__set_cpreg_tilde__1790__v = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1791__fn;
    __Vtask_tb_readback__DOT__strobe__1791__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1791__data;
    __Vtask_tb_readback__DOT__strobe__1791__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1791__ss;
    __Vtask_tb_readback__DOT__strobe__1791__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1792__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1792__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1792__n;
    __Vfunc_tb_readback__DOT__WT__1792__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1793__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1793__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1793__n;
    __Vfunc_tb_readback__DOT__WT__1793__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1794__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1794__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1794__n;
    __Vfunc_tb_readback__DOT__WT__1794__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1795__fn;
    __Vtask_tb_readback__DOT__strobe__1795__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1795__data;
    __Vtask_tb_readback__DOT__strobe__1795__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1795__ss;
    __Vtask_tb_readback__DOT__strobe__1795__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1796__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1796__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1796__n;
    __Vfunc_tb_readback__DOT__WT__1796__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1797__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1797__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1797__n;
    __Vfunc_tb_readback__DOT__WT__1797__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1798__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1798__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1798__n;
    __Vfunc_tb_readback__DOT__WT__1798__n = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__b0;
    __Vtask_tb_readback__DOT__parc_micro__1799__b0 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__b1;
    __Vtask_tb_readback__DOT__parc_micro__1799__b1 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__b2;
    __Vtask_tb_readback__DOT__parc_micro__1799__b2 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__b3;
    __Vtask_tb_readback__DOT__parc_micro__1799__b3 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__b4;
    __Vtask_tb_readback__DOT__parc_micro__1799__b4 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1800__fn;
    __Vtask_tb_readback__DOT__strobe__1800__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1800__data;
    __Vtask_tb_readback__DOT__strobe__1800__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1800__ss;
    __Vtask_tb_readback__DOT__strobe__1800__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1801__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1801__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1801__n;
    __Vfunc_tb_readback__DOT__WT__1801__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1802__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1802__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1802__n;
    __Vfunc_tb_readback__DOT__WT__1802__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1803__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1803__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1803__n;
    __Vfunc_tb_readback__DOT__WT__1803__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1804__fn;
    __Vtask_tb_readback__DOT__strobe__1804__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1804__data;
    __Vtask_tb_readback__DOT__strobe__1804__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1804__ss;
    __Vtask_tb_readback__DOT__strobe__1804__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1805__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1805__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1805__n;
    __Vfunc_tb_readback__DOT__WT__1805__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1806__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1806__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1806__n;
    __Vfunc_tb_readback__DOT__WT__1806__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1807__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1807__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1807__n;
    __Vfunc_tb_readback__DOT__WT__1807__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1808__fn;
    __Vtask_tb_readback__DOT__strobe__1808__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1808__data;
    __Vtask_tb_readback__DOT__strobe__1808__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1808__ss;
    __Vtask_tb_readback__DOT__strobe__1808__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1809__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1809__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1809__n;
    __Vfunc_tb_readback__DOT__WT__1809__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1810__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1810__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1810__n;
    __Vfunc_tb_readback__DOT__WT__1810__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1811__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1811__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1811__n;
    __Vfunc_tb_readback__DOT__WT__1811__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1812__fn;
    __Vtask_tb_readback__DOT__strobe__1812__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1812__data;
    __Vtask_tb_readback__DOT__strobe__1812__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1812__ss;
    __Vtask_tb_readback__DOT__strobe__1812__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1813__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1813__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1813__n;
    __Vfunc_tb_readback__DOT__WT__1813__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1814__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1814__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1814__n;
    __Vfunc_tb_readback__DOT__WT__1814__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1815__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1815__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1815__n;
    __Vfunc_tb_readback__DOT__WT__1815__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1816__fn;
    __Vtask_tb_readback__DOT__strobe__1816__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1816__data;
    __Vtask_tb_readback__DOT__strobe__1816__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1816__ss;
    __Vtask_tb_readback__DOT__strobe__1816__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1817__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1817__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1817__n;
    __Vfunc_tb_readback__DOT__WT__1817__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1818__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1818__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1818__n;
    __Vfunc_tb_readback__DOT__WT__1818__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1819__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1819__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1819__n;
    __Vfunc_tb_readback__DOT__WT__1819__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1820__fn;
    __Vtask_tb_readback__DOT__strobe__1820__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1820__data;
    __Vtask_tb_readback__DOT__strobe__1820__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1820__ss;
    __Vtask_tb_readback__DOT__strobe__1820__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1821__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1821__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1821__n;
    __Vfunc_tb_readback__DOT__WT__1821__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1822__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1822__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1822__n;
    __Vfunc_tb_readback__DOT__WT__1822__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1823__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1823__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1823__n;
    __Vfunc_tb_readback__DOT__WT__1823__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1824__fn;
    __Vtask_tb_readback__DOT__strobe__1824__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1824__data;
    __Vtask_tb_readback__DOT__strobe__1824__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1824__ss;
    __Vtask_tb_readback__DOT__strobe__1824__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1825__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1825__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1825__n;
    __Vfunc_tb_readback__DOT__WT__1825__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1826__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1826__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1826__n;
    __Vfunc_tb_readback__DOT__WT__1826__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1827__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1827__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1827__n;
    __Vfunc_tb_readback__DOT__WT__1827__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1829__fn;
    __Vtask_tb_readback__DOT__strobe__1829__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1829__data;
    __Vtask_tb_readback__DOT__strobe__1829__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1829__ss;
    __Vtask_tb_readback__DOT__strobe__1829__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1830__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1830__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1830__n;
    __Vfunc_tb_readback__DOT__WT__1830__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1831__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1831__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1831__n;
    __Vfunc_tb_readback__DOT__WT__1831__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1832__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1832__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1832__n;
    __Vfunc_tb_readback__DOT__WT__1832__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1833__fn;
    __Vtask_tb_readback__DOT__strobe__1833__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1833__data;
    __Vtask_tb_readback__DOT__strobe__1833__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1833__ss;
    __Vtask_tb_readback__DOT__strobe__1833__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1834__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1834__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1834__n;
    __Vfunc_tb_readback__DOT__WT__1834__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1835__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1835__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1835__n;
    __Vfunc_tb_readback__DOT__WT__1835__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1836__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1836__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1836__n;
    __Vfunc_tb_readback__DOT__WT__1836__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1837__fn;
    __Vtask_tb_readback__DOT__strobe__1837__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1837__data;
    __Vtask_tb_readback__DOT__strobe__1837__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1837__ss;
    __Vtask_tb_readback__DOT__strobe__1837__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1838__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1838__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1838__n;
    __Vfunc_tb_readback__DOT__WT__1838__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1839__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1839__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1839__n;
    __Vfunc_tb_readback__DOT__WT__1839__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1840__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1840__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1840__n;
    __Vfunc_tb_readback__DOT__WT__1840__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1841__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1841__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1841__n;
    __Vfunc_tb_readback__DOT__WT__1841__n = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__b0;
    __Vtask_tb_readback__DOT__parc_micro__1843__b0 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__b1;
    __Vtask_tb_readback__DOT__parc_micro__1843__b1 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__b2;
    __Vtask_tb_readback__DOT__parc_micro__1843__b2 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__b3;
    __Vtask_tb_readback__DOT__parc_micro__1843__b3 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__b4;
    __Vtask_tb_readback__DOT__parc_micro__1843__b4 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1844__fn;
    __Vtask_tb_readback__DOT__strobe__1844__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1844__data;
    __Vtask_tb_readback__DOT__strobe__1844__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1844__ss;
    __Vtask_tb_readback__DOT__strobe__1844__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1845__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1845__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1845__n;
    __Vfunc_tb_readback__DOT__WT__1845__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1846__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1846__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1846__n;
    __Vfunc_tb_readback__DOT__WT__1846__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1847__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1847__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1847__n;
    __Vfunc_tb_readback__DOT__WT__1847__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1848__fn;
    __Vtask_tb_readback__DOT__strobe__1848__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1848__data;
    __Vtask_tb_readback__DOT__strobe__1848__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1848__ss;
    __Vtask_tb_readback__DOT__strobe__1848__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1849__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1849__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1849__n;
    __Vfunc_tb_readback__DOT__WT__1849__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1850__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1850__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1850__n;
    __Vfunc_tb_readback__DOT__WT__1850__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1851__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1851__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1851__n;
    __Vfunc_tb_readback__DOT__WT__1851__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1852__fn;
    __Vtask_tb_readback__DOT__strobe__1852__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1852__data;
    __Vtask_tb_readback__DOT__strobe__1852__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1852__ss;
    __Vtask_tb_readback__DOT__strobe__1852__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1853__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1853__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1853__n;
    __Vfunc_tb_readback__DOT__WT__1853__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1854__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1854__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1854__n;
    __Vfunc_tb_readback__DOT__WT__1854__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1855__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1855__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1855__n;
    __Vfunc_tb_readback__DOT__WT__1855__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1856__fn;
    __Vtask_tb_readback__DOT__strobe__1856__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1856__data;
    __Vtask_tb_readback__DOT__strobe__1856__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1856__ss;
    __Vtask_tb_readback__DOT__strobe__1856__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1857__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1857__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1857__n;
    __Vfunc_tb_readback__DOT__WT__1857__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1858__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1858__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1858__n;
    __Vfunc_tb_readback__DOT__WT__1858__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1859__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1859__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1859__n;
    __Vfunc_tb_readback__DOT__WT__1859__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1860__fn;
    __Vtask_tb_readback__DOT__strobe__1860__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1860__data;
    __Vtask_tb_readback__DOT__strobe__1860__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1860__ss;
    __Vtask_tb_readback__DOT__strobe__1860__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1861__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1861__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1861__n;
    __Vfunc_tb_readback__DOT__WT__1861__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1862__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1862__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1862__n;
    __Vfunc_tb_readback__DOT__WT__1862__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1863__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1863__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1863__n;
    __Vfunc_tb_readback__DOT__WT__1863__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1864__fn;
    __Vtask_tb_readback__DOT__strobe__1864__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1864__data;
    __Vtask_tb_readback__DOT__strobe__1864__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1864__ss;
    __Vtask_tb_readback__DOT__strobe__1864__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1865__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1865__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1865__n;
    __Vfunc_tb_readback__DOT__WT__1865__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1866__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1866__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1866__n;
    __Vfunc_tb_readback__DOT__WT__1866__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1867__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1867__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1867__n;
    __Vfunc_tb_readback__DOT__WT__1867__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1868__fn;
    __Vtask_tb_readback__DOT__strobe__1868__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1868__data;
    __Vtask_tb_readback__DOT__strobe__1868__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1868__ss;
    __Vtask_tb_readback__DOT__strobe__1868__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1869__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1869__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1869__n;
    __Vfunc_tb_readback__DOT__WT__1869__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1870__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1870__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1870__n;
    __Vfunc_tb_readback__DOT__WT__1870__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1871__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1871__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1871__n;
    __Vfunc_tb_readback__DOT__WT__1871__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1873__fn;
    __Vtask_tb_readback__DOT__strobe__1873__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1873__data;
    __Vtask_tb_readback__DOT__strobe__1873__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1873__ss;
    __Vtask_tb_readback__DOT__strobe__1873__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1874__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1874__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1874__n;
    __Vfunc_tb_readback__DOT__WT__1874__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1875__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1875__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1875__n;
    __Vfunc_tb_readback__DOT__WT__1875__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1876__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1876__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1876__n;
    __Vfunc_tb_readback__DOT__WT__1876__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1877__fn;
    __Vtask_tb_readback__DOT__strobe__1877__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1877__data;
    __Vtask_tb_readback__DOT__strobe__1877__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1877__ss;
    __Vtask_tb_readback__DOT__strobe__1877__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1878__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1878__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1878__n;
    __Vfunc_tb_readback__DOT__WT__1878__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1879__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1879__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1879__n;
    __Vfunc_tb_readback__DOT__WT__1879__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1880__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1880__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1880__n;
    __Vfunc_tb_readback__DOT__WT__1880__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1881__fn;
    __Vtask_tb_readback__DOT__strobe__1881__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1881__data;
    __Vtask_tb_readback__DOT__strobe__1881__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1881__ss;
    __Vtask_tb_readback__DOT__strobe__1881__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1882__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1882__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1882__n;
    __Vfunc_tb_readback__DOT__WT__1882__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1883__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1883__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1883__n;
    __Vfunc_tb_readback__DOT__WT__1883__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1884__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1884__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1884__n;
    __Vfunc_tb_readback__DOT__WT__1884__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1885__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1885__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1885__n;
    __Vfunc_tb_readback__DOT__WT__1885__n = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_run__1886__b0;
    __Vtask_tb_readback__DOT__parc_run__1886__b0 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_run__1886__b1;
    __Vtask_tb_readback__DOT__parc_run__1886__b1 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_run__1886__b2;
    __Vtask_tb_readback__DOT__parc_run__1886__b2 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_run__1886__b3;
    __Vtask_tb_readback__DOT__parc_run__1886__b3 = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__parc_run__1886__b4;
    __Vtask_tb_readback__DOT__parc_run__1886__b4 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1887__fn;
    __Vtask_tb_readback__DOT__strobe__1887__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1887__data;
    __Vtask_tb_readback__DOT__strobe__1887__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1887__ss;
    __Vtask_tb_readback__DOT__strobe__1887__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1888__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1888__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1888__n;
    __Vfunc_tb_readback__DOT__WT__1888__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1889__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1889__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1889__n;
    __Vfunc_tb_readback__DOT__WT__1889__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1890__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1890__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1890__n;
    __Vfunc_tb_readback__DOT__WT__1890__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1891__fn;
    __Vtask_tb_readback__DOT__strobe__1891__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1891__data;
    __Vtask_tb_readback__DOT__strobe__1891__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1891__ss;
    __Vtask_tb_readback__DOT__strobe__1891__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1892__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1892__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1892__n;
    __Vfunc_tb_readback__DOT__WT__1892__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1893__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1893__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1893__n;
    __Vfunc_tb_readback__DOT__WT__1893__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1894__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1894__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1894__n;
    __Vfunc_tb_readback__DOT__WT__1894__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1895__fn;
    __Vtask_tb_readback__DOT__strobe__1895__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1895__data;
    __Vtask_tb_readback__DOT__strobe__1895__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1895__ss;
    __Vtask_tb_readback__DOT__strobe__1895__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1896__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1896__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1896__n;
    __Vfunc_tb_readback__DOT__WT__1896__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1897__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1897__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1897__n;
    __Vfunc_tb_readback__DOT__WT__1897__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1898__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1898__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1898__n;
    __Vfunc_tb_readback__DOT__WT__1898__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1899__fn;
    __Vtask_tb_readback__DOT__strobe__1899__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1899__data;
    __Vtask_tb_readback__DOT__strobe__1899__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1899__ss;
    __Vtask_tb_readback__DOT__strobe__1899__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1900__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1900__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1900__n;
    __Vfunc_tb_readback__DOT__WT__1900__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1901__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1901__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1901__n;
    __Vfunc_tb_readback__DOT__WT__1901__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1902__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1902__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1902__n;
    __Vfunc_tb_readback__DOT__WT__1902__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1903__fn;
    __Vtask_tb_readback__DOT__strobe__1903__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1903__data;
    __Vtask_tb_readback__DOT__strobe__1903__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1903__ss;
    __Vtask_tb_readback__DOT__strobe__1903__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1904__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1904__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1904__n;
    __Vfunc_tb_readback__DOT__WT__1904__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1905__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1905__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1905__n;
    __Vfunc_tb_readback__DOT__WT__1905__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1906__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1906__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1906__n;
    __Vfunc_tb_readback__DOT__WT__1906__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1907__fn;
    __Vtask_tb_readback__DOT__strobe__1907__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1907__data;
    __Vtask_tb_readback__DOT__strobe__1907__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1907__ss;
    __Vtask_tb_readback__DOT__strobe__1907__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1908__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1908__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1908__n;
    __Vfunc_tb_readback__DOT__WT__1908__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1909__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1909__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1909__n;
    __Vfunc_tb_readback__DOT__WT__1909__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1910__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1910__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1910__n;
    __Vfunc_tb_readback__DOT__WT__1910__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1911__fn;
    __Vtask_tb_readback__DOT__strobe__1911__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1911__data;
    __Vtask_tb_readback__DOT__strobe__1911__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1911__ss;
    __Vtask_tb_readback__DOT__strobe__1911__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1912__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1912__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1912__n;
    __Vfunc_tb_readback__DOT__WT__1912__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1913__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1913__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1913__n;
    __Vfunc_tb_readback__DOT__WT__1913__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1914__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1914__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1914__n;
    __Vfunc_tb_readback__DOT__WT__1914__n = 0;
    CData/*2:0*/ __Vtask_tb_readback__DOT__strobe__1915__fn;
    __Vtask_tb_readback__DOT__strobe__1915__fn = 0;
    CData/*7:0*/ __Vtask_tb_readback__DOT__strobe__1915__data;
    __Vtask_tb_readback__DOT__strobe__1915__data = 0;
    CData/*0:0*/ __Vtask_tb_readback__DOT__strobe__1915__ss;
    __Vtask_tb_readback__DOT__strobe__1915__ss = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1916__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1916__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1916__n;
    __Vfunc_tb_readback__DOT__WT__1916__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1917__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1917__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1917__n;
    __Vfunc_tb_readback__DOT__WT__1917__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1918__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1918__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1918__n;
    __Vfunc_tb_readback__DOT__WT__1918__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1919__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1919__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1919__n;
    __Vfunc_tb_readback__DOT__WT__1919__n = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1920__Vfuncout;
    __Vfunc_tb_readback__DOT__WT__1920__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_readback__DOT__WT__1920__n;
    __Vfunc_tb_readback__DOT__WT__1920__n = 0;
    // Body
    VL_WRITEF_NX("tb_readback: IM[0..3] overwritten with ASEL=1 FF=100B references\n",0);
    VL_WRITEF_NX("tb_readback:   IM[0] L=%x R=%x   IM[1] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_L0__1762__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_L0__1762__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L0__1762__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_L0__1762__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_R0__1763__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_R0__1763__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R0__1763__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_R0__1763__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_L1__1764__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_L1__1764__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L1__1764__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_L1__1764__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_R1__1765__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_R1__1765__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R1__1765__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_R1__1765__Vfuncout)));
    VL_WRITEF_NX("tb_readback:   IM[2] L=%x R=%x   IM[3] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_L2__1766__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_L2__1766__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L2__1766__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_L2__1766__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_R2__1767__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_R2__1767__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R2__1767__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_R2__1767__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_L3__1768__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_L3__1768__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L3__1768__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_L3__1768__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_readback__DOT__rd_R3__1769__idx = 0U;
                    __Vfunc_tb_readback__DOT__rd_R3__1769__Vfuncout 
                        = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                  << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                    << 1U) | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)])) 
                               << 4U) | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R3__1769__idx)]))));
                }(), (IData)(__Vfunc_tb_readback__DOT__rd_R3__1769__Vfuncout)));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L0__1770__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R0__1774__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 0 idx 0 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L0__1770__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R0__1774__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 0 idx 1 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L0__1770__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L0__1770__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L0__1770__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R0__1774__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R0__1774__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R0__1774__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 0 idx 2 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L1__1771__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R1__1775__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 1 idx 0 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L1__1771__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R1__1775__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 1 idx 1 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L1__1771__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L1__1771__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L1__1771__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R1__1775__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R1__1775__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R1__1775__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 1 idx 2 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L2__1772__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R2__1776__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 2 idx 0 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L2__1772__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R2__1776__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 2 idx 1 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L2__1772__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L2__1772__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L2__1772__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R2__1776__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R2__1776__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R2__1776__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 2 idx 2 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L3__1773__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R3__1777__idx = 0U;
            __Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 3 idx 0 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L3__1773__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R3__1777__idx = 1U;
            __Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 3 idx 1 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_L3__1773__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_L3__1773__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_L3__1773__Vfuncout));
    tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_readback__DOT__rd_R3__1777__idx = 2U;
            __Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout 
                = ((((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                        << 3U) | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_readback__DOT__rd_R3__1777__idx)]))));
        }(), (IData)(__Vfunc_tb_readback__DOT__rd_R3__1777__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_readback:   IM bank 3 idx 2 : L=%x R=%x\n",0,
                     16,tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_readback__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    __Vtask_tb_readback__DOT__manifold__1778__word = 0x01c0U;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_readback__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__k)) {
        vlSelfRef.tb_readback__DOT__dmd = ((0x0bU >= 
                                            (0x0000000fU 
                                             & vlSelfRef.tb_readback__DOT__k)) 
                                           && (1U & 
                                               ((IData)(__Vtask_tb_readback__DOT__manifold__1778__word) 
                                                >> 
                                                (0x0000000fU 
                                                 & vlSelfRef.tb_readback__DOT__k))));
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmd;
        __Vfunc_tb_readback__DOT__WT__1779__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1779__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1779__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1779__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1779__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_readback__DOT__WT__1779__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1361);
            __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__dmc = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmc;
        __Vfunc_tb_readback__DOT__WT__1780__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1780__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1780__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1780__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1780__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_readback__DOT__WT__1780__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1362);
            __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__dmc = 0U;
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmc;
        __Vfunc_tb_readback__DOT__WT__1781__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1781__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1781__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1781__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1781__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_readback__DOT__WT__1781__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1363);
            __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__k = (vlSelfRef.tb_readback__DOT__k 
                                         - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__udmd = 1U;
    vlSelfRef.tb_readback__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_readback__DOT__udmd;
    __Vfunc_tb_readback__DOT__WT__1782__n = 0x0000000cU;
    __Vfunc_tb_readback__DOT__WT__1782__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1782__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1782__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1782__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_readback__DOT__WT__1782__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1365);
        __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__udmd = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_readback__DOT__udmd;
    __Vfunc_tb_readback__DOT__WT__1783__n = 0x0000000cU;
    __Vfunc_tb_readback__DOT__WT__1783__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1783__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1783__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1783__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_readback__DOT__WT__1783__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1366);
        __Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_readback__DOT__manifold__1778__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__manifold__1784__word = 0U;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_readback__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__k)) {
        vlSelfRef.tb_readback__DOT__dmd = ((0x0bU >= 
                                            (0x0000000fU 
                                             & vlSelfRef.tb_readback__DOT__k)) 
                                           && (1U & 
                                               ((IData)(__Vtask_tb_readback__DOT__manifold__1784__word) 
                                                >> 
                                                (0x0000000fU 
                                                 & vlSelfRef.tb_readback__DOT__k))));
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmd;
        __Vfunc_tb_readback__DOT__WT__1785__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1785__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1785__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1785__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1785__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_readback__DOT__WT__1785__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1361);
            __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__dmc = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmc;
        __Vfunc_tb_readback__DOT__WT__1786__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1786__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1786__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1786__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1786__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_readback__DOT__WT__1786__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1362);
            __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__dmc = 0U;
        vlSelfRef.tb_readback__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_readback__DOT__dmc;
        __Vfunc_tb_readback__DOT__WT__1787__n = 4U;
        __Vfunc_tb_readback__DOT__WT__1787__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1787__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1787__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1787__Vfuncout = 1U;
        }
        __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_readback__DOT__WT__1787__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 1363);
            __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_readback__DOT__k = (vlSelfRef.tb_readback__DOT__k 
                                         - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__udmd = 1U;
    vlSelfRef.tb_readback__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_readback__DOT__udmd;
    __Vfunc_tb_readback__DOT__WT__1788__n = 0x0000000cU;
    __Vfunc_tb_readback__DOT__WT__1788__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1788__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1788__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1788__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_readback__DOT__WT__1788__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1365);
        __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__udmd = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_readback__DOT__udmd;
    __Vfunc_tb_readback__DOT__WT__1789__n = 0x0000000cU;
    __Vfunc_tb_readback__DOT__WT__1789__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1789__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1789__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1789__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_readback__DOT__WT__1789__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1366);
        __Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_readback__DOT__manifold__1784__tb_readback__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_readback: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b\ntb_readback: MCR sequence -- T=%x (want 0043), MCR DisHold=%b\ntb_readback: before start -- PRhold=%b Hold=%b DisHold=%b\n",0,
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__IMLHPEenable,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__IMRHPEenable),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__StopMIRClkEn,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__StopMIRClk),
                 16,(((((((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                 << 1U)) | (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                  >> 1U))) 
                         << 6U) | (((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                           >> 1U)) 
                                    | (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                             >> 3U))) 
                                   << 4U)) | ((((2U 
                                                 & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    << 1U)) 
                                                | (1U 
                                                   & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                      >> 1U))) 
                                               << 2U) 
                                              | ((2U 
                                                  & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                     >> 1U)) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                       >> 3U))))) 
                      << 8U) | (((((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          >> 1U))) 
                                  << 6U) | (((2U & 
                                              ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                               >> 1U)) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                   >> 3U))) 
                                            << 4U)) 
                                | ((((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                            << 1U)) 
                                     | (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 3U)))))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__DisHold___05FMemC),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__PRhold,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__Hold___05FMemC),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__DisHold___05FMemC);
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__PRhold) 
                      | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__Hold___05FMemC))))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the memory is holding the processor before the machine starts\n",0);
    }
    __Vtask_tb_readback__DOT__set_cpreg_tilde__1790__v = 0U;
    __Vtask_tb_readback__DOT__strobe__1791__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1791__data = 
        (0x000000ffU & (~ ((IData)(__Vtask_tb_readback__DOT__set_cpreg_tilde__1790__v) 
                           >> 8U)));
    __Vtask_tb_readback__DOT__strobe__1791__fn = 2U;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1791__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1791__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1791__ss));
    __Vfunc_tb_readback__DOT__WT__1792__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1792__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1792__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1792__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1792__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1792__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1793__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1793__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1793__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1793__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1793__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1793__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1791__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1791__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1791__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1794__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1794__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1794__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1794__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1794__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1794__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1791__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1795__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1795__data = 
        (0x000000ffU & (~ (IData)(__Vtask_tb_readback__DOT__set_cpreg_tilde__1790__v)));
    __Vtask_tb_readback__DOT__strobe__1795__fn = 3U;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1795__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1795__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1795__ss));
    __Vfunc_tb_readback__DOT__WT__1796__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1796__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1796__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1796__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1796__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1796__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1797__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1797__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1797__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1797__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1797__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1797__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1795__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1795__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1795__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1798__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1798__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1798__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1798__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1798__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1798__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1795__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__b4 = 0x40U;
    __Vtask_tb_readback__DOT__parc_micro__1799__b3 = 4U;
    __Vtask_tb_readback__DOT__parc_micro__1799__b2 = 0xefU;
    __Vtask_tb_readback__DOT__parc_micro__1799__b1 = 0x13U;
    __Vtask_tb_readback__DOT__parc_micro__1799__b0 = 0x30U;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_readback__DOT__strobe__1800__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1800__data = 0x21U;
    __Vtask_tb_readback__DOT__strobe__1800__fn = 1U;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1800__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1800__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1800__ss));
    __Vfunc_tb_readback__DOT__WT__1801__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1801__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1801__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1801__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1801__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1801__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1802__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1802__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1802__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1802__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1802__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1802__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1800__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1800__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1800__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1803__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1803__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1803__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1803__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1803__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1803__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1800__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1920);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1804__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1804__data = 0x4eU;
    __Vtask_tb_readback__DOT__strobe__1804__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1804__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1804__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1804__ss));
    __Vfunc_tb_readback__DOT__WT__1805__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1805__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1805__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1805__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1805__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1805__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1806__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1806__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1806__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1806__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1806__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1806__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1804__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1804__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1804__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1807__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1807__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1807__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1807__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1807__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1807__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1804__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1922);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1808__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1808__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1808__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1808__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1808__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1808__ss));
    __Vfunc_tb_readback__DOT__WT__1809__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1809__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1809__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1809__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1809__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1809__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1810__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1810__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1810__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1810__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1810__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1810__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1808__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1808__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1808__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1811__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1811__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1811__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1811__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1811__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1811__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1808__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1924);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1812__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1799__b0) 
                                                     >> 7U));
    __Vtask_tb_readback__DOT__strobe__1812__data = __Vtask_tb_readback__DOT__parc_micro__1799__b1;
    __Vtask_tb_readback__DOT__strobe__1812__fn = 4U;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1812__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1812__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1812__ss));
    __Vfunc_tb_readback__DOT__WT__1813__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1813__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1813__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1813__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1813__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1813__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1814__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1814__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1814__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1814__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1814__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1814__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1812__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1812__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1812__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1815__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1815__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1815__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1815__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1815__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1815__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1812__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1816__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1799__b0) 
                                                     >> 6U));
    __Vtask_tb_readback__DOT__strobe__1816__data = __Vtask_tb_readback__DOT__parc_micro__1799__b2;
    __Vtask_tb_readback__DOT__strobe__1816__fn = 5U;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1816__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1816__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1816__ss));
    __Vfunc_tb_readback__DOT__WT__1817__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1817__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1817__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1817__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1817__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1817__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1818__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1818__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1818__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1818__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1818__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1818__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1816__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1816__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1816__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1819__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1819__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1819__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1819__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1819__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1819__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1816__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1820__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1799__b0) 
                                                     >> 5U));
    __Vtask_tb_readback__DOT__strobe__1820__data = __Vtask_tb_readback__DOT__parc_micro__1799__b3;
    __Vtask_tb_readback__DOT__strobe__1820__fn = 6U;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1820__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1820__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1820__ss));
    __Vfunc_tb_readback__DOT__WT__1821__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1821__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1821__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1821__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1821__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1821__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1822__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1822__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1822__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1822__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1822__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1822__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1820__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1820__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1820__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1823__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1823__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1823__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1823__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1823__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1823__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1820__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1824__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1799__b0) 
                                                     >> 4U));
    __Vtask_tb_readback__DOT__strobe__1824__data = __Vtask_tb_readback__DOT__parc_micro__1799__b4;
    __Vtask_tb_readback__DOT__strobe__1824__fn = 7U;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1824__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1824__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1824__ss));
    __Vfunc_tb_readback__DOT__WT__1825__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1825__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1825__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1825__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1825__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1825__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1826__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1826__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1826__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1826__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1826__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1826__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1824__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1824__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1824__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1827__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1827__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1827__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1827__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1827__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1827__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1824__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__n0 = 0U;
    vlSelfRef.tb_readback__DOT__n1 = 0U;
    vlSelfRef.tb_readback__DOT__n2 = 0U;
    __Vtask_tb_readback__DOT__strobe__1829__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1829__data = 1U;
    __Vtask_tb_readback__DOT__strobe__1829__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1829__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1829__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1829__ss));
    __Vfunc_tb_readback__DOT__WT__1830__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1830__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1830__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1830__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1830__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1830__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1831__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1831__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1831__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1831__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1831__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1831__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1829__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1829__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1829__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1832__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1832__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1832__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1832__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1832__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1832__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1829__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1932);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1833__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1833__data = 1U;
    __Vtask_tb_readback__DOT__strobe__1833__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1833__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1833__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1833__ss));
    __Vfunc_tb_readback__DOT__WT__1834__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1834__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1834__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1834__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1834__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1834__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1835__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1835__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1835__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1835__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1835__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1835__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1833__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1833__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1833__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1836__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1836__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1836__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1836__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1836__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1836__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1833__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1934);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1837__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1837__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1837__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1837__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1837__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1837__ss));
    __Vfunc_tb_readback__DOT__WT__1838__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1838__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1838__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1838__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1838__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1838__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1839__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1839__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1839__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1839__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1839__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1839__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1837__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1837__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1837__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1840__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1840__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1840__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1840__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1840__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1840__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1837__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_readback__DOT__WT__1841__n = 0x00000320U;
    __Vfunc_tb_readback__DOT__WT__1841__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1841__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1841__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1841__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_readback__DOT__WT__1841__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1936);
        __Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_readback__DOT__parc_micro__1799__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_readback__DOT__parc_micro__1799__b1,
                 32,vlSelfRef.tb_readback__DOT__n0,
                 32,vlSelfRef.tb_readback__DOT__n1,
                 32,vlSelfRef.tb_readback__DOT__n2,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
    __Vtask_tb_readback__DOT__parc_micro__1843__b4 = 0x40U;
    __Vtask_tb_readback__DOT__parc_micro__1843__b3 = 0x4cU;
    __Vtask_tb_readback__DOT__parc_micro__1843__b2 = 0x0fU;
    __Vtask_tb_readback__DOT__parc_micro__1843__b1 = 1U;
    __Vtask_tb_readback__DOT__parc_micro__1843__b0 = 0x70U;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_readback__DOT__strobe__1844__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1844__data = 0x21U;
    __Vtask_tb_readback__DOT__strobe__1844__fn = 1U;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1844__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1844__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1844__ss));
    __Vfunc_tb_readback__DOT__WT__1845__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1845__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1845__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1845__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1845__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1845__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1846__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1846__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1846__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1846__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1846__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1846__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1844__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1844__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1844__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1847__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1847__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1847__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1847__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1847__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1847__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1844__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1920);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1848__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1848__data = 0x4eU;
    __Vtask_tb_readback__DOT__strobe__1848__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1848__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1848__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1848__ss));
    __Vfunc_tb_readback__DOT__WT__1849__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1849__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1849__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1849__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1849__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1849__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1850__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1850__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1850__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1850__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1850__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1850__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1848__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1848__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1848__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1851__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1851__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1851__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1851__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1851__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1851__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1848__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1922);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1852__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1852__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1852__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1852__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1852__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1852__ss));
    __Vfunc_tb_readback__DOT__WT__1853__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1853__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1853__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1853__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1853__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1853__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1854__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1854__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1854__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1854__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1854__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1854__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1852__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1852__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1852__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1855__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1855__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1855__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1855__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1855__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1855__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1852__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1924);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1856__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1843__b0) 
                                                     >> 7U));
    __Vtask_tb_readback__DOT__strobe__1856__data = __Vtask_tb_readback__DOT__parc_micro__1843__b1;
    __Vtask_tb_readback__DOT__strobe__1856__fn = 4U;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1856__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1856__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1856__ss));
    __Vfunc_tb_readback__DOT__WT__1857__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1857__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1857__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1857__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1857__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1857__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1858__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1858__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1858__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1858__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1858__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1858__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1856__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1856__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1856__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1859__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1859__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1859__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1859__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1859__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1859__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1856__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1860__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1843__b0) 
                                                     >> 6U));
    __Vtask_tb_readback__DOT__strobe__1860__data = __Vtask_tb_readback__DOT__parc_micro__1843__b2;
    __Vtask_tb_readback__DOT__strobe__1860__fn = 5U;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1860__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1860__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1860__ss));
    __Vfunc_tb_readback__DOT__WT__1861__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1861__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1861__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1861__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1861__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1861__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1862__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1862__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1862__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1862__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1862__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1862__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1860__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1860__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1860__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1863__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1863__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1863__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1863__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1863__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1863__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1860__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1864__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1843__b0) 
                                                     >> 5U));
    __Vtask_tb_readback__DOT__strobe__1864__data = __Vtask_tb_readback__DOT__parc_micro__1843__b3;
    __Vtask_tb_readback__DOT__strobe__1864__fn = 6U;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1864__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1864__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1864__ss));
    __Vfunc_tb_readback__DOT__WT__1865__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1865__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1865__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1865__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1865__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1865__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1866__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1866__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1866__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1866__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1866__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1866__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1864__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1864__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1864__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1867__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1867__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1867__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1867__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1867__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1867__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1864__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1868__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_micro__1843__b0) 
                                                     >> 4U));
    __Vtask_tb_readback__DOT__strobe__1868__data = __Vtask_tb_readback__DOT__parc_micro__1843__b4;
    __Vtask_tb_readback__DOT__strobe__1868__fn = 7U;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1868__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1868__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1868__ss));
    __Vfunc_tb_readback__DOT__WT__1869__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1869__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1869__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1869__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1869__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1869__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1870__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1870__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1870__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1870__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1870__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1870__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1868__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1868__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1868__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1871__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1871__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1871__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1871__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1871__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1871__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1868__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__n0 = 0U;
    vlSelfRef.tb_readback__DOT__n1 = 0U;
    vlSelfRef.tb_readback__DOT__n2 = 0U;
    __Vtask_tb_readback__DOT__strobe__1873__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1873__data = 1U;
    __Vtask_tb_readback__DOT__strobe__1873__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1873__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1873__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1873__ss));
    __Vfunc_tb_readback__DOT__WT__1874__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1874__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1874__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1874__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1874__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1874__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1875__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1875__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1875__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1875__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1875__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1875__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1873__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1873__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1873__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1876__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1876__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1876__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1876__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1876__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1876__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1873__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1932);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1877__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1877__data = 1U;
    __Vtask_tb_readback__DOT__strobe__1877__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1877__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1877__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1877__ss));
    __Vfunc_tb_readback__DOT__WT__1878__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1878__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1878__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1878__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1878__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1878__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1879__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1879__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1879__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1879__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1879__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1879__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1877__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1877__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1877__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1880__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1880__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1880__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1880__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1880__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1880__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1877__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1934);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1881__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1881__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1881__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1881__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1881__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1881__ss));
    __Vfunc_tb_readback__DOT__WT__1882__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1882__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1882__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1882__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1882__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1882__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1883__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1883__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1883__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1883__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1883__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1883__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1881__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1881__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1881__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1884__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1884__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1884__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1884__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1884__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1884__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1881__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_readback__DOT__WT__1885__n = 0x00000320U;
    __Vfunc_tb_readback__DOT__WT__1885__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1885__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1885__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1885__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_readback__DOT__WT__1885__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1936);
        __Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_readback__DOT__parc_micro__1843__tb_readback__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_readback: Link[4:15]=%x\n",0,
                 8,__Vtask_tb_readback__DOT__parc_micro__1843__b1,
                 32,vlSelfRef.tb_readback__DOT__n0,
                 32,vlSelfRef.tb_readback__DOT__n1,
                 32,vlSelfRef.tb_readback__DOT__n2,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))));
    __Vtask_tb_readback__DOT__parc_run__1886__b4 = 0x43U;
    __Vtask_tb_readback__DOT__parc_run__1886__b3 = 0x4aU;
    __Vtask_tb_readback__DOT__parc_run__1886__b2 = 0xe1U;
    __Vtask_tb_readback__DOT__parc_run__1886__b1 = 0x13U;
    __Vtask_tb_readback__DOT__parc_run__1886__b0 = 0x60U;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_readback__DOT__strobe__1887__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1887__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1887__fn = 1U;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1887__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1887__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1887__ss));
    __Vfunc_tb_readback__DOT__WT__1888__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1888__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1888__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1888__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1888__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1888__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1889__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1889__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1889__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1889__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1889__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1889__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1887__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1887__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1887__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1890__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1890__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1890__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1890__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1890__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1890__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1887__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             2200);
        __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23 
            = (__Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_24__DOT____Vrepeat23 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1891__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1891__data = 0x4eU;
    __Vtask_tb_readback__DOT__strobe__1891__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1891__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1891__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1891__ss));
    __Vfunc_tb_readback__DOT__WT__1892__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1892__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1892__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1892__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1892__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1892__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1893__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1893__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1893__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1893__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1893__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1893__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1891__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1891__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1891__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1894__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1894__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1894__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1894__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1894__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1894__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1891__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             2202);
        __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1895__ss = 1U;
    __Vtask_tb_readback__DOT__strobe__1895__data = 0U;
    __Vtask_tb_readback__DOT__strobe__1895__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1895__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1895__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1895__ss));
    __Vfunc_tb_readback__DOT__WT__1896__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1896__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1896__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1896__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1896__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1896__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1897__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1897__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1897__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1897__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1897__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1897__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1895__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1895__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1895__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1898__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1898__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1898__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1898__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1898__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1898__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1895__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             2204);
        __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1899__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_run__1886__b0) 
                                                     >> 7U));
    __Vtask_tb_readback__DOT__strobe__1899__data = __Vtask_tb_readback__DOT__parc_run__1886__b1;
    __Vtask_tb_readback__DOT__strobe__1899__fn = 4U;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1899__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1899__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1899__ss));
    __Vfunc_tb_readback__DOT__WT__1900__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1900__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1900__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1900__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1900__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1900__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1901__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1901__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1901__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1901__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1901__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1901__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1899__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1899__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1899__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1902__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1902__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1902__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1902__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1902__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1902__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1899__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1903__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_run__1886__b0) 
                                                     >> 6U));
    __Vtask_tb_readback__DOT__strobe__1903__data = __Vtask_tb_readback__DOT__parc_run__1886__b2;
    __Vtask_tb_readback__DOT__strobe__1903__fn = 5U;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1903__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1903__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1903__ss));
    __Vfunc_tb_readback__DOT__WT__1904__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1904__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1904__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1904__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1904__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1904__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1905__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1905__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1905__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1905__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1905__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1905__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1903__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1903__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1903__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1906__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1906__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1906__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1906__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1906__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1906__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1903__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1907__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_run__1886__b0) 
                                                     >> 5U));
    __Vtask_tb_readback__DOT__strobe__1907__data = __Vtask_tb_readback__DOT__parc_run__1886__b3;
    __Vtask_tb_readback__DOT__strobe__1907__fn = 6U;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1907__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1907__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1907__ss));
    __Vfunc_tb_readback__DOT__WT__1908__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1908__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1908__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1908__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1908__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1908__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1909__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1909__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1909__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1909__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1909__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1909__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1907__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1907__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1907__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1910__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1910__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1910__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1910__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1910__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1910__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1907__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1911__ss = (1U 
                                                  & ((IData)(__Vtask_tb_readback__DOT__parc_run__1886__b0) 
                                                     >> 4U));
    __Vtask_tb_readback__DOT__strobe__1911__data = __Vtask_tb_readback__DOT__parc_run__1886__b4;
    __Vtask_tb_readback__DOT__strobe__1911__fn = 7U;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1911__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1911__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1911__ss));
    __Vfunc_tb_readback__DOT__WT__1912__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1912__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1912__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1912__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1912__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1912__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1913__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1913__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1913__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1913__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1913__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1913__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1911__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1911__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1911__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1914__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1914__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1914__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1914__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1914__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1914__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1911__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__setss_n = 1U;
    __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             2210);
        __Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_readback__DOT__parc_run__1886__tb_readback__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_readback__DOT__strobe__1915__ss = 0U;
    __Vtask_tb_readback__DOT__strobe__1915__data = 1U;
    __Vtask_tb_readback__DOT__strobe__1915__fn = 0U;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_readback__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_readback__DOT__strobe__1915__fn)));
    vlSelfRef.tb_readback__DOT__cpout = (((IData)(__Vtask_tb_readback__DOT__strobe__1915__data) 
                                          << 1U) | (IData)(__Vtask_tb_readback__DOT__strobe__1915__ss));
    __Vfunc_tb_readback__DOT__WT__1916__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1916__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1916__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1916__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1916__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_readback__DOT__WT__1916__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1342);
        __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 0U;
    __Vfunc_tb_readback__DOT__WT__1917__n = 6U;
    __Vfunc_tb_readback__DOT__WT__1917__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1917__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1917__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1917__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_readback__DOT__WT__1917__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1343);
        __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_readback__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_readback__DOT__strobe__1915__fn))) {
        vlSelfRef.tb_readback__DOT__setrun = (1U & (IData)(__Vtask_tb_readback__DOT__strobe__1915__data));
        vlSelfRef.tb_readback__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_readback__DOT__strobe__1915__ss)));
    }
    __Vfunc_tb_readback__DOT__WT__1918__n = 4U;
    __Vfunc_tb_readback__DOT__WT__1918__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1918__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1918__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1918__Vfuncout = 1U;
    }
    __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_readback__DOT__WT__1918__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             1349);
        __Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_readback__DOT__strobe__1915__tb_readback__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    tb_readback__DOT__n0a = 0U;
    tb_readback__DOT__nmemclk = 0U;
    tb_readback__DOT__npipe = 0U;
    tb_readback__DOT__ppa = vlSelfRef.tb_readback__DOT__pipead;
    tb_readback__DOT__nras = 0U;
    tb_readback__DOT__ncas = 0U;
    tb_readback__DOT__nwe = 0U;
    tb_readback__DOT__prasa = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa;
    tb_readback__DOT__pcasa = vlSelfRef.tb_readback__DOT__m__DOT__MemCASa___05FMemX;
    tb_readback__DOT__pwea = vlSelfRef.tb_readback__DOT__m__DOT__MemWEa___05FMemX;
    tb_readback__DOT__nmx = 0U;
    tb_readback__DOT__pmx = ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
                             | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                                | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7)));
    tb_readback__DOT__nrp = 0U;
    tb_readback__DOT__prp = vlSelfRef.tb_readback__DOT__rfshper;
    tb_readback__DOT__nmr = 0U;
    tb_readback__DOT__pmr = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                   >> 3U));
    tb_readback__DOT__nms = 0U;
    tb_readback__DOT__pms = (7U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
    tb_readback__DOT__nsq = 0U;
    tb_readback__DOT__psq = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave;
    tb_readback__DOT__nsrc = 0U;
    tb_readback__DOT__psrc = (1U & ((~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                                        ^ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                                    | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14)));
    tb_readback__DOT__nwr = 0U;
    tb_readback__DOT__pwr = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb)));
    tb_readback__DOT__nnr = 0U;
    tb_readback__DOT__pnr = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_;
    tb_readback__DOT__nmrf = 0U;
    tb_readback__DOT__pmrf = vlSelfRef.tb_readback__DOT__m__DOT__MapRfsh_p___05F_MemX;
    tb_readback__DOT__nsm = 0U;
    tb_readback__DOT__psm = vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC;
    tb_readback__DOT__nmw = 0U;
    tb_readback__DOT__pmw = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait;
    tb_readback__DOT__npsm = 0U;
    tb_readback__DOT__ppsm = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                    >> 5U));
    tb_readback__DOT__nwmw = 0U;
    tb_readback__DOT__pwmw = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_;
    tb_readback__DOT__ng13 = 0U;
    tb_readback__DOT__pg13 = ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFree) 
                              & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC));
    tb_readback__DOT__nxsm = 0U;
    tb_readback__DOT__pxsm = vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC;
    tb_readback__DOT__nwpr = 0U;
    tb_readback__DOT__pwpr = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_;
    tb_readback__DOT__nrh = 0U;
    tb_readback__DOT__prh = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa;
    tb_readback__DOT__nldp = 0U;
    tb_readback__DOT__pldp = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__LdPair_p_;
    tb_readback__DOT__npha = 0U;
    tb_readback__DOT__ppha = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)));
    tb_readback__DOT__ncra = 0U;
    tb_readback__DOT__pcra = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)));
    tb_readback__DOT__nha = 0U;
    tb_readback__DOT__pha = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)));
    tb_readback__DOT__nhb = 0U;
    tb_readback__DOT__phb = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)));
    vlSelfRef.tb_readback__DOT__i14_hit[0U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[1U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[2U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[3U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[4U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[5U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[6U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[7U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[8U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[9U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0aU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0bU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0cU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0dU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0eU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x0fU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x10U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x11U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x12U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x13U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x14U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x15U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x16U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x17U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x18U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x19U] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1aU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1bU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1cU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1dU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1eU] = 0U;
    vlSelfRef.tb_readback__DOT__i14_hit[0x1fU] = 0U;
    tb_readback__DOT__runlen = 0U;
    tb_readback__DOT__maxrun = 0U;
    tb_readback__DOT__nwin = 0U;
    tb_readback__DOT__winat = 0xffffffffU;
    tb_readback__DOT__pmfree = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                      >> 2U));
    tb_readback__DOT__pmidle = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle;
    tb_readback__DOT__nmfree_e = 0U;
    tb_readback__DOT__nmidle_e = 0U;
    tb_readback__DOT__nmfree_hi = 0U;
    tb_readback__DOT__nrfsh = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[0U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[0U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[0U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[1U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[1U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[1U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[2U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[2U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[2U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[3U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[3U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[3U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[4U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[4U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[4U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[5U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[5U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[5U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[6U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[6U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[6U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_at[7U] = 0U;
    vlSelfRef.tb_readback__DOT__x10_at[7U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_seen[7U] = 0U;
    tb_readback__DOT__npsh = 0U;
    tb_readback__DOT__ncoin = 0U;
    tb_readback__DOT__nmwlo = 0U;
    tb_readback__DOT__lastpsh = 0xffffffffU;
    tb_readback__DOT__lastcoin = 0xffffffffU;
    tb_readback__DOT__lastmwlo = 0xffffffffU;
    tb_readback__DOT__lastmf00 = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf_cnt[0U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_cnt[1U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_cnt[2U] = 0U;
    vlSelfRef.tb_readback__DOT__mf_cnt[3U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_cnt[0U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[0U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[1U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[1U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[2U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[2U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[3U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[3U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[4U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[4U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[5U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[5U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[6U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[6U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3_cnt[7U] = 0U;
    vlSelfRef.tb_readback__DOT__ms3_last[7U] = 0xffffffffU;
    tb_readback__DOT__pmf00 = 0U;
    tb_readback__DOT__pms3 = 0U;
    tb_readback__DOT__nmf00on = 0U;
    tb_readback__DOT__nms3on = 0U;
    vlSelfRef.tb_readback__DOT__mf00at[0U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[0U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[1U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[1U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[2U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[2U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[3U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[3U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[4U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[4U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[5U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[5U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[6U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[6U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[7U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[7U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[8U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[8U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[9U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[9U] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[0x0aU] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[0x0aU] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__mf00at[0x0bU] = 0xffffffffU;
    vlSelfRef.tb_readback__DOT__ms3at[0x0bU] = 0xffffffffU;
    tb_readback__DOT__ndd = 0U;
    tb_readback__DOT__nidle_lo = 0U;
    tb_readback__DOT__pdd = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd;
    tb_readback__DOT__nmemst = 0U;
    tb_readback__DOT__nfree = 0U;
    tb_readback__DOT__nmemfr = 0U;
    tb_readback__DOT__nheld_nz = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[1U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[2U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[3U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[4U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[5U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[6U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[7U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[8U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[9U] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0aU] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0bU] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0cU] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0dU] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0eU] = 0U;
    vlSelfRef.tb_readback__DOT__memfr_hit[0x0fU] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[0U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[1U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[2U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[3U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[4U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[5U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[6U] = 0U;
    vlSelfRef.tb_readback__DOT__memst_hit[7U] = 0U;
    tb_readback__DOT__nmapst = 0U;
    tb_readback__DOT__nmapfn = 0U;
    tb_readback__DOT__npsm2 = 0U;
    tb_readback__DOT__nsm2 = 0U;
    tb_readback__DOT__nload = 0U;
    tb_readback__DOT__ncnt = 0U;
    tb_readback__DOT__nd0 = 0U;
    tb_readback__DOT__nwim = 0U;
    tb_readback__DOT__nx10 = 0U;
    tb_readback__DOT__nmti = 0U;
    tb_readback__DOT__nwm = 0U;
    tb_readback__DOT__nall3 = 0U;
    tb_readback__DOT__nrw = 0U;
    tb_readback__DOT__nwp = 0U;
    tb_readback__DOT__ndty = 0U;
    tb_readback__DOT__nevn = 0U;
    tb_readback__DOT__nckw = 0U;
    tb_readback__DOT__nprf = 0U;
    tb_readback__DOT__nthi = 0U;
    tb_readback__DOT__nmt = 0U;
    tb_readback__DOT__nmtp = 0U;
    tb_readback__DOT__nmras = 0U;
    tb_readback__DOT__nmcas = 0U;
    tb_readback__DOT__nmrd = 0U;
    tb_readback__DOT__nmwr = 0U;
    tb_readback__DOT__nd13w = 0U;
    tb_readback__DOT__pmras = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapRAS_p_;
    tb_readback__DOT__pmcas = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapCAS_p_;
    vlSelfRef.tb_readback__DOT__mapst_hit[0U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[1U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[2U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[3U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[4U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[5U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[6U] = 0U;
    vlSelfRef.tb_readback__DOT__mapst_hit[7U] = 0U;
    vlSelfRef.tb_readback__DOT__mapfn_hit[0U] = 0U;
    vlSelfRef.tb_readback__DOT__mapfn_hit[1U] = 0U;
    vlSelfRef.tb_readback__DOT__mapfn_hit[2U] = 0U;
    vlSelfRef.tb_readback__DOT__mapfn_hit[3U] = 0U;
    tb_readback__DOT__ntnia = 0U;
    tb_readback__DOT__nff0 = 0U;
    tb_readback__DOT__nsamp = 0U;
    tb_readback__DOT__nff0_wpr = 0U;
    tb_readback__DOT__nff0_cr = 0U;
    tb_readback__DOT__nff0_alt = 0U;
    tb_readback__DOT__nff0_fl = 0U;
    tb_readback__DOT__nff0_a1 = 0U;
    tb_readback__DOT__nff0_ign = 0U;
    tb_readback__DOT__nff0_a0 = 0U;
    tb_readback__DOT__nff0_ffok = 0U;
    tb_readback__DOT__nff0_bad = 0U;
    tb_readback__DOT__nff0_fs = 0U;
    tb_readback__DOT__nff0_fm = 0U;
    tb_readback__DOT__nff0_mia = 0U;
    tb_readback__DOT__nff0_mib = 0U;
    tb_readback__DOT__nff0_fsp = 0U;
    tb_readback__DOT__nff0_ech = 0U;
    tb_readback__DOT__nff0_st = 0U;
    tb_readback__DOT__nff0_hcd = 0U;
    tb_readback__DOT__nvc_wv = 0U;
    tb_readback__DOT__nvc_dv = 0U;
    tb_readback__DOT__nvc_fdm = 0U;
    tb_readback__DOT__nvc_fia = 0U;
    tb_readback__DOT__nvc_vip = 0U;
    tb_readback__DOT__nvc_via = 0U;
    tb_readback__DOT__nvc_ios = 0U;
    tb_readback__DOT__nvc_wia = 0U;
    tb_readback__DOT__nvc_wim = 0U;
    tb_readback__DOT__nvc_wimem = 0U;
    tb_readback__DOT__nvc_smc = 0U;
    tb_readback__DOT__nvc_sec = 0U;
    tb_readback__DOT__nvc_smc_e = 0U;
    tb_readback__DOT__nvc_sec_e = 0U;
    tb_readback__DOT__nvc_coin = 0U;
    tb_readback__DOT__nvc_coin2 = 0U;
    tb_readback__DOT__unnamedblk13__DOT__zi = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_readback__DOT__unnamedblk13__DOT__zi)) {
        vlSelfRef.tb_readback__DOT__tnia_hit[(0x00000fffU 
                                              & tb_readback__DOT__unnamedblk13__DOT__zi)] = 0U;
        tb_readback__DOT__unnamedblk13__DOT__zi = ((IData)(1U) 
                                                   + tb_readback__DOT__unnamedblk13__DOT__zi);
    }
    tb_readback__DOT__nwcr = 0U;
    tb_readback__DOT__pwcr = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)));
    tb_readback__DOT__nwar = 0U;
    tb_readback__DOT__pwar = (1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                                    | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)));
    tb_readback__DOT__nfl = 0U;
    tb_readback__DOT__pfl = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                   >> 3U));
    tb_readback__DOT__nmp = 0U;
    tb_readback__DOT__pmp = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                   >> 1U));
    tb_readback__DOT__unnamedblk14__DOT__mi2 = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_readback__DOT__unnamedblk14__DOT__mi2)) {
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a04__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a05__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a06__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a07__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a08__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a09__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a10__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a11__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a12__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a13__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a14__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d04__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d05__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d06__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d07__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d08__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d09__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d10__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d12__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__mem[(0x00000fffU 
                                                                          & tb_readback__DOT__unnamedblk14__DOT__mi2)] = 1U;
        tb_readback__DOT__unnamedblk14__DOT__mi2 = 
            ((IData)(1U) + tb_readback__DOT__unnamedblk14__DOT__mi2);
    }
    VL_WRITEF_NX("tb_readback: ALL 21 map bit planes preloaded to 1 (parity experiment)\n",0);
    vlSelfRef.tb_readback__DOT__p0 = vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca;
    tb_readback__DOT__pmc = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__clk0_p_A;
    tb_readback__DOT__j2 = 0U;
    while (VL_GTS_III(32, 0x00000bb8U, tb_readback__DOT__j2)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             2511);
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca) 
             != (IData)(vlSelfRef.tb_readback__DOT__p0))) {
            tb_readback__DOT__n0a = ((IData)(1U) + tb_readback__DOT__n0a);
            vlSelfRef.tb_readback__DOT__p0 = vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__clk0_p_A) 
             != (IData)(tb_readback__DOT__pmc))) {
            tb_readback__DOT__nmemclk = ((IData)(1U) 
                                         + tb_readback__DOT__nmemclk);
            tb_readback__DOT__pmc = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__clk0_p_A;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__pipead) 
             != (IData)(tb_readback__DOT__ppa))) {
            tb_readback__DOT__npipe = ((IData)(1U) 
                                       + tb_readback__DOT__npipe);
            tb_readback__DOT__ppa = vlSelfRef.tb_readback__DOT__pipead;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa) 
             != (IData)(tb_readback__DOT__prasa))) {
            tb_readback__DOT__nras = ((IData)(1U) + tb_readback__DOT__nras);
            tb_readback__DOT__prasa = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MemCASa___05FMemX) 
             != (IData)(tb_readback__DOT__pcasa))) {
            tb_readback__DOT__ncas = ((IData)(1U) + tb_readback__DOT__ncas);
            tb_readback__DOT__pcasa = vlSelfRef.tb_readback__DOT__m__DOT__MemCASa___05FMemX;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MemWEa___05FMemX) 
             != (IData)(tb_readback__DOT__pwea))) {
            tb_readback__DOT__nwe = ((IData)(1U) + tb_readback__DOT__nwe);
            tb_readback__DOT__pwea = vlSelfRef.tb_readback__DOT__m__DOT__MemWEa___05FMemX;
        }
        if ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
              | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                 | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7))) 
             != (IData)(tb_readback__DOT__pmx))) {
            tb_readback__DOT__nmx = ((IData)(1U) + tb_readback__DOT__nmx);
            tb_readback__DOT__pmx = ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
                                     | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                                        | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7)));
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__rfshper) 
             != (IData)(tb_readback__DOT__prp))) {
            tb_readback__DOT__nrp = ((IData)(1U) + tb_readback__DOT__nrp);
            tb_readback__DOT__prp = vlSelfRef.tb_readback__DOT__rfshper;
        }
        if (((1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                    >> 3U)) != (IData)(tb_readback__DOT__pmr))) {
            tb_readback__DOT__nmr = ((IData)(1U) + tb_readback__DOT__nmr);
            tb_readback__DOT__pmr = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                           >> 3U));
        }
        if (((7U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q)) 
             != (IData)(tb_readback__DOT__pms))) {
            tb_readback__DOT__nms = ((IData)(1U) + tb_readback__DOT__nms);
            tb_readback__DOT__pms = (7U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
             != (IData)(tb_readback__DOT__psq))) {
            tb_readback__DOT__nsq = ((IData)(1U) + tb_readback__DOT__nsq);
            tb_readback__DOT__psq = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave;
        }
        if (((1U & ((~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                        ^ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                    | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14))) 
             != (IData)(tb_readback__DOT__psrc))) {
            tb_readback__DOT__nsrc = ((IData)(1U) + tb_readback__DOT__nsrc);
            tb_readback__DOT__psrc = (1U & ((~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                                                ^ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                                            | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb))) 
             != (IData)(tb_readback__DOT__pwr))) {
            tb_readback__DOT__nwr = ((IData)(1U) + tb_readback__DOT__nwr);
            tb_readback__DOT__pwr = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb)));
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_) 
             != (IData)(tb_readback__DOT__pnr))) {
            tb_readback__DOT__nnr = ((IData)(1U) + tb_readback__DOT__nnr);
            tb_readback__DOT__pnr = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MapRfsh_p___05F_MemX) 
             != (IData)(tb_readback__DOT__pmrf))) {
            tb_readback__DOT__nmrf = ((IData)(1U) + tb_readback__DOT__nmrf);
            tb_readback__DOT__pmrf = vlSelfRef.tb_readback__DOT__m__DOT__MapRfsh_p___05F_MemX;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC) 
             != (IData)(tb_readback__DOT__psm))) {
            tb_readback__DOT__nsm = ((IData)(1U) + tb_readback__DOT__nsm);
            tb_readback__DOT__psm = vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait) 
             != (IData)(tb_readback__DOT__pmw))) {
            tb_readback__DOT__nmw = ((IData)(1U) + tb_readback__DOT__nmw);
            tb_readback__DOT__pmw = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
             != (IData)(tb_readback__DOT__pwpr))) {
            tb_readback__DOT__nwpr = ((IData)(1U) + tb_readback__DOT__nwpr);
            tb_readback__DOT__pwpr = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa) 
             != (IData)(tb_readback__DOT__prh))) {
            tb_readback__DOT__nrh = ((IData)(1U) + tb_readback__DOT__nrh);
            tb_readback__DOT__prh = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__LdPair_p_) 
             != (IData)(tb_readback__DOT__pldp))) {
            tb_readback__DOT__nldp = ((IData)(1U) + tb_readback__DOT__nldp);
            tb_readback__DOT__pldp = vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__LdPair_p_;
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))) 
             != (IData)(tb_readback__DOT__ppha))) {
            tb_readback__DOT__npha = ((IData)(1U) + tb_readback__DOT__npha);
            tb_readback__DOT__ppha = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_))) 
             != (IData)(tb_readback__DOT__pcra))) {
            tb_readback__DOT__ncra = ((IData)(1U) + tb_readback__DOT__ncra);
            tb_readback__DOT__pcra = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))) 
             != (IData)(tb_readback__DOT__pha))) {
            tb_readback__DOT__nha = ((IData)(1U) + tb_readback__DOT__nha);
            tb_readback__DOT__pha = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))) 
             != (IData)(tb_readback__DOT__phb))) {
            tb_readback__DOT__nhb = ((IData)(1U) + tb_readback__DOT__nhb);
            tb_readback__DOT__phb = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_))) 
             != (IData)(tb_readback__DOT__pwcr))) {
            tb_readback__DOT__nwcr = ((IData)(1U) + tb_readback__DOT__nwcr);
            tb_readback__DOT__pwcr = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)));
        }
        tb_readback__DOT__nsamp = ((IData)(1U) + tb_readback__DOT__nsamp);
        vlSelfRef.tb_readback__DOT__mapst_now = (7U 
                                                 & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        vlSelfRef.tb_readback__DOT__mapfn_now = (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_));
        if ((1U & (~ vlSelfRef.tb_readback__DOT__mapst_hit
                   [vlSelfRef.tb_readback__DOT__mapst_now]))) {
            vlSelfRef.tb_readback__DOT__mapst_hit[vlSelfRef.tb_readback__DOT__mapst_now] = 1U;
            tb_readback__DOT__nmapst = ((IData)(1U) 
                                        + tb_readback__DOT__nmapst);
        }
        vlSelfRef.tb_readback__DOT__i14a = (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                             << 4U) 
                                            | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_) 
                                                << 3U) 
                                               | (7U 
                                                  & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q))));
        vlSelfRef.tb_readback__DOT__i14_hit[vlSelfRef.tb_readback__DOT__i14a] 
            = ((IData)(1U) + vlSelfRef.tb_readback__DOT__i14_hit
               [vlSelfRef.tb_readback__DOT__i14a]);
        if ((1U & (~ vlSelfRef.tb_readback__DOT__mapfn_hit
                   [vlSelfRef.tb_readback__DOT__mapfn_now]))) {
            vlSelfRef.tb_readback__DOT__mapfn_hit[vlSelfRef.tb_readback__DOT__mapfn_now] = 1U;
            tb_readback__DOT__nmapfn = ((IData)(1U) 
                                        + tb_readback__DOT__nmapfn);
        }
        if ((1U & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                      >> 5U)))) {
            tb_readback__DOT__npsm2 = ((IData)(1U) 
                                       + tb_readback__DOT__npsm2);
        }
        if ((0x00000020U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5))) {
            tb_readback__DOT__npsh = ((IData)(1U) + tb_readback__DOT__npsh);
            tb_readback__DOT__lastpsh = tb_readback__DOT__nsamp;
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait)))) {
                tb_readback__DOT__ncoin = ((IData)(1U) 
                                           + tb_readback__DOT__ncoin);
                tb_readback__DOT__lastcoin = tb_readback__DOT__nsamp;
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait)))) {
            tb_readback__DOT__nmwlo = ((IData)(1U) 
                                       + tb_readback__DOT__nmwlo);
            tb_readback__DOT__lastmwlo = tb_readback__DOT__nsamp;
        }
        vlSelfRef.tb_readback__DOT__mf_now = (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_));
        vlSelfRef.tb_readback__DOT__mf_cnt[vlSelfRef.tb_readback__DOT__mf_now] 
            = ((IData)(1U) + vlSelfRef.tb_readback__DOT__mf_cnt
               [vlSelfRef.tb_readback__DOT__mf_now]);
        if ((0U == (IData)(vlSelfRef.tb_readback__DOT__mf_now))) {
            tb_readback__DOT__lastmf00 = tb_readback__DOT__nsamp;
            if ((1U & (~ (IData)(tb_readback__DOT__pmf00)))) {
                if (VL_GTS_III(32, 0x0000000cU, tb_readback__DOT__nmf00on)) {
                    vlSelfRef.tb_readback__DOT____Vlvbound_h58252910__0 
                        = tb_readback__DOT__nsamp;
                    if (VL_LIKELY(((0x0bU >= (0x0000000fU 
                                              & tb_readback__DOT__nmf00on))))) {
                        vlSelfRef.tb_readback__DOT__mf00at[(0x0000000fU 
                                                            & tb_readback__DOT__nmf00on)] 
                            = vlSelfRef.tb_readback__DOT____Vlvbound_h58252910__0;
                    }
                }
                tb_readback__DOT__nmf00on = ((IData)(1U) 
                                             + tb_readback__DOT__nmf00on);
            }
            tb_readback__DOT__pmf00 = 1U;
        } else {
            tb_readback__DOT__pmf00 = 0U;
        }
        vlSelfRef.tb_readback__DOT__ms3_now = (7U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        vlSelfRef.tb_readback__DOT__ms3_cnt[vlSelfRef.tb_readback__DOT__ms3_now] 
            = ((IData)(1U) + vlSelfRef.tb_readback__DOT__ms3_cnt
               [vlSelfRef.tb_readback__DOT__ms3_now]);
        vlSelfRef.tb_readback__DOT__ms3_last[vlSelfRef.tb_readback__DOT__ms3_now] 
            = tb_readback__DOT__nsamp;
        if ((3U == (IData)(vlSelfRef.tb_readback__DOT__ms3_now))) {
            if ((1U & (~ (IData)(tb_readback__DOT__pms3)))) {
                if (VL_GTS_III(32, 0x0000000cU, tb_readback__DOT__nms3on)) {
                    vlSelfRef.tb_readback__DOT____Vlvbound_hff9270b6__0 
                        = tb_readback__DOT__nsamp;
                    if (VL_LIKELY(((0x0bU >= (0x0000000fU 
                                              & tb_readback__DOT__nms3on))))) {
                        vlSelfRef.tb_readback__DOT__ms3at[(0x0000000fU 
                                                           & tb_readback__DOT__nms3on)] 
                            = vlSelfRef.tb_readback__DOT____Vlvbound_hff9270b6__0;
                    }
                }
                tb_readback__DOT__nms3on = ((IData)(1U) 
                                            + tb_readback__DOT__nms3on);
            }
            tb_readback__DOT__pms3 = 1U;
        } else {
            tb_readback__DOT__pms3 = 0U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                      >> 2U)))) {
            tb_readback__DOT__nsm2 = ((IData)(1U) + tb_readback__DOT__nsm2);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle)))) {
            tb_readback__DOT__nload = ((IData)(1U) 
                                       + tb_readback__DOT__nload);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__TrueBD)))) {
            tb_readback__DOT__ncnt = ((IData)(1U) + tb_readback__DOT__ncnt);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_) 
                      | (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_12) 
                          >> 7U) | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))))) {
            tb_readback__DOT__nd0 = ((IData)(1U) + tb_readback__DOT__nd0);
        }
        if ((1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                   & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))))) {
            tb_readback__DOT__nwm = ((IData)(1U) + tb_readback__DOT__nwm);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                    & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))) 
                   & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_12) 
                         >> 7U))))) {
            tb_readback__DOT__nall3 = ((IData)(1U) 
                                       + tb_readback__DOT__nall3);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)))) {
            tb_readback__DOT__nwim = ((IData)(1U) + tb_readback__DOT__nwim);
        }
        if ((1U & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_12) 
                      >> 7U)))) {
            tb_readback__DOT__nx10 = ((IData)(1U) + tb_readback__DOT__nx10);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))) {
            tb_readback__DOT__nmti = ((IData)(1U) + tb_readback__DOT__nmti);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__ReadOrWriteInMap_p_)))) {
            tb_readback__DOT__nrw = ((IData)(1U) + tb_readback__DOT__nrw);
        }
        if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWP) {
            tb_readback__DOT__nwp = ((IData)(1U) + tb_readback__DOT__nwp);
        }
        if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapDirtya) {
            tb_readback__DOT__ndty = ((IData)(1U) + tb_readback__DOT__ndty);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapEven_p_)))) {
            tb_readback__DOT__nevn = ((IData)(1U) + tb_readback__DOT__nevn);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__Store_u_InMap_p_) 
                      & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_))))) {
            tb_readback__DOT__nckw = ((IData)(1U) + tb_readback__DOT__nckw);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_e15__DOT__p14)))) {
            tb_readback__DOT__nthi = ((IData)(1U) + tb_readback__DOT__nthi);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                      >> 3U)))) {
            tb_readback__DOT__nprf = ((IData)(1U) + tb_readback__DOT__nprf);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTrouble_p_)))) {
            tb_readback__DOT__nmt = ((IData)(1U) + tb_readback__DOT__nmt);
        }
        if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapTrouble_p_) {
            tb_readback__DOT__nmtp = ((IData)(1U) + tb_readback__DOT__nmtp);
        }
        vlSelfRef.tb_readback__DOT__memst_grp = (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshInMem) 
                                                  << 1U) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                       >> 3U)));
        vlSelfRef.tb_readback__DOT__memst_now = (7U 
                                                 & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q));
        if ((1U & (~ vlSelfRef.tb_readback__DOT__memst_hit
                   [vlSelfRef.tb_readback__DOT__memst_now]))) {
            vlSelfRef.tb_readback__DOT__memst_hit[vlSelfRef.tb_readback__DOT__memst_now] = 1U;
            tb_readback__DOT__nmemst = ((IData)(1U) 
                                        + tb_readback__DOT__nmemst);
        }
        if ((4U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q))) {
            tb_readback__DOT__nfree = ((IData)(1U) 
                                       + tb_readback__DOT__nfree);
            tb_readback__DOT__runlen = ((IData)(1U) 
                                        + tb_readback__DOT__runlen);
            if (VL_GTS_III(32, tb_readback__DOT__runlen, tb_readback__DOT__maxrun)) {
                tb_readback__DOT__maxrun = tb_readback__DOT__runlen;
            }
            if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd) 
                 != (IData)(tb_readback__DOT__pdd))) {
                tb_readback__DOT__ndd = ((IData)(1U) 
                                         + tb_readback__DOT__ndd);
                tb_readback__DOT__pdd = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd;
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle)))) {
                tb_readback__DOT__nidle_lo = ((IData)(1U) 
                                              + tb_readback__DOT__nidle_lo);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshInMem) {
                tb_readback__DOT__nrfsh = ((IData)(1U) 
                                           + tb_readback__DOT__nrfsh);
            }
            vlSelfRef.tb_readback__DOT__mf_at[vlSelfRef.tb_readback__DOT__memst_now] 
                = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                         >> 2U));
            vlSelfRef.tb_readback__DOT__x10_at[vlSelfRef.tb_readback__DOT__memst_now] 
                = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_12) 
                         >> 7U));
            vlSelfRef.tb_readback__DOT__mf_seen[vlSelfRef.tb_readback__DOT__memst_now] = 1U;
            if (((1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                        >> 2U)) != (IData)(tb_readback__DOT__pmfree))) {
                tb_readback__DOT__nmfree_e = ((IData)(1U) 
                                              + tb_readback__DOT__nmfree_e);
                tb_readback__DOT__pmfree = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                                  >> 2U));
            }
            if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle) 
                 != (IData)(tb_readback__DOT__pmidle))) {
                tb_readback__DOT__nmidle_e = ((IData)(1U) 
                                              + tb_readback__DOT__nmidle_e);
                tb_readback__DOT__pmidle = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle;
            }
            if ((4U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q))) {
                tb_readback__DOT__nmfree_hi = ((IData)(1U) 
                                               + tb_readback__DOT__nmfree_hi);
            }
            if ((1U & (~ vlSelfRef.tb_readback__DOT__memfr_hit
                       [vlSelfRef.tb_readback__DOT__memst_now]))) {
                vlSelfRef.tb_readback__DOT__memfr_hit[vlSelfRef.tb_readback__DOT__memst_now] = 1U;
                tb_readback__DOT__nmemfr = ((IData)(1U) 
                                            + tb_readback__DOT__nmemfr);
            }
        } else {
            tb_readback__DOT__runlen = 0U;
            if ((0U != (IData)(vlSelfRef.tb_readback__DOT__memst_now))) {
                tb_readback__DOT__nheld_nz = ((IData)(1U) 
                                              + tb_readback__DOT__nheld_nz);
            }
        }
        if ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
              >> 2U) & (1U == tb_readback__DOT__runlen))) {
            tb_readback__DOT__nwin = ((IData)(1U) + tb_readback__DOT__nwin);
            tb_readback__DOT__winat = tb_readback__DOT__nsamp;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapRAS_p_) 
             != (IData)(tb_readback__DOT__pmras))) {
            tb_readback__DOT__nmras = ((IData)(1U) 
                                       + tb_readback__DOT__nmras);
            tb_readback__DOT__pmras = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapRAS_p_;
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapCAS_p_) 
             != (IData)(tb_readback__DOT__pmcas))) {
            tb_readback__DOT__nmcas = ((IData)(1U) 
                                       + tb_readback__DOT__nmcas);
            tb_readback__DOT__pmcas = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapCAS_p_;
        }
        if ((((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapRAS_p_)) 
              & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
             & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWE_p_))) {
            tb_readback__DOT__nmrd = ((IData)(1U) + tb_readback__DOT__nmrd);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapRAS_p_)) 
                    & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
                   & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWE_p_))))) {
            tb_readback__DOT__nmwr = ((IData)(1U) + tb_readback__DOT__nmwr);
            tb_readback__DOT__nd13w = ((IData)(1U) 
                                       + tb_readback__DOT__nd13w);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd)) 
                    & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ASEL_2))) 
                   & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_))))) {
            tb_readback__DOT__nff0_st = ((IData)(1U) 
                                         + tb_readback__DOT__nff0_st);
        }
        if (((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd)) 
             & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ASEL_2))) {
            tb_readback__DOT__nff0 = ((IData)(1U) + tb_readback__DOT__nff0);
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)))) {
                tb_readback__DOT__nff0_wpr = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_wpr);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_) {
                tb_readback__DOT__nff0_cr = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_cr);
            }
            if ((1U & (~ ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                          | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))))) {
                tb_readback__DOT__nff0_alt = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_alt);
            }
            if ((1U & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                          >> 3U)))) {
                tb_readback__DOT__nff0_fl = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_fl);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__FlushStore) {
                tb_readback__DOT__nff0_fs = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_fs);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__HitColDirty) {
                tb_readback__DOT__nff0_hcd = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_hcd);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantVic_p_)))) {
                tb_readback__DOT__nvc_wv = ((IData)(1U) 
                                            + tb_readback__DOT__nvc_wv);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) {
                tb_readback__DOT__nvc_dv = ((IData)(1U) 
                                            + tb_readback__DOT__nvc_dv);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) {
                tb_readback__DOT__nvc_fdm = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_fdm);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)))) {
                tb_readback__DOT__nvc_fia = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_fia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__VicInPair_p___05F_MemC)))) {
                tb_readback__DOT__nvc_vip = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_vip);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)))) {
                tb_readback__DOT__nvc_via = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_via);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__IoStoreInA_p_)))) {
                tb_readback__DOT__nvc_ios = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_ios);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInA_p_)))) {
                tb_readback__DOT__nvc_wia = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_wia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_)))) {
                tb_readback__DOT__nvc_wim = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_wim);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)))) {
                tb_readback__DOT__nvc_wimem = ((IData)(1U) 
                                               + tb_readback__DOT__nvc_wimem);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a) {
                tb_readback__DOT__nvc_smc = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_smc);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_) {
                tb_readback__DOT__nvc_sec = ((IData)(1U) 
                                             + tb_readback__DOT__nvc_sec);
            }
            if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a) 
                 & (~ (IData)(vlSelfRef.tb_readback__DOT__smc_d)))) {
                tb_readback__DOT__nvc_smc_e = ((IData)(1U) 
                                               + tb_readback__DOT__nvc_smc_e);
                if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInA_p_)))) {
                    tb_readback__DOT__nvc_coin = ((IData)(1U) 
                                                  + tb_readback__DOT__nvc_coin);
                }
            }
            if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_) 
                 & (~ (IData)(vlSelfRef.tb_readback__DOT__sec_d)))) {
                tb_readback__DOT__nvc_sec_e = ((IData)(1U) 
                                               + tb_readback__DOT__nvc_sec_e);
                if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_)))) {
                    tb_readback__DOT__nvc_coin2 = ((IData)(1U) 
                                                   + tb_readback__DOT__nvc_coin2);
                }
            }
            vlSelfRef.tb_readback__DOT__smc_d = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a;
            vlSelfRef.tb_readback__DOT__sec_d = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_;
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__FSinPair_p_)))) {
                tb_readback__DOT__nff0_fsp = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_fsp);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__EcHasAb)))) {
                tb_readback__DOT__nff0_ech = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_ech);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ForceMiss) {
                tb_readback__DOT__nff0_fm = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_fm);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)))) {
                tb_readback__DOT__nff0_mia = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_mia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)))) {
                tb_readback__DOT__nff0_mib = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_mib);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceRd) {
                tb_readback__DOT__nff0_a1 = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_a1);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__IgnoreProc) {
                tb_readback__DOT__nff0_ign = ((IData)(1U) 
                                              + tb_readback__DOT__nff0_ign);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd) {
                tb_readback__DOT__nff0_a0 = ((IData)(1U) 
                                             + tb_readback__DOT__nff0_a0);
            }
            if (vlSelfRef.tb_readback__DOT__m__DOT__FFok_p_a___05FContA) {
                tb_readback__DOT__nff0_ffok = ((IData)(1U) 
                                               + tb_readback__DOT__nff0_ffok);
                if (vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceRd) {
                    tb_readback__DOT__nff0_bad = ((IData)(1U) 
                                                  + tb_readback__DOT__nff0_bad);
                }
                if ((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceRd)))) {
                    tb_readback__DOT__nff0_bad = ((IData)(1U) 
                                                  + tb_readback__DOT__nff0_bad);
                }
            }
        }
        if (VL_UNLIKELY(((1U & (~ vlSelfRef.tb_readback__DOT__tnia_hit
                                [((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_04) 
                                      << 5U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_05) 
                                                 << 4U) 
                                                | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_06) 
                                                   << 3U))) 
                                    | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_07) 
                                        << 2U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_08) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_09)))) 
                                   << 6U) | ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_10) 
                                               << 5U) 
                                              | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_11) 
                                                  << 4U) 
                                                 | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_12) 
                                                    << 3U))) 
                                             | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_13) 
                                                 << 2U) 
                                                | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_14) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_15)))))]))))) {
            vlSelfRef.tb_readback__DOT__tnia_hit[((
                                                   ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_04) 
                                                      << 5U) 
                                                     | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_05) 
                                                         << 4U) 
                                                        | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_06) 
                                                           << 3U))) 
                                                    | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_07) 
                                                        << 2U) 
                                                       | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_08) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_09)))) 
                                                   << 6U) 
                                                  | ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_10) 
                                                       << 5U) 
                                                      | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_11) 
                                                          << 4U) 
                                                         | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_12) 
                                                            << 3U))) 
                                                     | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_13) 
                                                         << 2U) 
                                                        | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_14) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_15)))))] = 1U;
            tb_readback__DOT__ntnia = ((IData)(1U) 
                                       + tb_readback__DOT__ntnia);
            VL_WRITEF_NX("tb_readback:   TNIA visits %x (FF.0mem'=%b ASEL.0=%b ASEL.1'=%b ASEL.2=%b)\n",0,
                         12,((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_04) 
                                 << 5U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_05) 
                                            << 4U) 
                                           | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_06) 
                                              << 3U))) 
                               | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_07) 
                                   << 2U) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_08) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_09)))) 
                              << 6U) | ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_10) 
                                          << 5U) | 
                                         (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_11) 
                                           << 4U) | 
                                          ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_12) 
                                           << 3U))) 
                                        | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_13) 
                                            << 2U) 
                                           | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_14) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__TNIA_15))))),
                         1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceRd),
                         1,vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd,
                         1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceRd),
                         1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ASEL_2);
        }
        if (((1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                    | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))) 
             != (IData)(tb_readback__DOT__pwar))) {
            tb_readback__DOT__nwar = ((IData)(1U) + tb_readback__DOT__nwar);
            tb_readback__DOT__pwar = (1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                                            | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)));
        }
        if (((1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                    >> 3U)) != (IData)(tb_readback__DOT__pfl))) {
            tb_readback__DOT__nfl = ((IData)(1U) + tb_readback__DOT__nfl);
            tb_readback__DOT__pfl = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                           >> 3U));
        }
        if (((1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                    >> 1U)) != (IData)(tb_readback__DOT__pmp))) {
            tb_readback__DOT__nmp = ((IData)(1U) + tb_readback__DOT__nmp);
            tb_readback__DOT__pmp = (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                           >> 1U));
        }
        if (((1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                    >> 5U)) != (IData)(tb_readback__DOT__ppsm))) {
            tb_readback__DOT__npsm = ((IData)(1U) + tb_readback__DOT__npsm);
            tb_readback__DOT__ppsm = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                            >> 5U));
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_) 
             != (IData)(tb_readback__DOT__pwmw))) {
            tb_readback__DOT__nwmw = ((IData)(1U) + tb_readback__DOT__nwmw);
            tb_readback__DOT__pwmw = vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_;
        }
        if ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFree) 
              & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC)) 
             != (IData)(tb_readback__DOT__pg13))) {
            tb_readback__DOT__ng13 = ((IData)(1U) + tb_readback__DOT__ng13);
            tb_readback__DOT__pg13 = ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFree) 
                                      & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC));
        }
        if (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC) 
             != (IData)(tb_readback__DOT__pxsm))) {
            tb_readback__DOT__nxsm = ((IData)(1U) + tb_readback__DOT__nxsm);
            tb_readback__DOT__pxsm = vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC;
        }
        tb_readback__DOT__j2 = ((IData)(1U) + tb_readback__DOT__j2);
    }
    VL_WRITEF_NX("tb_readback: storage strobes over the run -- MemRASa %0d, MemCASa %0d, MemWEa %0d\ntb_readback: WantProcRef' edges %0d, RefHold' edges %0d (is the RUNNING microcode asking?)\ntb_readback:   LdPair' edges %0d -- AfreeOrEc'a=%b EcKeepsAbusy=%b\ntb_readback:   cache -- Hit'a=%b Hit'b=%b (edges %0d/%0d) | PairHasA edges %0d, CacheRefInA edges %0d\ntb_readback:   WHERE IS IT -- TNIA hit %0d distinct values; FF.0mem'=1 on %0d of %0d samples\n",0,
                 32,tb_readback__DOT__nras,32,tb_readback__DOT__ncas,
                 32,tb_readback__DOT__nwe,32,tb_readback__DOT__nwpr,
                 32,tb_readback__DOT__nrh,32,tb_readback__DOT__nldp,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__EcKeepsAbusy___05FMemC,
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))),
                 32,tb_readback__DOT__nha,32,tb_readback__DOT__nhb,
                 32,tb_readback__DOT__npha,32,tb_readback__DOT__ncra,
                 32,tb_readback__DOT__ntnia,32,tb_readback__DOT__nff0,
                 32,tb_readback__DOT__nsamp);
    VL_WRITEF_NX("tb_readback:   WHILE THE ASEL=1 REFERENCE IS RUNNING (%0d samples) -- ASEL.1'=1 %0d, WantProcRef'=0 %0d, WantCR=0 %0d, WantAltRef'=0 %0d, Flush'=0 %0d\n",0,
                 32,tb_readback__DOT__nff0,32,tb_readback__DOT__nff0_a1,
                 32,tb_readback__DOT__nff0_wpr,32,tb_readback__DOT__nff0_cr,
                 32,tb_readback__DOT__nff0_alt,32,tb_readback__DOT__nff0_fl);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000064U, tb_readback__DOT__nff0)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the ASEL=1 reference barely ran (%0d samples) -- it must be the executing instruction\n",0,
                     32,tb_readback__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_readback__DOT__nff0_wpr != tb_readback__DOT__nff0)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) WantProcRef' was not asserted on every cycle the reference ran (%0d of %0d)\n",0,
                     32,tb_readback__DOT__nff0_wpr,
                     32,tb_readback__DOT__nff0);
    }
    if (VL_UNLIKELY((((0U != tb_readback__DOT__nff0_ign) 
                      | (0U != tb_readback__DOT__nff0_a0))))) {
        VL_WRITEF_NX("tb_readback: (relaxed) WantProcRef' low requires IgnoreProc = 0 and ASEL.0 = 0 (saw %0d, %0d)\n",0,
                     32,tb_readback__DOT__nff0_ign,
                     32,tb_readback__DOT__nff0_a0);
    }
    if (VL_UNLIKELY(((0U != tb_readback__DOT__nff0_bad)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) FFok'a high did not force FF.0mem'=0 and FF.1mem=1 (%0d violations)\n",0,
                     32,tb_readback__DOT__nff0_bad);
    }
    if (VL_UNLIKELY(((tb_readback__DOT__nff0_cr != tb_readback__DOT__nff0)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) WantCR did not fall on every running cycle (%0d of %0d)\n",0,
                     32,tb_readback__DOT__nff0_cr,32,
                     tb_readback__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_readback__DOT__nff0_alt != tb_readback__DOT__nff0)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) WantAltRef' did not enable the alternate decoder (%0d of %0d)\n",0,
                     32,tb_readback__DOT__nff0_alt,
                     32,tb_readback__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_readback__DOT__nff0_fl != tb_readback__DOT__nff0)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) Flush' did not assert on every running cycle (%0d of %0d)\n",0,
                     32,tb_readback__DOT__nff0_fl,32,
                     tb_readback__DOT__nff0);
    }
    if (VL_UNLIKELY(((0U != tb_readback__DOT__nff0_ffok)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) FFok' must be LOW for the FF field to reach the memory section (high on %0d)\n",0,
                     32,tb_readback__DOT__nff0_ffok);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nff0_st)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the Store never asserted -- nothing dirties a line\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nff0_hcd)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) HitColDirty never true -- the Store did not dirty the flushed line\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nff0_fsp)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) FSinPair' never fell -- the flush was not latched into the pair\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nff0_fs)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) FlushStore never asserted\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nff0_fm)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) ForceMiss never asserted\n",0);
    }
    if (VL_UNLIKELY((((0U == tb_readback__DOT__nff0_mia) 
                      | (0U == tb_readback__DOT__nff0_mib))))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the cache never missed (a %0d, b %0d)\n",0,
                     32,tb_readback__DOT__nff0_mia,
                     32,tb_readback__DOT__nff0_mib);
    }
    if (VL_UNLIKELY((VL_GTS_III(32, 4U, tb_readback__DOT__nmapst)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the map sequencer barely moved (%0d of 8 states)\n",0,
                     32,tb_readback__DOT__nmapst);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__npsm2)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) preStartMem' never asserted -- no storage cycle was started\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nsm2)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) StartMem' never asserted\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nwim)))) {
        VL_WRITEF_NX("tb_readback:   OPEN (task #17) -- WriteInMem' never asserted; see CLOCK EDGES below -- the victim is real, it just never coincides with a StartMap edge\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nrw)))) {
        VL_WRITEF_NX("tb_readback:   OPEN (task #17) -- ReadOrWriteInMap' never asserted; the MapTrouble diagnosis below says nothing\n",0);
    }
    VL_WRITEF_NX("tb_readback:   MapTrouble terms low on -- ReadOrWriteInMap' %0d, MapWP' %0d, MapDirty' %0d, MapEven' %0d, CheckWP' %0d of %0d\ntb_readback:   ...and the fourth term: preRfshInMem low on %0d of %0d\n",0,
                 32,tb_readback__DOT__nrw,32,tb_readback__DOT__nwp,
                 32,tb_readback__DOT__ndty,32,tb_readback__DOT__nevn,
                 32,tb_readback__DOT__nckw,32,tb_readback__DOT__nsamp,
                 32,tb_readback__DOT__nprf,32,tb_readback__DOT__nsamp);
    if (VL_UNLIKELY(((tb_readback__DOT__nthi != tb_readback__DOT__nsamp)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) THi must be a constant HIGH -- it strobes the whole map read path (high on %0d of %0d)\n",0,
                     32,tb_readback__DOT__nthi,32,tb_readback__DOT__nsamp);
    }
    if (VL_UNLIKELY((((0U == tb_readback__DOT__nwp) 
                      | (0U == tb_readback__DOT__ndty))))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the map outputs never varied -- the MC10124 translators are strobed off\n",0);
    }
    VL_WRITEF_NX("tb_readback:   MAP ARRAY d13 mem[0]=%x dout=%b | a04-write %0d, d13-WRITE %0d, read-cond %0d\ntb_readback:   MAP ARRAY douts -- a04=%b d11=%b d13=%b | MemX13.sil+13=%b MemX13.sil+3=%b | MapDirty'=%b MapWP'=%b\ntb_readback:   MAP ARRAY strobes -- RAS' edges %0d, CAS' edges %0d, READ-condition cycles %0d, WRITE-condition cycles %0d, of %0d\ntb_readback:   g14 OUTPUTS -- MapTrouble high on %0d, MapTrouble' high on %0d of %0d\n",0,
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__mem
                 [0U],1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout,
                 32,tb_readback__DOT__nmwr,32,tb_readback__DOT__nd13w,
                 32,tb_readback__DOT__nmrd,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_a04__DOT__dout),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__dout,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__dout,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapDirtya))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWP))),
                 32,tb_readback__DOT__nmras,32,tb_readback__DOT__nmcas,
                 32,tb_readback__DOT__nmrd,32,tb_readback__DOT__nmwr,
                 32,tb_readback__DOT__nsamp,32,tb_readback__DOT__nmt,
                 32,tb_readback__DOT__nmtp,32,tb_readback__DOT__nsamp);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x000001f4U, tb_readback__DOT__nmti)))) {
        VL_WRITEF_NX("tb_readback:   OPEN (task #17) -- MapTrouble cleared on only %0d; see the write-back note above\n",0,
                     32,tb_readback__DOT__nmti);
    }
    if (VL_UNLIKELY(((0U == tb_readback__DOT__nwm)))) {
        VL_WRITEF_NX("tb_readback:   OPEN (task #17) -- WriteInMem' and a clear map never coincided\n",0);
    }
    VL_WRITEF_NX("tb_readback:   MapFnc=00 ONSETS (%0d total):",0,
                 32,tb_readback__DOT__nmf00on);
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [0U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [0U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [1U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [1U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [2U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [2U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [3U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [3U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [4U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [4U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [5U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [5U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [6U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [6U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [7U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [7U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [8U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [8U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [9U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [9U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [0x0aU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [0x0aU]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__mf00at
                                 [0x0bU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__mf00at
                     [0x0bU]);
    }
    VL_WRITEF_NX("\ntb_readback:   MapState=3 ONSETS (%0d total):",0,
                 32,tb_readback__DOT__nms3on);
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [0U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [0U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [1U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [1U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [2U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [2U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [3U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [3U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [4U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [4U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [5U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [5U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [6U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [6U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [7U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [7U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [8U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [8U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [9U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [9U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [0x0aU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [0x0aU]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_readback__DOT__ms3at
                                 [0x0bU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_readback__DOT__ms3at
                     [0x0bU]);
    }
    VL_WRITEF_NX("\ntb_readback:   MapState counts (last sample):",0);
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [0U])))) {
        VL_WRITEF_NX(" 0=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [0U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [0U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [1U])))) {
        VL_WRITEF_NX(" 1=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [1U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [1U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [2U])))) {
        VL_WRITEF_NX(" 2=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [2U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [2U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [3U])))) {
        VL_WRITEF_NX(" 3=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [3U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [3U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [4U])))) {
        VL_WRITEF_NX(" 4=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [4U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [4U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [5U])))) {
        VL_WRITEF_NX(" 5=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [5U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [5U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [6U])))) {
        VL_WRITEF_NX(" 6=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [6U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [6U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__ms3_cnt
                      [7U])))) {
        VL_WRITEF_NX(" 7=%0d(@%0d)",0,32,vlSelfRef.tb_readback__DOT__ms3_cnt
                     [7U],32,vlSelfRef.tb_readback__DOT__ms3_last
                     [7U]);
    }
    VL_WRITEF_NX("\ntb_readback:   MemFree MEASURED per MemState:",0);
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [0U]))) {
        VL_WRITEF_NX("  state0: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [0U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [0U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [1U]))) {
        VL_WRITEF_NX("  state1: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [1U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [1U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [2U]))) {
        VL_WRITEF_NX("  state2: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [2U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [2U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [3U]))) {
        VL_WRITEF_NX("  state3: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [3U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [3U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [4U]))) {
        VL_WRITEF_NX("  state4: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [4U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [4U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [5U]))) {
        VL_WRITEF_NX("  state5: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [5U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [5U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [6U]))) {
        VL_WRITEF_NX("  state6: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [6U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [6U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__mf_seen
                     [7U]))) {
        VL_WRITEF_NX("  state7: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_readback__DOT__mf_at
                     [7U],1,vlSelfRef.tb_readback__DOT__x10_at
                     [7U]);
    }
    VL_WRITEF_NX("\ntb_readback:   RfshInMem high on %0d of the %0d in-window samples -- j13's TOP address bit\ntb_readback:   IN-WINDOW FEEDBACK -- MemFree edges %0d (high on %0d), MemIdle edges %0d, of %0d in-window samples\ntb_readback:   MapFnc {0',1'} counts -- 00=%0d 01=%0d 10=%0d 11=%0d | last 00 (function pending) @%0d\ntb_readback:   WINDOW OPENER -- preStartMem' HIGH on %0d (last @%0d), MapWait LOW on %0d (last @%0d), BOTH on %0d (last @%0d)\n",0,
                 32,tb_readback__DOT__nrfsh,32,tb_readback__DOT__nfree,
                 32,tb_readback__DOT__nmfree_e,32,tb_readback__DOT__nmfree_hi,
                 32,tb_readback__DOT__nmidle_e,32,tb_readback__DOT__nfree,
                 32,vlSelfRef.tb_readback__DOT__mf_cnt
                 [0U],32,vlSelfRef.tb_readback__DOT__mf_cnt
                 [1U],32,vlSelfRef.tb_readback__DOT__mf_cnt
                 [2U],32,vlSelfRef.tb_readback__DOT__mf_cnt
                 [3U],32,tb_readback__DOT__lastmf00,
                 32,tb_readback__DOT__npsh,32,tb_readback__DOT__lastpsh,
                 32,tb_readback__DOT__nmwlo,32,tb_readback__DOT__lastmwlo,
                 32,tb_readback__DOT__ncoin,32,tb_readback__DOT__lastcoin);
    VL_WRITEF_NX("tb_readback:   i14 ADDRESSES VISITED (Q2 = preStartMem' high ONLY at 3):",0);
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0U])))) {
        VL_WRITEF_NX(" 0=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [1U])))) {
        VL_WRITEF_NX(" 1=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [1U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [2U])))) {
        VL_WRITEF_NX(" 2=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [2U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [3U])))) {
        VL_WRITEF_NX(" 3=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [3U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [4U])))) {
        VL_WRITEF_NX(" 4=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [4U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [5U])))) {
        VL_WRITEF_NX(" 5=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [5U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [6U])))) {
        VL_WRITEF_NX(" 6=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [6U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [7U])))) {
        VL_WRITEF_NX(" 7=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [7U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [8U])))) {
        VL_WRITEF_NX(" 8=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [8U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [9U])))) {
        VL_WRITEF_NX(" 9=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [9U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0aU])))) {
        VL_WRITEF_NX(" 10=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0aU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0bU])))) {
        VL_WRITEF_NX(" 11=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0bU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0cU])))) {
        VL_WRITEF_NX(" 12=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0cU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0dU])))) {
        VL_WRITEF_NX(" 13=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0dU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0eU])))) {
        VL_WRITEF_NX(" 14=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0eU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x0fU])))) {
        VL_WRITEF_NX(" 15=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x0fU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x10U])))) {
        VL_WRITEF_NX(" 16=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x10U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x11U])))) {
        VL_WRITEF_NX(" 17=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x11U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x12U])))) {
        VL_WRITEF_NX(" 18=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x12U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x13U])))) {
        VL_WRITEF_NX(" 19=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x13U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x14U])))) {
        VL_WRITEF_NX(" 20=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x14U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x15U])))) {
        VL_WRITEF_NX(" 21=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x15U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x16U])))) {
        VL_WRITEF_NX(" 22=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x16U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x17U])))) {
        VL_WRITEF_NX(" 23=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x17U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x18U])))) {
        VL_WRITEF_NX(" 24=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x18U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x19U])))) {
        VL_WRITEF_NX(" 25=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x19U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1aU])))) {
        VL_WRITEF_NX(" 26=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1aU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1bU])))) {
        VL_WRITEF_NX(" 27=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1bU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1cU])))) {
        VL_WRITEF_NX(" 28=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1cU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1dU])))) {
        VL_WRITEF_NX(" 29=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1dU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1eU])))) {
        VL_WRITEF_NX(" 30=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1eU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__i14_hit
                      [0x1fU])))) {
        VL_WRITEF_NX(" 31=%0d",0,32,vlSelfRef.tb_readback__DOT__i14_hit
                     [0x1fU]);
    }
    VL_WRITEF_NX("\ntb_readback:   MemState reached %0d of 8 steps (group %b: 00=read/write 01=idle 10=refresh) | StartMem' HIGH (counter free) on %0d of %0d, reaching %0d values there; non-zero while HELD: %0d | %0d windows, LONGEST %0d sys_clk OPENING AT SAMPLE %0d | in-window: Clk0'Dd edges %0d, CE'(MemIdle) low %0d\ntb_readback:   DRAM TIMING PROM enables -- ChipsAre256/16K=1 ChipsAre64K=0 -> Use256/16KProm'=0 Use64KProm'=1 (CE' low = enabled)\n",0,
                 32,tb_readback__DOT__nmemst,2,(IData)(vlSelfRef.tb_readback__DOT__memst_grp),
                 32,tb_readback__DOT__nfree,32,tb_readback__DOT__nsamp,
                 32,tb_readback__DOT__nmemfr,32,tb_readback__DOT__nheld_nz,
                 32,tb_readback__DOT__nwin,32,tb_readback__DOT__maxrun,
                 32,tb_readback__DOT__winat,32,tb_readback__DOT__ndd,
                 32,tb_readback__DOT__nidle_lo);
    VL_WRITEF_NX("tb_readback:   D0 CONJUNCTION -- WriteInMem'&!MapTrouble on %0d, all three on %0d of %0d\ntb_readback:   MemWEa's D0 = ~(WriteInMem' | x10 | MapTroubleInMem) -- low on: WriteInMem' %0d, x10 %0d, MapTroubleInMem %0d of %0d%11d%11d\ntb_readback:   WRITE COUNTER over the run -- in LOAD (PE' low) on %0d, allowed to COUNT (CE' low) on %0d, D0 high on %0d of %0d\ntb_readback:   WRITE COUNTER (end sample) -- MemIdle(PE')=%b TrueBD(CE')=%b STPerr(MR)=%b | H0(MemWEa src)=%b H2(MakeMemCAS)=%b\n",0,
                 32,tb_readback__DOT__nwm,32,tb_readback__DOT__nall3,
                 32,tb_readback__DOT__nsamp,32,tb_readback__DOT__nwim,
                 32,tb_readback__DOT__nx10,32,tb_readback__DOT__nmti,
                 32,tb_readback__DOT__nwm,32,tb_readback__DOT__nall3,
                 32,tb_readback__DOT__nsamp,32,tb_readback__DOT__nload,
                 32,tb_readback__DOT__ncnt,32,tb_readback__DOT__nd0,
                 32,tb_readback__DOT__nsamp,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdle),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__TrueBD,
                 1,(1U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q)),
                 1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                           >> 1U)));
    VL_WRITEF_NX("tb_readback:   MAP SEQUENCER -- MapState took %0d of 8 values, MapFnc %0d of 4 | preStartMem' low on %0d, StartMem' low on %0d\ntb_readback:   STORE cycles with Store_ asserted: %0d   |   HitColDirty during the flush: %0d\ntb_readback:   FlushStore = ~(FSinPair' | EcHasAb) -- FSinPair'=0 on %0d, EcHasAb=0 on %0d of %0d\ntb_readback:   STORAGE PATH WHILE RUNNING -- FlushStore %0d, ForceMiss %0d, MISS(a) %0d, MISS(b) %0d of %0d\n",0,
                 32,tb_readback__DOT__nmapst,32,tb_readback__DOT__nmapfn,
                 32,tb_readback__DOT__npsm2,32,tb_readback__DOT__nsm2,
                 32,tb_readback__DOT__nff0_st,32,tb_readback__DOT__nff0_hcd,
                 32,tb_readback__DOT__nff0_fsp,32,tb_readback__DOT__nff0_ech,
                 32,tb_readback__DOT__nff0,32,tb_readback__DOT__nff0_fs,
                 32,tb_readback__DOT__nff0_fm,32,tb_readback__DOT__nff0_mia,
                 32,tb_readback__DOT__nff0_mib,32,tb_readback__DOT__nff0);
    VL_WRITEF_NX("tb_readback:   STORAGE PATH (end sample) -- FlushStore=%b ForceMiss=%b | Hit'a=%b Hit'b=%b\ntb_readback:   FFok'a (ProcH d24/d23 qualifier) high on %0d of %0d -- high FORCES FF.0mem/FF.1mem high\ntb_readback:   ...and WantProcRef' = IgnoreProc | ASEL.0 -- IgnoreProc=1 on %0d, ASEL.0=1 on %0d of %0d\ntb_readback:   WantCR=%b terms -- ASEL.1'=%b FF.0mem'=%b WantProcRef'=%b\ntb_readback:   alt-ref path EDGES -- WantCR %0d, WantAltRef' %0d, Flush_' %0d, Map_' %0d\n",0,
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__FlushStore,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ForceMiss),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))),
                 32,tb_readback__DOT__nff0_ffok,32,
                 tb_readback__DOT__nff0,32,tb_readback__DOT__nff0_ign,
                 32,tb_readback__DOT__nff0_a0,32,tb_readback__DOT__nff0,
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceRd),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_),
                 32,tb_readback__DOT__nwcr,32,tb_readback__DOT__nwar,
                 32,tb_readback__DOT__nfl,32,tb_readback__DOT__nmp);
    VL_WRITEF_NX("tb_readback:   kind decoder -- EmuOrFT'=%b ASEL.2=%b FF.1mem=%b | WantAltRef'=%b HoldOrIP=%b | Flush_'=%b Map_'=%b\ntb_readback:   A slot -- CacheRefInA=%b IfuRefInA=%b Store_InA=%b PrefetchInA=%b IoFetchInA=%b PairHasA=%b\ntb_readback:   VICTIM CHAIN over %0d in-window samples -- WantVic %0d, DirtyVicOrAB %0d, ForceDirtyMiss %0d, FlushInA %0d\ntb_readback:                  -> VicInPair' asserted %0d, VictimInA %0d, IoStoreInA %0d, WriteInA' asserted %0d\n",0,
                 1,vlSelfRef.tb_readback__DOT__m__DOT__EmuOrFT_p___05F_MemX,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__ASEL_2),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceRd,
                 1,(1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                          | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__HoldOrIP),
                 1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                           >> 1U)),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__IfuRefInA_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__Store_u_InA_p___05F_MemC))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PrefetchInA),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__IoFetchInA_p___05F_MemC))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))),
                 32,tb_readback__DOT__nff0,32,tb_readback__DOT__nvc_wv,
                 32,tb_readback__DOT__nvc_dv,32,tb_readback__DOT__nvc_fdm,
                 32,tb_readback__DOT__nvc_fia,32,tb_readback__DOT__nvc_vip,
                 32,tb_readback__DOT__nvc_via,32,tb_readback__DOT__nvc_ios,
                 32,tb_readback__DOT__nvc_wia);
    VL_WRITEF_NX("tb_readback:                  -> PIPELINE: WriteInMap' asserted %0d, WriteInMem' asserted %0d | StartMapClk0'a high %0d, StartMemClk0' high %0d\ntb_readback:                  -> CLOCK EDGES: StartMapClk0'a %0d edges (with WriteInA' asserted: %0d), StartMemClk0' %0d edges (with WriteInMap' asserted: %0d)\ntb_readback:   AwantsMapFS=%b terms -- EcHasAb=%b Map_InPair'=%b VicInPair'=%b\n",0,
                 32,tb_readback__DOT__nvc_wim,32,tb_readback__DOT__nvc_wimem,
                 32,tb_readback__DOT__nvc_smc,32,tb_readback__DOT__nvc_sec,
                 32,tb_readback__DOT__nvc_smc_e,32,
                 tb_readback__DOT__nvc_coin,32,tb_readback__DOT__nvc_sec_e,
                 32,tb_readback__DOT__nvc_coin2,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__AwantsMapFS),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__EcHasAb,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__Map_u_InPair_p___05F_MemC),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__VicInPair_p___05F_MemC);
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_readback__DOT__nras)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) MemRASa never strobed -- no DRAM cycle started\n",0);
    }
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_readback__DOT__ncas)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) MemCASa never strobed -- the cycle did not get past RAS\n",0);
    }
    VL_WRITEF_NX("tb_readback:   DRAM CYCLE RUNS -- RAS %0d, CAS %0d edges\n",0,
                 32,tb_readback__DOT__nras,32,tb_readback__DOT__ncas);
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_readback__DOT__nms)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) MapState never advanced -- the PROM sequencer is not running\n",0);
    }
    VL_WRITEF_NX("tb_readback:   MemIdlea=%b MemX clk0' edges=%0d  MemRfsh=%b RfshPeriod=%b SetRunRfsh=1\ntb_readback:   RfshPeriod edges=%0d  MemRfsh edges=%0d\ntb_readback:   StartMap' terms -- Hia=1 NoRef=%b MapRfsh'=%b AwantsMapFS'=%b AfreeOrEc'a=%b WantVic'=%b\ntb_readback:   MapRfsh' = MapFree'(%b) | NeedRfsh'(%b) = %b\ntb_readback:   refresh chain EDGES -- RfshPeriod %0d, RfshSqWave %0d, StartRfshCycle' %0d,\ntb_readback:                          WantRfsh' %0d, NeedRfsh' %0d, MapRfsh' %0d, StartMap' %0d\n",0,
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemIdlea,
                 32,tb_readback__DOT__nmx,1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                   >> 3U)),
                 1,(IData)(vlSelfRef.tb_readback__DOT__rfshper),
                 32,tb_readback__DOT__nrp,32,tb_readback__DOT__nmr,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__NoRef),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__MapRfsh_p___05F_MemX,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantVic_p_),
                 1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                          >> 1U)),1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__MapRfsh_p___05F_MemX,
                 32,tb_readback__DOT__nrp,32,tb_readback__DOT__nsq,
                 32,tb_readback__DOT__nsrc,32,tb_readback__DOT__nwr,
                 32,tb_readback__DOT__nnr,32,tb_readback__DOT__nmrf,
                 32,tb_readback__DOT__nsm);
    VL_WRITEF_NX("tb_readback:   MapWait edges %0d (CE' of the MapState counter -- it must go LOW to advance)\ntb_readback:   MapWait terms -- StartMap'=%b MapFree=%b preStartMem'=%b WantMapWait'=%b (edges: pSM %0d, WMW %0d)\ntb_readback:   WantMapWait' terms -- MapFnc.0'=%b MapFnc.1'=%b DisHold(MemX)=%b i20=%b l17=%b\ntb_readback:   g13 stub=%b (edges %0d)  h13 stub=%b  | MemX StartMap'=%b (edges %0d)\ntb_readback:   NeedRfsh' inputs -- WantRfsh'=%b RfshSqWave=%b PairFull=%b (sqwave edges=%0d)\n",0,
                 32,tb_readback__DOT__nmw,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFree,
                 1,(1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                          >> 5U)),1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_),
                 32,tb_readback__DOT__npsm,32,tb_readback__DOT__nwmw,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__DisHold___05FMemC),
                 1,((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p4) 
                      | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_) 
                         | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_))) 
                     & (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_) 
                         | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                            | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_))) 
                        & (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                            | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_) 
                               | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                           & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__VicInPair_p___05F_MemC) 
                              | ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__MapWait_m_D___05FMemX)) 
                                 | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p14)))))) 
                    & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__DisHold___05FMemC),
                 1,((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapFree) 
                    & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC)),
                 32,tb_readback__DOT__ng13,1,(1U & 
                                              (~ (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                                   >> 5U) 
                                                  | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_)))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC),
                 32,tb_readback__DOT__nxsm,1,(1U & 
                                              (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RfshSqWave),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__PairFull,
                 32,tb_readback__DOT__nsq);
    VL_WRITEF_NX("tb_readback:   MapState=%b%b%b  StartMap'=%b MapWait=%b | MapState changes=%0d\ntb_readback:   MemAd=%b%b%b%b%b Sout=%x  (RASa=%b CASa=%b WEa=%b)\ntb_readback: the Pipe pointer moved %0d times over the run, ending at %0#\ntb_readback: holds -- PrHoldReq=%b CHoldReq=%b ExtHoldReq=%b PRhold=%b\ntb_readback: which hold -- RefHold'=%b MDhold'=%b MiscHold'=%b (0 = that one is holding)\n",0,
                 1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                          >> 2U)),1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                           >> 1U)),
                 1,(1U & (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q)),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__StartMap_p___05F_MemC),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MapWait,
                 32,tb_readback__DOT__nms,1,(1U & (~ 
                                                   ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d23__DOT__p7) 
                                                    | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                                        ? (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_g18__DOT__q)
                                                        : (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RP_02))))),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d24__DOT__p7)) 
                          & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                 ? (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_g19__DOT__q)
                                 : (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RP_04))))),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_d24__DOT__p7)) 
                          & (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                 ? ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_g19__DOT__q) 
                                    >> 1U) : (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__RP_06))))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__MemAd_3),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__MemAd_4,
                 16,(((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qb) 
                          << 3U) | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qa) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__qb) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__qa))) 
                       << 0x0000000cU) | (((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__qb) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__qa) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__qb) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__qa))) 
                                          << 8U)) | 
                     ((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__qb) 
                          << 3U) | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__qa) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__qb) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__qa))) 
                       << 4U) | ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__qb) 
                                   << 3U) | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__qa) 
                                             << 2U)) 
                                 | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__qb) 
                                     << 1U) | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__qa))))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__MemCASa___05FMemX,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__MemWEa___05FMemX),
                 32,tb_readback__DOT__npipe,4,(IData)(vlSelfRef.tb_readback__DOT__pipead),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_11)) 
                          | ((~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_10)) 
                             | (~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__SimHoldDis) 
                                   | ((~ (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__q) 
                                           >> 3U) | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_a01__DOT__p7))) 
                                      | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcL__DOT__u_h20__DOT__p13))))))),
                 1,(1U & ((~ ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p9) 
                              | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p10) 
                                 | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p11) 
                                    | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__WantIfuHold_p_) 
                                       | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a)))))) 
                          | (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK)))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__ExtHoldReq),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__PRhold,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb,
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa))));
    if (VL_UNLIKELY((vlSelfRef.tb_readback__DOT__m__DOT__PRhold))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the memory is still holding the processor after a run\n",0);
    }
    VL_WRITEF_NX("tb_readback: machine running -- %0d clk0' edges, Stop=%b\n",0,
                 32,tb_readback__DOT__n0a,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa));
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000064U, tb_readback__DOT__n0a)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the microinstruction clock is not free-running\n",0);
    }
    tb_readback__DOT__tbad = 0U;
    VL_WRITEF_NX("tb_readback: MemC clock edges %0d | MemClkEnable'a=%b CLKEnable'b=%b dStop=%b Stop=%b\n",0,
                 32,tb_readback__DOT__nmemclk,1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__MemClkEnable_p_a___05FContA),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__CLKEnable_p_b___05FContA,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__dStop),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000032U, tb_readback__DOT__nmemclk)))) {
        VL_WRITEF_NX("tb_readback: FAIL -- MemC's local clock is not running\n",0);
        tb_readback__DOT__tbad = ((IData)(1U) + tb_readback__DOT__tbad);
    }
    if (VL_UNLIKELY(((VL_LTS_III(32, tb_readback__DOT__nmemclk, tb_readback__DOT__n0a) 
                      | VL_GTS_III(32, tb_readback__DOT__nmemclk, 
                                   VL_MULS_III(32, (IData)(2U), tb_readback__DOT__n0a)))))) {
        VL_WRITEF_NX("tb_readback: FAIL -- MemC clock %0d edges against the processor's %0d\n",0,
                     32,tb_readback__DOT__nmemclk,32,
                     tb_readback__DOT__n0a);
        tb_readback__DOT__tbad = ((IData)(1U) + tb_readback__DOT__tbad);
    }
    VL_WRITEF_NX("tb_readback: MAR=%x\ntb_readback: PipeAd=%0# | PipeVA[16:19]=%b%b%b%b dVA[04:07]=%b%b%b%b\n",0,
                 16,(0x0000ffffU & (~ (((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_00_p_) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_01_p_) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_02_p_) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_03_p_))) 
                                         << 0x0000000cU) 
                                        | (((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_04_p_) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_05_p_) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_06_p_) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_07_p_))) 
                                           << 8U)) 
                                       | ((((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_08_p_) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_09_p_) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_10_p_) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_11_p_))) 
                                           << 4U) | 
                                          ((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_12_p_) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_13_p_) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_14_p_) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_readback__DOT__m__DOT__MAR_15_p_))))))),
                 4,((((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                             << 1U)) | (1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                              >> 1U))) 
                     << 2U) | ((2U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                      >> 1U)) | (1U 
                                                 & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                                    >> 3U)))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PipeVA_16),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PipeVA_17,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PipeVA_18),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__PipeVA_19,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__dVA_04),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__dVA_05,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__dVA_06),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__dVA_07);
    tb_readback__DOT__kk = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[0] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0U]);
    }
    tb_readback__DOT__kk = 1U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [1U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[1] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [1U]);
    }
    tb_readback__DOT__kk = 2U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [2U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[2] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [2U]);
    }
    tb_readback__DOT__kk = 3U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [3U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[3] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [3U]);
    }
    tb_readback__DOT__kk = 4U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [4U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[4] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [4U]);
    }
    tb_readback__DOT__kk = 5U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [5U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[5] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [5U]);
    }
    tb_readback__DOT__kk = 6U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [6U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[6] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [6U]);
    }
    tb_readback__DOT__kk = 7U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [7U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[7] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [7U]);
    }
    tb_readback__DOT__kk = 8U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [8U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[8] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [8U]);
    }
    tb_readback__DOT__kk = 9U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [9U])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[9] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [9U]);
    }
    tb_readback__DOT__kk = 0x0000000aU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0aU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[10] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0aU]);
    }
    tb_readback__DOT__kk = 0x0000000bU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0bU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[11] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0bU]);
    }
    tb_readback__DOT__kk = 0x0000000cU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0cU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[12] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0cU]);
    }
    tb_readback__DOT__kk = 0x0000000dU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0dU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[13] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0dU]);
    }
    tb_readback__DOT__kk = 0x0000000eU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0eU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[14] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0eU]);
    }
    tb_readback__DOT__kk = 0x0000000fU;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0fU])))) {
        VL_WRITEF_NX("tb_readback:   Pipe[15] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0fU]);
    }
    tb_readback__DOT__kk = 0x00000010U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    VL_WRITEF_NX("tb_readback: ASEL=%0# WantProcRef'=%b | Dbusy=%b WantCR=%b CacheRefInA'=%b IgnoreProc=%b\n",0,
                 3,vlSelfRef.tb_readback__DOT__asel,
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__Dbusy,
                 1,(1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantCR_p_))),
                 1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_),
                 1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__IgnoreProc);
    tb_readback__DOT__kk = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_readback__DOT__kk);
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 1U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 2U)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    while (VL_GTS_III(32, 0x00000010U, tb_readback__DOT__kk)) {
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceEn = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceVal = 0U;
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd = 0U;
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceEn = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 3U)));
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceRd 
            = (1U & (~ (tb_readback__DOT__kk >> 3U)));
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceEn = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 2U)));
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceRd 
            = (1U & (~ (tb_readback__DOT__kk >> 2U)));
        vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceEn = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 1U)));
        vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceRd 
            = (1U & (~ (tb_readback__DOT__kk >> 1U)));
        vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceEn = 1U;
        vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
            = (1U & tb_readback__DOT__kk);
        vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceRd 
            = (1U & tb_readback__DOT__kk);
        __Vfunc_tb_readback__DOT__WT__1919__n = 0x0000003cU;
        __Vfunc_tb_readback__DOT__WT__1919__Vfuncout 
            = VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1919__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1919__Vfuncout)) {
            __Vfunc_tb_readback__DOT__WT__1919__Vfuncout = 1U;
        }
        tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28 
            = __Vfunc_tb_readback__DOT__WT__1919__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28)) {
            co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_readback.sys_clk)", 
                                                                 "verilog/verilator/tb_readback.sv", 
                                                                 3175);
            tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28 
                = (tb_readback__DOT__unnamedblk1_29__DOT____Vrepeat28 
                   - (IData)(1U));
        }
        if (VL_UNLIKELY((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__Lfetch_u_) 
                          != (IData)((2U == (0x0000000fU 
                                             & tb_readback__DOT__kk))))))) {
            VL_WRITEF_NX("tb_readback: FAIL -- LFetch<- at ASEL=%0# ff01=%0# reads %b\n",0,
                         2,(3U & (tb_readback__DOT__kk 
                                  >> 2U)),2,(3U & tb_readback__DOT__kk),
                         1,(IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__Lfetch_u_));
            tb_readback__DOT__tbad = ((IData)(1U) + tb_readback__DOT__tbad);
        }
        if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                                 >> 4U)) != (IData)(
                                                    (6U 
                                                     == 
                                                     (0x0000000fU 
                                                      & tb_readback__DOT__kk))))))) {
            VL_WRITEF_NX("tb_readback: FAIL -- IFetch<- at ASEL=%0# ff01=%0# reads %b\n",0,
                         2,(3U & (tb_readback__DOT__kk 
                                  >> 2U)),2,(3U & tb_readback__DOT__kk),
                         1,(1U & ((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                                  >> 4U)));
            tb_readback__DOT__tbad = ((IData)(1U) + tb_readback__DOT__tbad);
        }
        tb_readback__DOT__kk = ((IData)(1U) + tb_readback__DOT__kk);
        vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceVal 
            = (1U & tb_readback__DOT__kk);
        vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 1U)));
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 2U)));
        vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceVal 
            = (1U & (~ (tb_readback__DOT__kk >> 3U)));
    }
    VL_WRITEF_NX("tb_readback: LFetch<- at (ASEL 0, ff01 2) and IFetch<- at (ASEL 1, ff01 2), and nowhere else\n",0);
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceRd 
        = vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb;
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_0__VforceEn = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_1_p___VforceEn = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb)));
    vlSelfRef.tb_readback__DOT__m__DOT__ASEL_2_p___VforceEn = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_ProcH__DOT__FF_0mem)));
    vlSelfRef.tb_readback__DOT__m__DOT__FF_0mem_p___VforceEn = 0U;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceRd 
        = vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem;
    vlSelfRef.tb_readback__DOT__m__DOT__FF_1mem__VforceEn = 0U;
    __Vfunc_tb_readback__DOT__WT__1920__n = 0x0000003cU;
    __Vfunc_tb_readback__DOT__WT__1920__Vfuncout = 
        VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_readback__DOT__WT__1920__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_readback__DOT__WT__1920__Vfuncout)) {
        __Vfunc_tb_readback__DOT__WT__1920__Vfuncout = 1U;
    }
    tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29 
        = __Vfunc_tb_readback__DOT__WT__1920__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29)) {
        co_await vlSelfRef.__VtrigSched_h531d4bfc__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_readback.sys_clk)", 
                                                             "verilog/verilator/tb_readback.sv", 
                                                             3192);
        tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29 
            = (tb_readback__DOT__unnamedblk1_30__DOT____Vrepeat29 
               - (IData)(1U));
    }
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                      != (3U < (IData)(vlSelfRef.tb_readback__DOT__asel)))))) {
        VL_WRITEF_NX("tb_readback: FAIL -- WantProcRef'=%b for ASEL=%0#\n",0,
                     1,vlSelfRef.tb_readback__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_,
                     3,(IData)(vlSelfRef.tb_readback__DOT__asel));
        tb_readback__DOT__tbad = ((IData)(1U) + tb_readback__DOT__tbad);
    }
    if (VL_UNLIKELY(((0U != tb_readback__DOT__tbad)))) {
        VL_WRITEF_NX("tb_readback: (relaxed) the memory section is not right in a running machine\n",0);
    }
    VL_WRITEF_NX("tb_readback: READ PATH -- '166 load edges %0d | q at load = %b (want 10101100) | QH=%b | Sin.00 high on %0d, SinD.00 high on %0d\n",0,
                 32,vlSelfRef.tb_readback__DOT__n_load_edge_rb,
                 8,(IData)(vlSelfRef.tb_readback__DOT__q_at_load),
                 1,vlSelfRef.tb_readback__DOT__qh_at_load,
                 32,vlSelfRef.tb_readback__DOT__n_sin_hi,
                 32,vlSelfRef.tb_readback__DOT__n_sind_hi);
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_readback__DOT__n_load_edge_rb)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3208: Assertion failed in %Ntb_readback: the '166s were never loaded -- no read reached the storage array\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3208, "", false);
    }
    if (VL_UNLIKELY(((0xacU != (IData)(vlSelfRef.tb_readback__DOT__q_at_load))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3210: Assertion failed in %Ntb_readback: the array did not return the seeded word (got %b, want 10101100)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     8,(IData)(vlSelfRef.tb_readback__DOT__q_at_load));
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3210, "", false);
    }
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.tb_readback__DOT__qh_at_load)))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3213: Assertion failed in %Ntb_readback: QH is not the H stage at load (QH=%b, H=1)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_readback__DOT__qh_at_load));
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3213, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_readback__DOT__n_sin_hi)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3215: Assertion failed in %Ntb_readback: Sin.00 never went high -- the word never left the storage board\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3215, "", false);
    }
    VL_WRITEF_NX("tb_readback: RETURN PATH -- D.00 high %0d (edges %0d) | MD_D high %0d | dMD.00 high %0d (edges %0d) | Md.00 high %0d (edges %0d)\ntb_readback:   ERRORS -- MemError high %0d, ECFault high %0d\ntb_readback:   h05 gate -- D.00 & MD_D coincide on %0d samples; h05's own output stub high on %0d\ntb_readback:   cache fill -- a03 WE' low on %0d, CE' low on %0d, D0in.00 high on %0d\ntb_readback:   THE FILL -- a03 WE' falling edges %0d, of which D0in.00 == SinD.00: %0d | SinD.00 high on %0d\n",0,
                 32,vlSelfRef.tb_readback__DOT__n_d00,
                 32,vlSelfRef.tb_readback__DOT__n_d00_e,
                 32,vlSelfRef.tb_readback__DOT__n_mdd,
                 32,vlSelfRef.tb_readback__DOT__n_dmd,
                 32,vlSelfRef.tb_readback__DOT__n_dmd_e,
                 32,vlSelfRef.tb_readback__DOT__n_md,
                 32,vlSelfRef.tb_readback__DOT__n_md_e,
                 32,vlSelfRef.tb_readback__DOT__n_merr,
                 32,vlSelfRef.tb_readback__DOT__n_ecf,
                 32,vlSelfRef.tb_readback__DOT__n_coin_dmd,
                 32,vlSelfRef.tb_readback__DOT__n_h05out,
                 32,vlSelfRef.tb_readback__DOT__n_cwe,
                 32,vlSelfRef.tb_readback__DOT__n_cce,
                 32,vlSelfRef.tb_readback__DOT__n_d0in,
                 32,vlSelfRef.tb_readback__DOT__n_we_fall,
                 32,vlSelfRef.tb_readback__DOT__n_we_match,
                 32,vlSelfRef.tb_readback__DOT__n_sind1);
    VL_WRITEF_NX("tb_readback:   FILL ADDRESS -- last write %b, last write CARRYING A ONE %b (%0d such), last read %b\ntb_readback:   RETURN WORD (cache seeded 001100101101001101) -- dMD=%b matched on %0d | Md=%b matched on %0d\ntb_readback:   ...16-bit data word alone -- dMD matched on %0d, Md matched on %0d of %0d MD_D samples\n",0,
                 12,vlSelfRef.tb_readback__DOT__dad_at_write,
                 12,(IData)(vlSelfRef.tb_readback__DOT__dad_ones),
                 32,vlSelfRef.tb_readback__DOT__n_we_ones,
                 12,(IData)(vlSelfRef.tb_readback__DOT__dad_at_read),
                 18,vlSelfRef.tb_readback__DOT__dmd_cap,
                 32,vlSelfRef.tb_readback__DOT__n_dmd_ok,
                 18,vlSelfRef.tb_readback__DOT__md_cap,
                 32,vlSelfRef.tb_readback__DOT__n_md_ok,
                 32,vlSelfRef.tb_readback__DOT__n_dmd16,
                 32,vlSelfRef.tb_readback__DOT__n_md16,
                 32,vlSelfRef.tb_readback__DOT__n_mdd);
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_readback__DOT__n_mdd)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3239: Assertion failed in %Ntb_readback: MD_D never asserted -- the cache was never selected as the MD source\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3239, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_readback__DOT__n_dmd16)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3241: Assertion failed in %Ntb_readback: the seeded cache word never reached dMD (last dMD=%b, want 001100101101001101)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     18,vlSelfRef.tb_readback__DOT__dmd_cap);
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3241, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_readback__DOT__n_md16)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_readback.sv:3244: Assertion failed in %Ntb_readback: the seeded cache word never reached Md on the processor (last Md=%b, want 001100101101001101)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     18,vlSelfRef.tb_readback__DOT__md_cap);
        VL_STOP_MT("verilog/verilator/tb_readback.sv", 3244, "", false);
    }
    VL_WRITEF_NX("tb_readback: PASS -- A WORD COMES OUT OF PARC'S STORAGE ARRAY:\ntb_readback:   real microcode runs, the memory section sequences a DRAM cycle,\ntb_readback:   the MK4096s are parallel-loaded into the SN74166s in the part's\ntb_readback:   own stage order, and the data reaches Sin on the backplane.\ntb_readback:   AND THE RETURN PATH CARRIES A WORD TO THE PROCESSOR: a seeded\ntb_readback:   cache word arrives intact on dMD and on Md, which is what\ntb_readback:   microcode reads and what cpu.c models.\n",0);
    VL_FINISH_MT("verilog/verilator/tb_readback.sv", 3254, "");
    co_return;}
