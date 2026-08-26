// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_disk__DOT__n_sectw;
    tb_disk__DOT__n_sectw = 0;
    IData/*31:0*/ tb_disk__DOT__n_idxtw;
    tb_disk__DOT__n_idxtw = 0;
    IData/*31:0*/ tb_disk__DOT__n_secpulse;
    tb_disk__DOT__n_secpulse = 0;
    IData/*31:0*/ tb_disk__DOT__n_secgap;
    tb_disk__DOT__n_secgap = 0;
    CData/*3:0*/ tb_disk__DOT__fraddr_before;
    tb_disk__DOT__fraddr_before = 0;
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
    IData/*31:0*/ tb_disk__DOT__unnamedblk13__DOT__zi;
    tb_disk__DOT__unnamedblk13__DOT__zi = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk14__DOT__mi2;
    tb_disk__DOT__unnamedblk14__DOT__mi2 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32;
    tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33;
    tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34;
    tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35;
    tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36;
    tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37;
    tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38;
    tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39;
    tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40;
    tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41;
    tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42;
    tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43;
    tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44;
    tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45;
    tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46;
    tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47;
    tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48;
    tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5291__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5291__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5291__n;
    __Vfunc_tb_disk__DOT__WT__5291__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5292__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5292__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5292__n;
    __Vfunc_tb_disk__DOT__WT__5292__n = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5293__Vfuncout;
    __Vfunc_tb_disk__DOT__revn__5293__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5293__n;
    __Vfunc_tb_disk__DOT__revn__5293__n = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5294__Vfuncout;
    __Vfunc_tb_disk__DOT__revn__5294__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5294__n;
    __Vfunc_tb_disk__DOT__revn__5294__n = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5295__Vfuncout;
    __Vfunc_tb_disk__DOT__revn__5295__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__revn__5295__n;
    __Vfunc_tb_disk__DOT__revn__5295__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__5296__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__5296__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5297__fn;
    __Vtask_tb_disk__DOT__strobe__5297__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5297__data;
    __Vtask_tb_disk__DOT__strobe__5297__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5297__ss;
    __Vtask_tb_disk__DOT__strobe__5297__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5298__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5298__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5298__n;
    __Vfunc_tb_disk__DOT__WT__5298__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5299__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5299__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5299__n;
    __Vfunc_tb_disk__DOT__WT__5299__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5300__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5300__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5300__n;
    __Vfunc_tb_disk__DOT__WT__5300__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5301__fn;
    __Vtask_tb_disk__DOT__strobe__5301__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5301__data;
    __Vtask_tb_disk__DOT__strobe__5301__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5301__ss;
    __Vtask_tb_disk__DOT__strobe__5301__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5302__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5302__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5302__n;
    __Vfunc_tb_disk__DOT__WT__5302__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5303__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5303__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5303__n;
    __Vfunc_tb_disk__DOT__WT__5303__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5304__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5304__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5304__n;
    __Vfunc_tb_disk__DOT__WT__5304__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__b0;
    __Vtask_tb_disk__DOT__parc_micro__5305__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__b1;
    __Vtask_tb_disk__DOT__parc_micro__5305__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__b2;
    __Vtask_tb_disk__DOT__parc_micro__5305__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__b3;
    __Vtask_tb_disk__DOT__parc_micro__5305__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__b4;
    __Vtask_tb_disk__DOT__parc_micro__5305__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5306__fn;
    __Vtask_tb_disk__DOT__strobe__5306__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5306__data;
    __Vtask_tb_disk__DOT__strobe__5306__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5306__ss;
    __Vtask_tb_disk__DOT__strobe__5306__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5307__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5307__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5307__n;
    __Vfunc_tb_disk__DOT__WT__5307__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5308__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5308__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5308__n;
    __Vfunc_tb_disk__DOT__WT__5308__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5309__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5309__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5309__n;
    __Vfunc_tb_disk__DOT__WT__5309__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5310__fn;
    __Vtask_tb_disk__DOT__strobe__5310__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5310__data;
    __Vtask_tb_disk__DOT__strobe__5310__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5310__ss;
    __Vtask_tb_disk__DOT__strobe__5310__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5311__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5311__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5311__n;
    __Vfunc_tb_disk__DOT__WT__5311__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5312__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5312__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5312__n;
    __Vfunc_tb_disk__DOT__WT__5312__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5313__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5313__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5313__n;
    __Vfunc_tb_disk__DOT__WT__5313__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5314__fn;
    __Vtask_tb_disk__DOT__strobe__5314__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5314__data;
    __Vtask_tb_disk__DOT__strobe__5314__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5314__ss;
    __Vtask_tb_disk__DOT__strobe__5314__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5315__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5315__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5315__n;
    __Vfunc_tb_disk__DOT__WT__5315__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5316__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5316__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5316__n;
    __Vfunc_tb_disk__DOT__WT__5316__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5317__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5317__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5317__n;
    __Vfunc_tb_disk__DOT__WT__5317__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5318__fn;
    __Vtask_tb_disk__DOT__strobe__5318__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5318__data;
    __Vtask_tb_disk__DOT__strobe__5318__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5318__ss;
    __Vtask_tb_disk__DOT__strobe__5318__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5319__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5319__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5319__n;
    __Vfunc_tb_disk__DOT__WT__5319__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5320__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5320__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5320__n;
    __Vfunc_tb_disk__DOT__WT__5320__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5321__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5321__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5321__n;
    __Vfunc_tb_disk__DOT__WT__5321__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5322__fn;
    __Vtask_tb_disk__DOT__strobe__5322__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5322__data;
    __Vtask_tb_disk__DOT__strobe__5322__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5322__ss;
    __Vtask_tb_disk__DOT__strobe__5322__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5323__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5323__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5323__n;
    __Vfunc_tb_disk__DOT__WT__5323__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5324__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5324__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5324__n;
    __Vfunc_tb_disk__DOT__WT__5324__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5325__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5325__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5325__n;
    __Vfunc_tb_disk__DOT__WT__5325__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5326__fn;
    __Vtask_tb_disk__DOT__strobe__5326__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5326__data;
    __Vtask_tb_disk__DOT__strobe__5326__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5326__ss;
    __Vtask_tb_disk__DOT__strobe__5326__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5327__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5327__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5327__n;
    __Vfunc_tb_disk__DOT__WT__5327__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5328__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5328__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5328__n;
    __Vfunc_tb_disk__DOT__WT__5328__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5329__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5329__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5329__n;
    __Vfunc_tb_disk__DOT__WT__5329__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5330__fn;
    __Vtask_tb_disk__DOT__strobe__5330__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5330__data;
    __Vtask_tb_disk__DOT__strobe__5330__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5330__ss;
    __Vtask_tb_disk__DOT__strobe__5330__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5331__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5331__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5331__n;
    __Vfunc_tb_disk__DOT__WT__5331__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5332__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5332__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5332__n;
    __Vfunc_tb_disk__DOT__WT__5332__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5333__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5333__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5333__n;
    __Vfunc_tb_disk__DOT__WT__5333__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5335__fn;
    __Vtask_tb_disk__DOT__strobe__5335__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5335__data;
    __Vtask_tb_disk__DOT__strobe__5335__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5335__ss;
    __Vtask_tb_disk__DOT__strobe__5335__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5336__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5336__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5336__n;
    __Vfunc_tb_disk__DOT__WT__5336__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5337__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5337__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5337__n;
    __Vfunc_tb_disk__DOT__WT__5337__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5338__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5338__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5338__n;
    __Vfunc_tb_disk__DOT__WT__5338__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5339__fn;
    __Vtask_tb_disk__DOT__strobe__5339__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5339__data;
    __Vtask_tb_disk__DOT__strobe__5339__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5339__ss;
    __Vtask_tb_disk__DOT__strobe__5339__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5340__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5340__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5340__n;
    __Vfunc_tb_disk__DOT__WT__5340__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5341__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5341__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5341__n;
    __Vfunc_tb_disk__DOT__WT__5341__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5342__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5342__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5342__n;
    __Vfunc_tb_disk__DOT__WT__5342__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5343__fn;
    __Vtask_tb_disk__DOT__strobe__5343__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5343__data;
    __Vtask_tb_disk__DOT__strobe__5343__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5343__ss;
    __Vtask_tb_disk__DOT__strobe__5343__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5344__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5344__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5344__n;
    __Vfunc_tb_disk__DOT__WT__5344__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5345__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5345__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5345__n;
    __Vfunc_tb_disk__DOT__WT__5345__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5346__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5346__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5346__n;
    __Vfunc_tb_disk__DOT__WT__5346__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5347__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5347__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5347__n;
    __Vfunc_tb_disk__DOT__WT__5347__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__b0;
    __Vtask_tb_disk__DOT__parc_micro__5349__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__b1;
    __Vtask_tb_disk__DOT__parc_micro__5349__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__b2;
    __Vtask_tb_disk__DOT__parc_micro__5349__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__b3;
    __Vtask_tb_disk__DOT__parc_micro__5349__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__b4;
    __Vtask_tb_disk__DOT__parc_micro__5349__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5350__fn;
    __Vtask_tb_disk__DOT__strobe__5350__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5350__data;
    __Vtask_tb_disk__DOT__strobe__5350__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5350__ss;
    __Vtask_tb_disk__DOT__strobe__5350__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5351__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5351__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5351__n;
    __Vfunc_tb_disk__DOT__WT__5351__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5352__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5352__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5352__n;
    __Vfunc_tb_disk__DOT__WT__5352__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5353__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5353__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5353__n;
    __Vfunc_tb_disk__DOT__WT__5353__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5354__fn;
    __Vtask_tb_disk__DOT__strobe__5354__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5354__data;
    __Vtask_tb_disk__DOT__strobe__5354__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5354__ss;
    __Vtask_tb_disk__DOT__strobe__5354__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5355__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5355__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5355__n;
    __Vfunc_tb_disk__DOT__WT__5355__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5356__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5356__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5356__n;
    __Vfunc_tb_disk__DOT__WT__5356__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5357__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5357__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5357__n;
    __Vfunc_tb_disk__DOT__WT__5357__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5358__fn;
    __Vtask_tb_disk__DOT__strobe__5358__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5358__data;
    __Vtask_tb_disk__DOT__strobe__5358__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5358__ss;
    __Vtask_tb_disk__DOT__strobe__5358__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5359__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5359__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5359__n;
    __Vfunc_tb_disk__DOT__WT__5359__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5360__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5360__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5360__n;
    __Vfunc_tb_disk__DOT__WT__5360__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5361__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5361__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5361__n;
    __Vfunc_tb_disk__DOT__WT__5361__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5362__fn;
    __Vtask_tb_disk__DOT__strobe__5362__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5362__data;
    __Vtask_tb_disk__DOT__strobe__5362__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5362__ss;
    __Vtask_tb_disk__DOT__strobe__5362__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5363__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5363__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5363__n;
    __Vfunc_tb_disk__DOT__WT__5363__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5364__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5364__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5364__n;
    __Vfunc_tb_disk__DOT__WT__5364__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5365__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5365__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5365__n;
    __Vfunc_tb_disk__DOT__WT__5365__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5366__fn;
    __Vtask_tb_disk__DOT__strobe__5366__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5366__data;
    __Vtask_tb_disk__DOT__strobe__5366__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5366__ss;
    __Vtask_tb_disk__DOT__strobe__5366__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5367__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5367__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5367__n;
    __Vfunc_tb_disk__DOT__WT__5367__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5368__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5368__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5368__n;
    __Vfunc_tb_disk__DOT__WT__5368__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5369__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5369__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5369__n;
    __Vfunc_tb_disk__DOT__WT__5369__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5370__fn;
    __Vtask_tb_disk__DOT__strobe__5370__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5370__data;
    __Vtask_tb_disk__DOT__strobe__5370__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5370__ss;
    __Vtask_tb_disk__DOT__strobe__5370__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5371__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5371__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5371__n;
    __Vfunc_tb_disk__DOT__WT__5371__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5372__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5372__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5372__n;
    __Vfunc_tb_disk__DOT__WT__5372__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5373__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5373__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5373__n;
    __Vfunc_tb_disk__DOT__WT__5373__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5374__fn;
    __Vtask_tb_disk__DOT__strobe__5374__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5374__data;
    __Vtask_tb_disk__DOT__strobe__5374__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5374__ss;
    __Vtask_tb_disk__DOT__strobe__5374__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5375__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5375__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5375__n;
    __Vfunc_tb_disk__DOT__WT__5375__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5376__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5376__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5376__n;
    __Vfunc_tb_disk__DOT__WT__5376__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5377__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5377__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5377__n;
    __Vfunc_tb_disk__DOT__WT__5377__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5379__fn;
    __Vtask_tb_disk__DOT__strobe__5379__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5379__data;
    __Vtask_tb_disk__DOT__strobe__5379__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5379__ss;
    __Vtask_tb_disk__DOT__strobe__5379__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5380__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5380__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5380__n;
    __Vfunc_tb_disk__DOT__WT__5380__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5381__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5381__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5381__n;
    __Vfunc_tb_disk__DOT__WT__5381__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5382__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5382__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5382__n;
    __Vfunc_tb_disk__DOT__WT__5382__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5383__fn;
    __Vtask_tb_disk__DOT__strobe__5383__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5383__data;
    __Vtask_tb_disk__DOT__strobe__5383__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5383__ss;
    __Vtask_tb_disk__DOT__strobe__5383__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5384__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5384__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5384__n;
    __Vfunc_tb_disk__DOT__WT__5384__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5385__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5385__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5385__n;
    __Vfunc_tb_disk__DOT__WT__5385__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5386__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5386__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5386__n;
    __Vfunc_tb_disk__DOT__WT__5386__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5387__fn;
    __Vtask_tb_disk__DOT__strobe__5387__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5387__data;
    __Vtask_tb_disk__DOT__strobe__5387__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5387__ss;
    __Vtask_tb_disk__DOT__strobe__5387__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5388__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5388__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5388__n;
    __Vfunc_tb_disk__DOT__WT__5388__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5389__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5389__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5389__n;
    __Vfunc_tb_disk__DOT__WT__5389__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5390__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5390__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5390__n;
    __Vfunc_tb_disk__DOT__WT__5390__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5391__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5391__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5391__n;
    __Vfunc_tb_disk__DOT__WT__5391__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__5392__b0;
    __Vtask_tb_disk__DOT__parc_run__5392__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__5392__b1;
    __Vtask_tb_disk__DOT__parc_run__5392__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__5392__b2;
    __Vtask_tb_disk__DOT__parc_run__5392__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__5392__b3;
    __Vtask_tb_disk__DOT__parc_run__5392__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__5392__b4;
    __Vtask_tb_disk__DOT__parc_run__5392__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5393__fn;
    __Vtask_tb_disk__DOT__strobe__5393__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5393__data;
    __Vtask_tb_disk__DOT__strobe__5393__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5393__ss;
    __Vtask_tb_disk__DOT__strobe__5393__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5394__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5394__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5394__n;
    __Vfunc_tb_disk__DOT__WT__5394__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5395__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5395__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5395__n;
    __Vfunc_tb_disk__DOT__WT__5395__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5396__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5396__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5396__n;
    __Vfunc_tb_disk__DOT__WT__5396__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5397__fn;
    __Vtask_tb_disk__DOT__strobe__5397__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5397__data;
    __Vtask_tb_disk__DOT__strobe__5397__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5397__ss;
    __Vtask_tb_disk__DOT__strobe__5397__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5398__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5398__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5398__n;
    __Vfunc_tb_disk__DOT__WT__5398__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5399__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5399__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5399__n;
    __Vfunc_tb_disk__DOT__WT__5399__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5400__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5400__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5400__n;
    __Vfunc_tb_disk__DOT__WT__5400__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5401__fn;
    __Vtask_tb_disk__DOT__strobe__5401__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5401__data;
    __Vtask_tb_disk__DOT__strobe__5401__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5401__ss;
    __Vtask_tb_disk__DOT__strobe__5401__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5402__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5402__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5402__n;
    __Vfunc_tb_disk__DOT__WT__5402__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5403__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5403__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5403__n;
    __Vfunc_tb_disk__DOT__WT__5403__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5404__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5404__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5404__n;
    __Vfunc_tb_disk__DOT__WT__5404__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5405__fn;
    __Vtask_tb_disk__DOT__strobe__5405__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5405__data;
    __Vtask_tb_disk__DOT__strobe__5405__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5405__ss;
    __Vtask_tb_disk__DOT__strobe__5405__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5406__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5406__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5406__n;
    __Vfunc_tb_disk__DOT__WT__5406__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5407__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5407__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5407__n;
    __Vfunc_tb_disk__DOT__WT__5407__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5408__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5408__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5408__n;
    __Vfunc_tb_disk__DOT__WT__5408__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5409__fn;
    __Vtask_tb_disk__DOT__strobe__5409__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5409__data;
    __Vtask_tb_disk__DOT__strobe__5409__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5409__ss;
    __Vtask_tb_disk__DOT__strobe__5409__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5410__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5410__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5410__n;
    __Vfunc_tb_disk__DOT__WT__5410__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5411__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5411__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5411__n;
    __Vfunc_tb_disk__DOT__WT__5411__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5412__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5412__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5412__n;
    __Vfunc_tb_disk__DOT__WT__5412__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5413__fn;
    __Vtask_tb_disk__DOT__strobe__5413__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5413__data;
    __Vtask_tb_disk__DOT__strobe__5413__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5413__ss;
    __Vtask_tb_disk__DOT__strobe__5413__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5414__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5414__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5414__n;
    __Vfunc_tb_disk__DOT__WT__5414__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5415__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5415__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5415__n;
    __Vfunc_tb_disk__DOT__WT__5415__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5416__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5416__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5416__n;
    __Vfunc_tb_disk__DOT__WT__5416__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5417__fn;
    __Vtask_tb_disk__DOT__strobe__5417__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5417__data;
    __Vtask_tb_disk__DOT__strobe__5417__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5417__ss;
    __Vtask_tb_disk__DOT__strobe__5417__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5418__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5418__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5418__n;
    __Vfunc_tb_disk__DOT__WT__5418__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5419__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5419__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5419__n;
    __Vfunc_tb_disk__DOT__WT__5419__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5420__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5420__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5420__n;
    __Vfunc_tb_disk__DOT__WT__5420__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5421__fn;
    __Vtask_tb_disk__DOT__strobe__5421__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5421__data;
    __Vtask_tb_disk__DOT__strobe__5421__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5421__ss;
    __Vtask_tb_disk__DOT__strobe__5421__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5422__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5422__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5422__n;
    __Vfunc_tb_disk__DOT__WT__5422__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5423__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5423__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5423__n;
    __Vfunc_tb_disk__DOT__WT__5423__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5424__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5424__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5424__n;
    __Vfunc_tb_disk__DOT__WT__5424__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5425__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5425__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5425__n;
    __Vfunc_tb_disk__DOT__WT__5425__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5426__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5426__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5426__n;
    __Vfunc_tb_disk__DOT__WT__5426__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__5427__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__5427__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5428__fn;
    __Vtask_tb_disk__DOT__strobe__5428__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5428__data;
    __Vtask_tb_disk__DOT__strobe__5428__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5428__ss;
    __Vtask_tb_disk__DOT__strobe__5428__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5429__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5429__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5429__n;
    __Vfunc_tb_disk__DOT__WT__5429__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5430__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5430__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5430__n;
    __Vfunc_tb_disk__DOT__WT__5430__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5431__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5431__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5431__n;
    __Vfunc_tb_disk__DOT__WT__5431__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5432__fn;
    __Vtask_tb_disk__DOT__strobe__5432__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5432__data;
    __Vtask_tb_disk__DOT__strobe__5432__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5432__ss;
    __Vtask_tb_disk__DOT__strobe__5432__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5433__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5433__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5433__n;
    __Vfunc_tb_disk__DOT__WT__5433__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5434__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5434__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5434__n;
    __Vfunc_tb_disk__DOT__WT__5434__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5435__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5435__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5435__n;
    __Vfunc_tb_disk__DOT__WT__5435__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__b0;
    __Vtask_tb_disk__DOT__parc_micro__5436__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__b1;
    __Vtask_tb_disk__DOT__parc_micro__5436__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__b2;
    __Vtask_tb_disk__DOT__parc_micro__5436__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__b3;
    __Vtask_tb_disk__DOT__parc_micro__5436__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__b4;
    __Vtask_tb_disk__DOT__parc_micro__5436__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5437__fn;
    __Vtask_tb_disk__DOT__strobe__5437__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5437__data;
    __Vtask_tb_disk__DOT__strobe__5437__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5437__ss;
    __Vtask_tb_disk__DOT__strobe__5437__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5438__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5438__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5438__n;
    __Vfunc_tb_disk__DOT__WT__5438__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5439__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5439__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5439__n;
    __Vfunc_tb_disk__DOT__WT__5439__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5440__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5440__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5440__n;
    __Vfunc_tb_disk__DOT__WT__5440__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5441__fn;
    __Vtask_tb_disk__DOT__strobe__5441__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5441__data;
    __Vtask_tb_disk__DOT__strobe__5441__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5441__ss;
    __Vtask_tb_disk__DOT__strobe__5441__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5442__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5442__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5442__n;
    __Vfunc_tb_disk__DOT__WT__5442__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5443__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5443__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5443__n;
    __Vfunc_tb_disk__DOT__WT__5443__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5444__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5444__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5444__n;
    __Vfunc_tb_disk__DOT__WT__5444__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5445__fn;
    __Vtask_tb_disk__DOT__strobe__5445__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5445__data;
    __Vtask_tb_disk__DOT__strobe__5445__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5445__ss;
    __Vtask_tb_disk__DOT__strobe__5445__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5446__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5446__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5446__n;
    __Vfunc_tb_disk__DOT__WT__5446__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5447__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5447__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5447__n;
    __Vfunc_tb_disk__DOT__WT__5447__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5448__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5448__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5448__n;
    __Vfunc_tb_disk__DOT__WT__5448__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5449__fn;
    __Vtask_tb_disk__DOT__strobe__5449__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5449__data;
    __Vtask_tb_disk__DOT__strobe__5449__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5449__ss;
    __Vtask_tb_disk__DOT__strobe__5449__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5450__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5450__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5450__n;
    __Vfunc_tb_disk__DOT__WT__5450__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5451__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5451__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5451__n;
    __Vfunc_tb_disk__DOT__WT__5451__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5452__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5452__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5452__n;
    __Vfunc_tb_disk__DOT__WT__5452__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5453__fn;
    __Vtask_tb_disk__DOT__strobe__5453__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5453__data;
    __Vtask_tb_disk__DOT__strobe__5453__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5453__ss;
    __Vtask_tb_disk__DOT__strobe__5453__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5454__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5454__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5454__n;
    __Vfunc_tb_disk__DOT__WT__5454__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5455__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5455__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5455__n;
    __Vfunc_tb_disk__DOT__WT__5455__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5456__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5456__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5456__n;
    __Vfunc_tb_disk__DOT__WT__5456__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5457__fn;
    __Vtask_tb_disk__DOT__strobe__5457__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5457__data;
    __Vtask_tb_disk__DOT__strobe__5457__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5457__ss;
    __Vtask_tb_disk__DOT__strobe__5457__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5458__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5458__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5458__n;
    __Vfunc_tb_disk__DOT__WT__5458__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5459__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5459__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5459__n;
    __Vfunc_tb_disk__DOT__WT__5459__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5460__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5460__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5460__n;
    __Vfunc_tb_disk__DOT__WT__5460__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5461__fn;
    __Vtask_tb_disk__DOT__strobe__5461__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5461__data;
    __Vtask_tb_disk__DOT__strobe__5461__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5461__ss;
    __Vtask_tb_disk__DOT__strobe__5461__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5462__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5462__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5462__n;
    __Vfunc_tb_disk__DOT__WT__5462__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5463__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5463__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5463__n;
    __Vfunc_tb_disk__DOT__WT__5463__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5464__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5464__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5464__n;
    __Vfunc_tb_disk__DOT__WT__5464__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5466__fn;
    __Vtask_tb_disk__DOT__strobe__5466__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5466__data;
    __Vtask_tb_disk__DOT__strobe__5466__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5466__ss;
    __Vtask_tb_disk__DOT__strobe__5466__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5467__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5467__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5467__n;
    __Vfunc_tb_disk__DOT__WT__5467__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5468__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5468__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5468__n;
    __Vfunc_tb_disk__DOT__WT__5468__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5469__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5469__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5469__n;
    __Vfunc_tb_disk__DOT__WT__5469__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5470__fn;
    __Vtask_tb_disk__DOT__strobe__5470__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5470__data;
    __Vtask_tb_disk__DOT__strobe__5470__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5470__ss;
    __Vtask_tb_disk__DOT__strobe__5470__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5471__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5471__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5471__n;
    __Vfunc_tb_disk__DOT__WT__5471__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5472__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5472__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5472__n;
    __Vfunc_tb_disk__DOT__WT__5472__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5473__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5473__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5473__n;
    __Vfunc_tb_disk__DOT__WT__5473__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5474__fn;
    __Vtask_tb_disk__DOT__strobe__5474__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5474__data;
    __Vtask_tb_disk__DOT__strobe__5474__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5474__ss;
    __Vtask_tb_disk__DOT__strobe__5474__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5475__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5475__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5475__n;
    __Vfunc_tb_disk__DOT__WT__5475__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5476__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5476__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5476__n;
    __Vfunc_tb_disk__DOT__WT__5476__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5477__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5477__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5477__n;
    __Vfunc_tb_disk__DOT__WT__5477__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5478__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5478__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5478__n;
    __Vfunc_tb_disk__DOT__WT__5478__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__b0;
    __Vtask_tb_disk__DOT__parc_micro__5480__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__b2;
    __Vtask_tb_disk__DOT__parc_micro__5480__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__b3;
    __Vtask_tb_disk__DOT__parc_micro__5480__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__b4;
    __Vtask_tb_disk__DOT__parc_micro__5480__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5481__fn;
    __Vtask_tb_disk__DOT__strobe__5481__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5481__data;
    __Vtask_tb_disk__DOT__strobe__5481__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5481__ss;
    __Vtask_tb_disk__DOT__strobe__5481__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5482__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5482__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5482__n;
    __Vfunc_tb_disk__DOT__WT__5482__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5483__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5483__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5483__n;
    __Vfunc_tb_disk__DOT__WT__5483__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5484__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5484__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5484__n;
    __Vfunc_tb_disk__DOT__WT__5484__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5485__fn;
    __Vtask_tb_disk__DOT__strobe__5485__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5485__data;
    __Vtask_tb_disk__DOT__strobe__5485__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5485__ss;
    __Vtask_tb_disk__DOT__strobe__5485__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5486__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5486__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5486__n;
    __Vfunc_tb_disk__DOT__WT__5486__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5487__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5487__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5487__n;
    __Vfunc_tb_disk__DOT__WT__5487__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5488__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5488__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5488__n;
    __Vfunc_tb_disk__DOT__WT__5488__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5489__fn;
    __Vtask_tb_disk__DOT__strobe__5489__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5489__data;
    __Vtask_tb_disk__DOT__strobe__5489__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5489__ss;
    __Vtask_tb_disk__DOT__strobe__5489__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5490__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5490__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5490__n;
    __Vfunc_tb_disk__DOT__WT__5490__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5491__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5491__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5491__n;
    __Vfunc_tb_disk__DOT__WT__5491__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5492__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5492__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5492__n;
    __Vfunc_tb_disk__DOT__WT__5492__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5493__fn;
    __Vtask_tb_disk__DOT__strobe__5493__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5493__data;
    __Vtask_tb_disk__DOT__strobe__5493__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5493__ss;
    __Vtask_tb_disk__DOT__strobe__5493__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5494__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5494__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5494__n;
    __Vfunc_tb_disk__DOT__WT__5494__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5495__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5495__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5495__n;
    __Vfunc_tb_disk__DOT__WT__5495__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5496__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5496__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5496__n;
    __Vfunc_tb_disk__DOT__WT__5496__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5497__fn;
    __Vtask_tb_disk__DOT__strobe__5497__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5497__data;
    __Vtask_tb_disk__DOT__strobe__5497__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5497__ss;
    __Vtask_tb_disk__DOT__strobe__5497__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5498__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5498__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5498__n;
    __Vfunc_tb_disk__DOT__WT__5498__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5499__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5499__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5499__n;
    __Vfunc_tb_disk__DOT__WT__5499__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5500__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5500__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5500__n;
    __Vfunc_tb_disk__DOT__WT__5500__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5501__fn;
    __Vtask_tb_disk__DOT__strobe__5501__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5501__data;
    __Vtask_tb_disk__DOT__strobe__5501__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5501__ss;
    __Vtask_tb_disk__DOT__strobe__5501__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5502__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5502__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5502__n;
    __Vfunc_tb_disk__DOT__WT__5502__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5503__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5503__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5503__n;
    __Vfunc_tb_disk__DOT__WT__5503__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5504__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5504__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5504__n;
    __Vfunc_tb_disk__DOT__WT__5504__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__5505__fn;
    __Vtask_tb_disk__DOT__strobe__5505__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__5505__data;
    __Vtask_tb_disk__DOT__strobe__5505__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__5505__ss;
    __Vtask_tb_disk__DOT__strobe__5505__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5506__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5506__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5506__n;
    __Vfunc_tb_disk__DOT__WT__5506__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5507__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5507__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5507__n;
    __Vfunc_tb_disk__DOT__WT__5507__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5508__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5508__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5508__n;
    __Vfunc_tb_disk__DOT__WT__5508__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5511__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5511__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5511__n;
    __Vfunc_tb_disk__DOT__WT__5511__n = 0;
    // Body
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
                                                             3476);
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
                      | (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_15) 
                          >> 7U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))))) {
            tb_disk__DOT__nd0 = ((IData)(1U) + tb_disk__DOT__nd0);
        }
        if ((1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))))) {
            tb_disk__DOT__nwm = ((IData)(1U) + tb_disk__DOT__nwm);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))) 
                   & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_15) 
                         >> 7U))))) {
            tb_disk__DOT__nall3 = ((IData)(1U) + tb_disk__DOT__nall3);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)))) {
            tb_disk__DOT__nwim = ((IData)(1U) + tb_disk__DOT__nwim);
        }
        if ((1U & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_15) 
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
                = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_15) 
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
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                           >> 3U)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
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
                                   | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__q) 
                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a01__DOT__p7))) 
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
        __Vfunc_tb_disk__DOT__WT__5291__n = 0x0000003cU;
        __Vfunc_tb_disk__DOT__WT__5291__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5291__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5291__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__5291__Vfuncout = 1U;
        }
        tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
            = __Vfunc_tb_disk__DOT__WT__5291__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4140);
            tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
                = (tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
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
    __Vfunc_tb_disk__DOT__WT__5292__n = 0x0000003cU;
    __Vfunc_tb_disk__DOT__WT__5292__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5292__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5292__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5292__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
        = __Vfunc_tb_disk__DOT__WT__5292__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4157);
        tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
            = (tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
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
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4222: Assertion failed in %Ntb_disk: DskEth selected on %0d samples but only %0d with the processor free -- a stepped processor must not reach a device\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_sel,
                     32,vlSelfRef.tb_disk__DOT__n_sel_free);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4222, "", false);
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
    VL_WRITEF_NX("tb_disk:   THE DATA WORD -- of %0d strobes, %0d carried the FF literal 5a00\ntb_disk:   THE ADDRESS AT THE STROBE -- TIOA held %o on %0d of %0d samples; of %0d IOBout strobes %0d carried it (last %o)\ntb_disk:   THE MUX -- TIOAWrite' asserted %0d, TIOABypass high %0d, Curr=Next' asserted %0d (of %0d); at the strobe bypass=%b ff4=%b RAM nibble=%b, and %0d of %0d strobes had the bypass ON\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_iobout,
                 32,vlSelfRef.tb_disk__DOT__n_iob_lit,
                 8,(IData)(vlSelfRef.tb_disk__DOT__want_tioa),
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
    VL_WRITEF_NX("tb_disk:   THE CONTROL REGISTER -- ControlRegCl edges %0d (all while DISKCONTROL addressed: %0d); at the last edge bIOB.05/06/07 = %b, {DebugMode,BlockTillIndex,EnableRun} = %b, 8 cycles later = %b, at end of run = %b\ntb_disk:   THE SEEK PATH -- Tag_IOB edges %0d (all while DISKTAG addressed: %0d), TagClock edges %0d\ntb_disk:   THE MUFFLER CLEAR -- ClearIndexTW high on %0d of %0d, IndexTW high on %0d\ntb_disk:   THE COMMAND -- ReadBlock high on %0d of %0d, Active high on %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_crc_edge,
                 32,vlSelfRef.tb_disk__DOT__n_crc_free,
                 3,(IData)(vlSelfRef.tb_disk__DOT__iobits),
                 3,vlSelfRef.tb_disk__DOT__ctlbits,
                 3,(IData)(vlSelfRef.tb_disk__DOT__ctl_post),
                 3,vlSelfRef.tb_disk__DOT__ctl_final,
                 32,vlSelfRef.tb_disk__DOT__n_tag_edge,
                 32,vlSelfRef.tb_disk__DOT__n_tag_free,
                 32,vlSelfRef.tb_disk__DOT__n_tagclk,
                 32,vlSelfRef.tb_disk__DOT__n_clridx,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_idxtw_run,
                 32,vlSelfRef.tb_disk__DOT__n_rdblk,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_actv);
    VL_WRITEF_NX("tb_disk:   THE FORMAT RAM -- RamCl'C edges %0d, LastRamAddr' asserted on %0d, address now %b\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_ramcl,
                 32,vlSelfRef.tb_disk__DOT__n_lastram,
                 4,(IData)(vlSelfRef.tb_disk__DOT__ramaddr_last));
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        VL_WRITEF_NX("tb_disk:   FORMAT RAM CONTENTS (12-bit, e16:f16:f17) vs HM p.98:\n",0);
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 0] = %04o   want 0001   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((1U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((1U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[0] = %04o, want 0001\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [1U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [1U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [1U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 1] = %04o   want 0007   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((7U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((7U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[1] = %04o, want 0007\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [2U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [2U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [2U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 2] = %04o   want 0377   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0x00ffU == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0x00ffU != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[2] = %04o, want 0377\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [3U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [3U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [3U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 3] = %04o   want 0000   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[3] = %04o, want 0000\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [4U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [4U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [4U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 4] = %04o   want 0104   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0x0044U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0x0044U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[4] = %04o, want 0104\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [5U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [5U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [5U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 5] = %04o   want 0204   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0x0084U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0x0084U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[5] = %04o, want 0204\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [6U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [6U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [6U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 6] = %04o   want 0004   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((4U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((4U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[6] = %04o, want 0004\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [7U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [7U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [7U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 7] = %04o   want 0000   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[7] = %04o, want 0000\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [8U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [8U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [8U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 8] = %04o   want 0033   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0x001bU == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0x001bU != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[8] = %04o, want 0033\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [9U];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [9U];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [9U];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [ 9] = %04o   want 0006   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((6U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((6U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[9] = %04o, want 0006\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0aU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0aU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0aU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [10] = %04o   want 0011   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((9U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((9U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[10] = %04o, want 0011\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0bU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0bU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0bU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [11] = %04o   want 0002   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((2U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((2U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[11] = %04o, want 0002\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0cU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0cU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0cU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [12] = %04o   want 0002   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((2U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((2U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[12] = %04o, want 0002\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0dU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0dU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0dU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [13] = %04o   want 0001   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((1U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((1U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[13] = %04o, want 0001\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0eU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0eU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0eU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [14] = %04o   want 0000   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[14] = %04o, want 0000\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        vlSelfRef.tb_disk__DOT__fwgot = ((([&]() {
                        __Vfunc_tb_disk__DOT__revn__5293__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                            [0x0fU];
                        __Vfunc_tb_disk__DOT__revn__5293__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5293__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5293__Vfuncout)) 
                                          << 8U) | 
                                         ((([&]() {
                            __Vfunc_tb_disk__DOT__revn__5294__n 
                                = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                                [0x0fU];
                            __Vfunc_tb_disk__DOT__revn__5294__Vfuncout 
                                = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                            << 1U)) 
                                     | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(__Vfunc_tb_disk__DOT__revn__5294__n) 
                                                    >> 3U))));
                        }(), (IData)(__Vfunc_tb_disk__DOT__revn__5294__Vfuncout)) 
                                           << 4U) | 
                                          ([&]() {
                        __Vfunc_tb_disk__DOT__revn__5295__n 
                            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                            [0x0fU];
                        __Vfunc_tb_disk__DOT__revn__5295__Vfuncout 
                            = ((((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                        << 1U)) | (1U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                      >> 1U))) 
                                << 2U) | ((2U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(__Vfunc_tb_disk__DOT__revn__5295__n) 
                                                   >> 3U))));
                    }(), (IData)(__Vfunc_tb_disk__DOT__revn__5295__Vfuncout))));
        VL_WRITEF_NX("tb_disk:     [15] = %04o   want 0000   %s\n",0,
                     12,vlSelfRef.tb_disk__DOT__fwgot,
                     64,((0U == (IData)(vlSelfRef.tb_disk__DOT__fwgot))
                          ? 0x0000000000006f6bULL : 0x4d49534d41544348ULL));
        if (VL_UNLIKELY(((0U != (IData)(vlSelfRef.tb_disk__DOT__fwgot))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4280: Assertion failed in %Ntb_disk: format RAM[15] = %04o, want 0000\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),12,(IData)(vlSelfRef.tb_disk__DOT__fwgot));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4280, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...so ALL SIXTEEN words of HM p.98's Alto Diablo format are loaded and verified\n",0);
        if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b09__DOT__qb)))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4295: Assertion failed in %Ntb_disk: a DISKTAG write did not release the bit counter (b10 PE' = %b)\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b09__DOT__qb));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4295, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...and a DISKTAG write RELEASES THE BIT COUNTER: b10 PE' = 1\n",0);
    }
    VL_WRITEF_NX("tb_disk:   TIOA WRITE DATA -- at the write: h13 out (lo, D3..D0) = %b, h12 out (hi) = %b, alub.04-07 = %b, FFdly.4 = %b\ntb_disk:   TIOA alub TIMING -- alub.04-07 = 1000 on %0d of %0d samples, and of those %0d had TIOAWrite' OPEN\ntb_disk:   TIOA WRITE START -- at WE' FALLING (%0d of them): h12 out (hi) = %b, alub.04-07 = %b, FFdly.4 = %b\ntb_disk:   TIOA WRITE COND -- g15 p3(CE')=%b, TIOAWrite' RISING edges %0d, cell write condition true on %0d samples\n",0,
                 4,vlSelfRef.tb_disk__DOT__wdata_lo,
                 4,(IData)(vlSelfRef.tb_disk__DOT__wdata_hi),
                 4,vlSelfRef.tb_disk__DOT__alub_hi,
                 1,(IData)(vlSelfRef.tb_disk__DOT__ff4_at_wr),
                 32,vlSelfRef.tb_disk__DOT__n_alub_ok,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_alub_ok_wr,
                 32,vlSelfRef.tb_disk__DOT__n_fall,
                 4,(IData)(vlSelfRef.tb_disk__DOT__wdata_fall),
                 4,vlSelfRef.tb_disk__DOT__alub_fall,
                 1,(IData)(vlSelfRef.tb_disk__DOT__ff4_fall),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__p3,
                 32,vlSelfRef.tb_disk__DOT__n_werise,
                 32,vlSelfRef.tb_disk__DOT__n_wecond);
    VL_WRITEF_NX("tb_disk:   TIOA RAM g15 -- [0]=%b [1]=%b [2]=%b [3]=%b [4]=%b [5]=%b [6]=%b [7]=%b\ntb_disk:   TIOA RAM h15 -- [0]=%b [1]=%b [2]=%b [3]=%b [4]=%b [5]=%b [6]=%b [7]=%b\ntb_disk:   TIOA RAM h15 -- [8]=%b [9]=%b [10]=%b [11]=%b [12]=%b [13]=%b [14]=%b [15]=%b | read address LastNext = %b%b%b%b\ntb_disk:   THE READ PATH -- bIOin' asserted on %0d of %0d, IOB output enable ASSERTED (low) on %0d (IOB there = %x)\ntb_disk:   DRIVE LINES before force -- TtlReady'=%b TtlOnLine'=%b Selected0'=%b\n",0,
                 4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [0U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [1U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [2U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [3U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [4U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [5U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [6U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                 [7U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [1U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [2U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [3U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [4U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [5U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [6U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [7U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [8U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [9U],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0aU],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0bU],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0cU],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0dU],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0eU],4,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                 [0x0fU],1,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_0_p_,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_1_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_2_p_,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_3_p_),
                 32,vlSelfRef.tb_disk__DOT__n_iobin,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_ioen,16,
                 (IData)(vlSelfRef.tb_disk__DOT__iob_at_en),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd);
    VL_WRITEF_NX("tb_disk:   ...and the controller sees NO drive: NotReady=%b NotOnLine=%b ReadError=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError);
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
    tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34 = 0x00000040U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4362);
        tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34 
            = (tb_disk__DOT__unnamedblk1_35__DOT____Vrepeat34 
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
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4370: Assertion failed in %Ntb_disk: TtlReady' asserted but NotReady is still %b -- c24 does not pass the cable through\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4370, "", false);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4373: Assertion failed in %Ntb_disk: TtlOnLine' asserted but NotOnLine is still %b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4373, "", false);
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
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4396: Assertion failed in %Ntb_disk: Selected0' asserted but the board does not see a selected drive: NotSelected=%b DrSelected=%b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected),
                     1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected))));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4396, "", false);
    }
    if (VL_UNLIKELY(((1U & (~ (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb)) 
                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa))))))) {
        VL_WRITEF_NX("tb_disk:   (note) the controller-side errors cleared on their own -- unexpected, re-read this\n",0);
    }
    VL_WRITEF_NX("tb_disk:   ...so four cable lines give a HEALTHY SELECTED DRIVE; what is left is controller state\ntb_disk:   SECTOR before -- TtlSector'=%b Sector=%b Index'=1 SectorTW=%b IndexTW=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa))));
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
    tb_disk__DOT__n_sectw = 0U;
    tb_disk__DOT__n_idxtw = 0U;
    tb_disk__DOT__n_secpulse = 0U;
    tb_disk__DOT__n_secgap = 0U;
    vlSelfRef.tb_disk__DOT__twin = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                    >> (0x0000000fU & ((IData)(0x0fU) 
                                       - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                 >> (0x0000000fU & ((IData)(0x0fU) 
                                    - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    while (VL_GTS_III(32, 8U, vlSelfRef.tb_disk__DOT__twin)) {
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd = 0U;
        tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35 = 0x00000018U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4424);
            tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35 
                = (tb_disk__DOT__unnamedblk1_36__DOT____Vrepeat35 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd = 1U;
        tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36 = 0x00000018U;
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_)))) {
            tb_disk__DOT__n_secpulse = ((IData)(1U) 
                                        + tb_disk__DOT__n_secpulse);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb)))) {
            tb_disk__DOT__n_sectw = ((IData)(1U) + tb_disk__DOT__n_sectw);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa)))) {
            tb_disk__DOT__n_idxtw = ((IData)(1U) + tb_disk__DOT__n_idxtw);
        }
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4429);
            tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36 
                = (tb_disk__DOT__unnamedblk1_37__DOT____Vrepeat36 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa)))) {
            tb_disk__DOT__n_idxtw = ((IData)(1U) + tb_disk__DOT__n_idxtw);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb)))) {
            tb_disk__DOT__n_sectw = ((IData)(1U) + tb_disk__DOT__n_sectw);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_)))) {
            tb_disk__DOT__n_secgap = ((IData)(1U) + tb_disk__DOT__n_secgap);
        }
        vlSelfRef.tb_disk__DOT__twin = ((IData)(1U) 
                                        + vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    }
    VL_WRITEF_NX("tb_disk:   SECTOR PULSES -- 8 pulses on SecIndx0': Sector high on %0d of 8 pulses and %0d of 8 GAPS, SectorTW on %0d, IndexTW on %0d (of 16)\ntb_disk:   SECTOR after  -- TtlSector'=%b Sector=%b Index'=1 SectorTW=%b IndexTW=%b\n",0,
                 32,tb_disk__DOT__n_secpulse,32,tb_disk__DOT__n_secgap,
                 32,tb_disk__DOT__n_sectw,32,tb_disk__DOT__n_idxtw,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa))));
    if (VL_UNLIKELY(((8U != tb_disk__DOT__n_secpulse)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4445: Assertion failed in %Ntb_disk: SecIndx0' pulsed 8 times but Sector followed only %0d\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,tb_disk__DOT__n_secpulse);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4445, "", false);
    }
    if (VL_UNLIKELY(((0U != tb_disk__DOT__n_secgap)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4447: Assertion failed in %Ntb_disk: Sector was high in %0d of 8 gaps -- it is stuck, not tracking\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,tb_disk__DOT__n_secgap);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4447, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...so the sector pulse reaches the controller and TRACKS; the TWs need a DISKMUFF clear first (open)\ntb_disk:   SERIAL PATH -- Select.0=%b Select.1=%b NotSelected=%b\n",0,
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                           >> 2U)),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected));
    vlSelfRef.tb_disk__DOT__twin = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                    >> (0x0000000fU & ((IData)(0x0fU) 
                                       - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                 >> (0x0000000fU & ((IData)(0x0fU) 
                                    - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    while (VL_GTS_III(32, 8U, vlSelfRef.tb_disk__DOT__twin)) {
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
            = (1U & vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
            = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd = 0U;
        tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37 = 0x0000000cU;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4468);
            tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37 
                = (tb_disk__DOT__unnamedblk1_38__DOT____Vrepeat37 
                   - (IData)(1U));
        }
        if ((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                   | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected)) 
                      & ((0U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                       >> 2U))) ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f02__DOT__qa)
                          : ((1U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                           >> 2U)))
                              ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f02__DOT__qb)
                              : ((2U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                               >> 2U)))
                                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c02__DOT__qa)
                                  : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c02__DOT__qb)))))))) {
            vlSelfRef.tb_disk__DOT__n_dat1 = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_dat1);
        } else {
            vlSelfRef.tb_disk__DOT__n_dat0 = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_dat0);
        }
        if ((1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                    >> 1U) | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected)) 
                              & ((0U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                               >> 2U)))
                                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f01__DOT__qa)
                                  : ((1U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                   >> 2U)))
                                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f01__DOT__qb)
                                      : ((2U == (3U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                    >> 2U)))
                                          ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c01__DOT__qa)
                                          : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c01__DOT__qb)))))))) {
            vlSelfRef.tb_disk__DOT__n_clk1 = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_clk1);
        }
        vlSelfRef.tb_disk__DOT__twin = ((IData)(1U) 
                                        + vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    }
    VL_WRITEF_NX("tb_disk:   SERIAL PATH -- 8 alternating bits: PreReadData high %0d, low %0d; PrePreBitClock high %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_dat1,
                 32,vlSelfRef.tb_disk__DOT__n_dat0,
                 32,vlSelfRef.tb_disk__DOT__n_clk1);
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn = 0U;
    tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38 = 0x00000020U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4478);
        tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38 
            = (tb_disk__DOT__unnamedblk1_39__DOT____Vrepeat38 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__twin = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                    >> (0x0000000fU & ((IData)(0x0fU) 
                                       - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                 >> (0x0000000fU & ((IData)(0x0fU) 
                                    - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    while (VL_GTS_III(32, 8U, vlSelfRef.tb_disk__DOT__twin)) {
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
            = (1U & vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
            = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
        tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 = 0x0000000cU;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4481);
            tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 
                = (tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 
                   - (IData)(1U));
        }
        if ((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                   | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected)) 
                      & ((0U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                       >> 2U))) ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f02__DOT__qa)
                          : ((1U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                           >> 2U)))
                              ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f02__DOT__qb)
                              : ((2U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                               >> 2U)))
                                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c02__DOT__qa)
                                  : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c02__DOT__qb)))))))) {
            vlSelfRef.tb_disk__DOT__n_desel = ((IData)(1U) 
                                               + vlSelfRef.tb_disk__DOT__n_desel);
        }
        vlSelfRef.tb_disk__DOT__twin = ((IData)(1U) 
                                        + vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    }
    VL_WRITEF_NX("tb_disk:   SERIAL PATH -- deselected, NotSelected=%b, PreReadData high on %0d of 8\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected,
                 32,vlSelfRef.tb_disk__DOT__n_desel);
    if (VL_UNLIKELY((((4U != vlSelfRef.tb_disk__DOT__n_dat1) 
                      | (4U != vlSelfRef.tb_disk__DOT__n_dat0))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4493: Assertion failed in %Ntb_disk: 8 alternating data bits gave PreReadData high %0d low %0d, not 4/4\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_dat1,
                     32,vlSelfRef.tb_disk__DOT__n_dat0);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4493, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_clk1)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4496: Assertion failed in %Ntb_disk: the drive's read clock never reached PrePreBitClock\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4496, "", false);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_desel)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4498: Assertion failed in %Ntb_disk: PreReadData followed the cable on %0d of 8 samples with the drive DESELECTED\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_desel);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4498, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...so the drive's DATA and CLOCK reach the controller, and only when selected\n",0);
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd = 0U;
    tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40 = 0x00000020U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4507);
        tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40 
            = (tb_disk__DOT__unnamedblk1_41__DOT____Vrepeat40 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__shreg_first = ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__q) 
                                             << 0x0000000cU) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__q) 
                                               << 8U)) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__q) 
                                               << 4U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__q)));
    VL_WRITEF_NX("tb_disk:   SHIFTER before -- Idle=%b Active=%b ShiftReg=%x InRegFull=%b FifoWaddr=%b%b%b%b\n",0,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa),
                 16,vlSelfRef.tb_disk__DOT__shreg_first,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                           >> 2U)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                          >> 1U)),1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q)));
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd = 0U;
    tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41 = 0x00000020U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4527);
        tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41 
            = (tb_disk__DOT__unnamedblk1_42__DOT____Vrepeat41 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd = 1U;
    tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 = 0x00000020U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4529);
        tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 
            = (tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_disk:   AFTER A SECTOR PULSE -- Sector=%b Active=%b ShiftIn=%b ComputeECC=%b sCountBits=%b ShiftReg=%x\ntb_disk:   SEQUENCE PROMS -- addr = %b%b%b%b%b | a20 CE'(WriteBlock')=%b  a21 CE'=%b | b20 D = %b%b%b%b\n",0,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                          >> 1U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                           >> 2U)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Tag_u_IOB) 
                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__q) 
                               >> 3U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e21__DOT__q)) 
                             & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__q)))),
                 16,((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__q) 
                       << 0x0000000cU) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__q) 
                                          << 8U)) | 
                     (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__q) 
                       << 4U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__q))),
                 1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q)),
                 1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                          >> 1U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                           >> 2U)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                          >> 3U)),1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ja))),
                 1,(1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f14__DOT__q) 
                              >> 3U) ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b19__DOT__p9)))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PromA4_p_),
                 1,(1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_13) 
                          >> 7U)),1,(1U & (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                                            | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_13)) 
                                           >> 6U)),
                 1,(1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                          >> 7U)));
    vlSelfRef.tb_disk__DOT__twin = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                    >> (0x0000000fU & ((IData)(0x0fU) 
                                       - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                 >> (0x0000000fU & ((IData)(0x0fU) 
                                    - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    while (VL_GTS_III(32, 0x00000100U, vlSelfRef.tb_disk__DOT__twin)) {
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd = 0U;
        tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 = 6U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4570);
            tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 
                = (tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 
                   - (IData)(1U));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull) 
             & (~ (IData)(vlSelfRef.tb_disk__DOT__irf_d)))) {
            vlSelfRef.tb_disk__DOT__n_irf = ((IData)(1U) 
                                             + vlSelfRef.tb_disk__DOT__n_irf);
            if ((1U == vlSelfRef.tb_disk__DOT__n_irf)) {
                vlSelfRef.tb_disk__DOT__fwaddr_first 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q;
            }
        }
        vlSelfRef.tb_disk__DOT__irf_d = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull;
        vlSelfRef.tb_disk__DOT__fwaddr_last2 = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q;
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_)))) {
            vlSelfRef.tb_disk__DOT__n_bclk = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_bclk);
        }
        if ((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Tag_u_IOB) 
                   | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__q) 
                        >> 3U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e21__DOT__q)) 
                      & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__q))))) {
            vlSelfRef.tb_disk__DOT__n_scnt = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_scnt);
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B) 
             & (~ (IData)(vlSelfRef.tb_disk__DOT__bclkb_d)))) {
            vlSelfRef.tb_disk__DOT__n_bclkb = ((IData)(1U) 
                                               + vlSelfRef.tb_disk__DOT__n_bclkb);
        }
        vlSelfRef.tb_disk__DOT__bclkb_d = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B;
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ShiftRegLd_p_)))) {
            vlSelfRef.tb_disk__DOT__n_shld = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_shld);
        }
        if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadData) {
            vlSelfRef.tb_disk__DOT__n_rdata = ((IData)(1U) 
                                               + vlSelfRef.tb_disk__DOT__n_rdata);
        }
        if ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))) {
            vlSelfRef.tb_disk__DOT__n_cecc = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_cecc);
        }
        if (((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))
              ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadData)
              : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccData_32))) {
            vlSelfRef.tb_disk__DOT__n_shin = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_shin);
        }
        if ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))) {
            vlSelfRef.tb_disk__DOT__n_shiftin = ((IData)(1U) 
                                                 + vlSelfRef.tb_disk__DOT__n_shiftin);
        }
        vlSelfRef.tb_disk__DOT__promaddr = ((((4U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                                << 2U)) 
                                              | ((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                       >> 1U)))) 
                                             << 2U) 
                                            | ((2U 
                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                   >> 1U)) 
                                               | (1U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                     >> 3U))));
        vlSelfRef.tb_disk__DOT__promseen = (vlSelfRef.tb_disk__DOT__promseen 
                                            | (0x00000000ffffffffULL 
                                               & ((IData)(1U) 
                                                  << (IData)(vlSelfRef.tb_disk__DOT__promaddr))));
        vlSelfRef.tb_disk__DOT__promaddr2 = ((0x0000001cU 
                                              & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                 << 1U)) 
                                             | ((2U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a22__DOT__q) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))));
        vlSelfRef.tb_disk__DOT__promseen2 = (vlSelfRef.tb_disk__DOT__promseen2 
                                             | (0x00000000ffffffffULL 
                                                & ((IData)(1U) 
                                                   << (IData)(vlSelfRef.tb_disk__DOT__promaddr2))));
        vlSelfRef.tb_disk__DOT__promaddr2_last = vlSelfRef.tb_disk__DOT__promaddr2;
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WordClock_p_) 
             & (~ (IData)(vlSelfRef.tb_disk__DOT__wclk_d)))) {
            vlSelfRef.tb_disk__DOT__n_wclk = ((IData)(1U) 
                                              + vlSelfRef.tb_disk__DOT__n_wclk);
        }
        vlSelfRef.tb_disk__DOT__wclk_d = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WordClock_p_;
        if ((0x0000000fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b10__DOT__q))) {
            vlSelfRef.tb_disk__DOT__n_co = ((IData)(1U) 
                                            + vlSelfRef.tb_disk__DOT__n_co);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_)))) {
            vlSelfRef.tb_disk__DOT__n_cntdone = ((IData)(1U) 
                                                 + vlSelfRef.tb_disk__DOT__n_cntdone);
        }
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd = 1U;
        tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 = 6U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4627);
            tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 
                = (tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 
                   - (IData)(1U));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B) 
             & (~ (IData)(vlSelfRef.tb_disk__DOT__bclkb_d)))) {
            vlSelfRef.tb_disk__DOT__n_bclkb = ((IData)(1U) 
                                               + vlSelfRef.tb_disk__DOT__n_bclkb);
        }
        vlSelfRef.tb_disk__DOT__shreg_last = ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__q) 
                                                << 0x0000000cU) 
                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__q) 
                                                  << 8U)) 
                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__q) 
                                                  << 4U) 
                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__q)));
        if (((IData)(vlSelfRef.tb_disk__DOT__shreg_last) 
             != (IData)(vlSelfRef.tb_disk__DOT__shreg_first))) {
            vlSelfRef.tb_disk__DOT__n_shmove = ((IData)(1U) 
                                                + vlSelfRef.tb_disk__DOT__n_shmove);
        }
        vlSelfRef.tb_disk__DOT__bclkb_d = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B;
        vlSelfRef.tb_disk__DOT__twin = ((IData)(1U) 
                                        + vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                        >> (0x0000000fU & ((IData)(0x0fU) 
                                           - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__FEDWORD) 
                     >> (0x0000000fU & ((IData)(0x0fU) 
                                        - VL_MODDIVS_III(32, vlSelfRef.tb_disk__DOT__twin, (IData)(0x00000010U))))));
    }
    VL_WRITEF_NX("tb_disk:   SHIFTER after  -- 256 bit clocks: PreBitClock high %0d, BitClock'B EDGES %0d, ShiftRegLd' low on %0d, sCountBits high %0d, ShiftReg moved on %0d (%x -> %x)\ntb_disk:   SHIFTER entry  -- ReadData %0d, ShiftIn %0d, ComputeECC %0d, ShiftReg.in %0d\ntb_disk:   SEQUENCER      -- WordClock' EDGES %0d, bit-counter CO' asserted %0d, CntDone' asserted %0d\ntb_disk:   SHIFTER state  -- Idle=%b Active=%b InRegFull=%b FifoWaddr=%b%b%b%b FifoEmpty=%b\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_bclk,
                 32,vlSelfRef.tb_disk__DOT__n_bclkb,
                 32,vlSelfRef.tb_disk__DOT__n_shld,
                 32,vlSelfRef.tb_disk__DOT__n_scnt,
                 32,vlSelfRef.tb_disk__DOT__n_shmove,
                 16,(IData)(vlSelfRef.tb_disk__DOT__shreg_first),
                 16,vlSelfRef.tb_disk__DOT__shreg_last,
                 32,vlSelfRef.tb_disk__DOT__n_rdata,
                 32,vlSelfRef.tb_disk__DOT__n_shiftin,
                 32,vlSelfRef.tb_disk__DOT__n_cecc,
                 32,vlSelfRef.tb_disk__DOT__n_shin,
                 32,vlSelfRef.tb_disk__DOT__n_wclk,
                 32,vlSelfRef.tb_disk__DOT__n_co,32,
                 vlSelfRef.tb_disk__DOT__n_cntdone,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull,
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                           >> 2U)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                          >> 1U)),1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q)),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__FifoEmpty));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)) 
           & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10));
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn = 0U;
    tb_disk__DOT__fraddr_before = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__5296__v = 0x000cU;
    __Vtask_tb_disk__DOT__strobe__5297__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5297__data = (0x000000ffU 
                                                & (~ 
                                                   ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__5296__v) 
                                                    >> 8U)));
    __Vtask_tb_disk__DOT__strobe__5297__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5297__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5297__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5297__ss));
    __Vfunc_tb_disk__DOT__WT__5298__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5298__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5298__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5298__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5298__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5298__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5299__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5299__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5299__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5299__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5299__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5299__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5297__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5297__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5297__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5300__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5300__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5300__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5300__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5300__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5300__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5297__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5301__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5301__data = (0x000000ffU 
                                                & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__5296__v)));
    __Vtask_tb_disk__DOT__strobe__5301__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5301__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5301__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5301__ss));
    __Vfunc_tb_disk__DOT__WT__5302__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5302__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5302__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5302__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5302__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5302__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5303__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5303__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5303__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5303__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5303__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5303__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5301__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5301__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5301__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5304__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5304__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5304__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5304__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5304__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5304__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5301__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__5305__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__5305__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__5305__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__5305__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__5306__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5306__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__5306__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5306__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5306__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5306__ss));
    __Vfunc_tb_disk__DOT__WT__5307__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5307__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5307__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5307__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5307__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5307__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5308__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5308__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5308__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5308__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5308__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5308__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5306__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5306__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5306__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5309__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5309__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5309__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5309__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5309__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5309__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5306__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5310__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5310__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__5310__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5310__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5310__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5310__ss));
    __Vfunc_tb_disk__DOT__WT__5311__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5311__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5311__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5311__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5311__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5311__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5312__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5312__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5312__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5312__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5312__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5312__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5310__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5310__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5310__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5313__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5313__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5313__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5313__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5313__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5313__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5310__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5314__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5314__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5314__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5314__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5314__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5314__ss));
    __Vfunc_tb_disk__DOT__WT__5315__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5315__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5315__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5315__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5315__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5315__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5316__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5316__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5316__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5316__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5316__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5316__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5314__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5314__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5314__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5317__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5317__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5317__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5317__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5317__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5317__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5314__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2506);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5318__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5305__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__5318__data = __Vtask_tb_disk__DOT__parc_micro__5305__b1;
    __Vtask_tb_disk__DOT__strobe__5318__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5318__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5318__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5318__ss));
    __Vfunc_tb_disk__DOT__WT__5319__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5319__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5319__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5319__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5319__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5319__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5320__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5320__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5320__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5320__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5320__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5320__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5318__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5318__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5318__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5321__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5321__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5321__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5321__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5321__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5321__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5318__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5322__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5305__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__5322__data = __Vtask_tb_disk__DOT__parc_micro__5305__b2;
    __Vtask_tb_disk__DOT__strobe__5322__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5322__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5322__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5322__ss));
    __Vfunc_tb_disk__DOT__WT__5323__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5323__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5323__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5323__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5323__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5323__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5324__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5324__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5324__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5324__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5324__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5324__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5322__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5322__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5322__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5325__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5325__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5325__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5325__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5325__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5325__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5322__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5326__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5305__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__5326__data = __Vtask_tb_disk__DOT__parc_micro__5305__b3;
    __Vtask_tb_disk__DOT__strobe__5326__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5326__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5326__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5326__ss));
    __Vfunc_tb_disk__DOT__WT__5327__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5327__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5327__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5327__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5327__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5327__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5328__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5328__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5328__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5328__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5328__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5328__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5326__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5326__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5326__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5329__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5329__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5329__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5329__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5329__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5329__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5326__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5330__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5305__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__5330__data = __Vtask_tb_disk__DOT__parc_micro__5305__b4;
    __Vtask_tb_disk__DOT__strobe__5330__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5330__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5330__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5330__ss));
    __Vfunc_tb_disk__DOT__WT__5331__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5331__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5331__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5331__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5331__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5331__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5332__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5332__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5332__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5332__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5332__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5332__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5330__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5330__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5330__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5333__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5333__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5333__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5333__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5333__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5333__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5330__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__5335__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5335__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5335__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5335__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5335__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5335__ss));
    __Vfunc_tb_disk__DOT__WT__5336__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5336__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5336__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5336__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5336__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5336__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5337__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5337__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5337__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5337__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5337__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5337__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5335__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5335__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5335__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5338__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5338__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5338__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5338__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5338__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5338__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5339__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5339__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5339__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5339__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5339__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5339__ss));
    __Vfunc_tb_disk__DOT__WT__5340__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5340__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5340__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5340__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5340__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5340__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5341__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5341__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5341__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5341__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5341__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5341__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5339__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5339__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5339__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5342__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5342__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5342__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5342__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5342__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5342__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5343__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5343__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5343__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5343__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5343__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5343__ss));
    __Vfunc_tb_disk__DOT__WT__5344__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5344__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5344__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5344__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5344__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5344__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5345__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5345__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5345__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5345__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5345__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5345__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5343__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5343__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5343__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5346__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5346__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5346__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5346__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5346__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5346__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5343__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__5347__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__5347__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5347__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5347__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5347__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__5347__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2518);
        __Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__5305__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__5305__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__5349__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__5349__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__5349__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__5349__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__5349__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__5350__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5350__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__5350__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5350__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5350__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5350__ss));
    __Vfunc_tb_disk__DOT__WT__5351__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5351__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5351__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5351__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5351__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5351__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5352__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5352__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5352__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5352__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5352__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5352__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5350__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5350__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5350__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5353__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5353__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5353__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5353__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5353__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5353__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5354__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5354__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__5354__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5354__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5354__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5354__ss));
    __Vfunc_tb_disk__DOT__WT__5355__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5355__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5355__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5355__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5355__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5355__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5356__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5356__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5356__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5356__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5356__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5356__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5354__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5354__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5354__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5357__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5357__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5357__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5357__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5357__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5357__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5358__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5358__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5358__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5358__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5358__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5358__ss));
    __Vfunc_tb_disk__DOT__WT__5359__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5359__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5359__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5359__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5359__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5359__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5360__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5360__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5360__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5360__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5360__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5360__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5358__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5358__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5358__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5361__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5361__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5361__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5361__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5361__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5361__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2506);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5362__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5349__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__5362__data = __Vtask_tb_disk__DOT__parc_micro__5349__b1;
    __Vtask_tb_disk__DOT__strobe__5362__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5362__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5362__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5362__ss));
    __Vfunc_tb_disk__DOT__WT__5363__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5363__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5363__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5363__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5363__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5363__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5364__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5364__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5364__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5364__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5364__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5364__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5362__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5362__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5362__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5365__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5365__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5365__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5365__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5365__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5365__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5366__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5349__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__5366__data = __Vtask_tb_disk__DOT__parc_micro__5349__b2;
    __Vtask_tb_disk__DOT__strobe__5366__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5366__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5366__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5366__ss));
    __Vfunc_tb_disk__DOT__WT__5367__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5367__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5367__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5367__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5367__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5367__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5368__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5368__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5368__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5368__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5368__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5368__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5366__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5366__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5366__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5369__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5369__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5369__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5369__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5369__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5369__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5370__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5349__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__5370__data = __Vtask_tb_disk__DOT__parc_micro__5349__b3;
    __Vtask_tb_disk__DOT__strobe__5370__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5370__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5370__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5370__ss));
    __Vfunc_tb_disk__DOT__WT__5371__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5371__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5371__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5371__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5371__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5371__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5372__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5372__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5372__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5372__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5372__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5372__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5370__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5370__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5370__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5373__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5373__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5373__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5373__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5373__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5373__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5374__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5349__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__5374__data = __Vtask_tb_disk__DOT__parc_micro__5349__b4;
    __Vtask_tb_disk__DOT__strobe__5374__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5374__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5374__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5374__ss));
    __Vfunc_tb_disk__DOT__WT__5375__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5375__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5375__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5375__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5375__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5375__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5376__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5376__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5376__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5376__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5376__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5376__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5374__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5374__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5374__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5377__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5377__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5377__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5377__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5377__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5377__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__5379__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5379__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5379__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5379__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5379__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5379__ss));
    __Vfunc_tb_disk__DOT__WT__5380__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5380__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5380__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5380__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5380__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5380__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5381__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5381__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5381__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5381__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5381__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5381__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5379__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5379__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5379__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5382__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5382__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5382__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5382__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5382__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5382__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5379__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5383__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5383__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5383__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5383__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5383__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5383__ss));
    __Vfunc_tb_disk__DOT__WT__5384__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5384__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5384__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5384__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5384__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5384__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5385__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5385__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5385__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5385__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5385__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5385__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5383__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5383__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5383__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5386__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5386__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5386__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5386__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5386__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5386__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5383__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5387__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5387__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5387__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5387__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5387__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5387__ss));
    __Vfunc_tb_disk__DOT__WT__5388__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5388__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5388__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5388__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5388__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5388__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5389__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5389__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5389__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5389__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5389__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5389__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5387__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5387__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5387__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5390__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5390__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5390__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5390__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5390__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5390__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5387__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__5391__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__5391__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5391__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5391__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5391__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__5391__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2518);
        __Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__5349__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__5349__b1,
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
    __Vtask_tb_disk__DOT__parc_run__5392__b4 = 0x43U;
    __Vtask_tb_disk__DOT__parc_run__5392__b3 = 0x4aU;
    __Vtask_tb_disk__DOT__parc_run__5392__b2 = 0xe1U;
    __Vtask_tb_disk__DOT__parc_run__5392__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_run__5392__b0 = 0x60U;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    __Vtask_tb_disk__DOT__strobe__5393__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5393__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5393__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5393__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5393__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5393__ss));
    __Vfunc_tb_disk__DOT__WT__5394__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5394__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5394__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5394__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5394__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5394__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5395__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5395__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5395__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5395__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5395__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5395__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5393__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5393__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5393__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5396__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5396__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5396__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5396__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5396__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5396__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5393__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2856);
        __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5397__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5397__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__5397__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5397__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5397__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5397__ss));
    __Vfunc_tb_disk__DOT__WT__5398__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5398__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5398__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5398__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5398__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5398__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5399__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5399__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5399__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5399__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5399__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5399__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5397__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5397__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5397__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5400__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5400__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5400__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5400__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5400__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5400__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5397__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2858);
        __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5401__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5401__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5401__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5401__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5401__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5401__ss));
    __Vfunc_tb_disk__DOT__WT__5402__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5402__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5402__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5402__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5402__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5402__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5403__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5403__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5403__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5403__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5403__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5403__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5401__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5401__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5401__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5404__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5404__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5404__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5404__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5404__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5404__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5401__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2860);
        __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5405__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__5392__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__5405__data = __Vtask_tb_disk__DOT__parc_run__5392__b1;
    __Vtask_tb_disk__DOT__strobe__5405__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5405__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5405__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5405__ss));
    __Vfunc_tb_disk__DOT__WT__5406__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5406__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5406__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5406__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5406__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5406__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5407__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5407__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5407__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5407__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5407__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5407__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5405__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5405__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5405__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5408__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5408__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5408__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5408__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5408__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5408__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5405__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5409__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__5392__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__5409__data = __Vtask_tb_disk__DOT__parc_run__5392__b2;
    __Vtask_tb_disk__DOT__strobe__5409__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5409__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5409__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5409__ss));
    __Vfunc_tb_disk__DOT__WT__5410__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5410__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5410__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5410__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5410__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5410__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5411__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5411__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5411__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5411__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5411__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5411__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5409__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5409__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5409__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5412__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5412__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5412__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5412__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5412__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5412__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5413__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__5392__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__5413__data = __Vtask_tb_disk__DOT__parc_run__5392__b3;
    __Vtask_tb_disk__DOT__strobe__5413__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5413__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5413__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5413__ss));
    __Vfunc_tb_disk__DOT__WT__5414__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5414__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5414__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5414__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5414__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5414__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5415__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5415__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5415__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5415__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5415__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5415__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5413__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5413__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5413__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5416__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5416__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5416__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5416__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5416__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5416__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5417__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__5392__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__5417__data = __Vtask_tb_disk__DOT__parc_run__5392__b4;
    __Vtask_tb_disk__DOT__strobe__5417__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5417__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5417__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5417__ss));
    __Vfunc_tb_disk__DOT__WT__5418__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5418__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5418__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5418__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5418__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5418__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5419__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5419__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5419__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5419__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5419__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5419__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5417__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5417__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5417__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5420__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5420__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5420__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5420__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5420__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5420__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__setss_n = 1U;
    __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2866);
        __Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
            = (__Vtask_tb_disk__DOT__parc_run__5392__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5421__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5421__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5421__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5421__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5421__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5421__ss));
    __Vfunc_tb_disk__DOT__WT__5422__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5422__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5422__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5422__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5422__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5422__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5423__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5423__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5423__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5423__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5423__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5423__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5421__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5421__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5421__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5424__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5424__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5424__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5424__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5424__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5424__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5421__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 = 0x00000258U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4660);
        tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 
            = (tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_disk:   THE READ BACK -- FifoRaddr %b -> %b, FifoEmpty=%b, DskData = %x\n",0,
                 4,tb_disk__DOT__fraddr_before,4,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__FifoEmpty,
                 16,(((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_00) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_01) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_02) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_03))) 
                       << 0x0000000cU) | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_04) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_05) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_06) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_07))) 
                                          << 8U)) | 
                     ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_08) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_09) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_10) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_11))) 
                       << 4U) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_12) 
                                   << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_13) 
                                             << 2U)) 
                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_14) 
                                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_15))))));
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                          == (IData)(tb_disk__DOT__fraddr_before))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4680: Assertion failed in %Ntb_disk: a DISKDATA read did not advance the FIFO read address (%b)\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),4,(IData)(tb_disk__DOT__fraddr_before));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4680, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...so a DISKDATA read POPS THE FIFO\n",0);
    }
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        if (VL_UNLIKELY(((IData)((7U != (7U & vlSelfRef.tb_disk__DOT__promseen2)))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4701: Assertion failed in %Ntb_disk: the PROM program counter did not walk its first steps (visited %b)\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),16,(0x0000ffffU 
                                             & vlSelfRef.tb_disk__DOT__promseen2));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4701, "", false);
        }
        if (VL_UNLIKELY(((1U & (~ (vlSelfRef.tb_disk__DOT__promseen2 
                                   >> 4U)))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4704: Assertion failed in %Ntb_disk: the sequencer never reached step 04, 'read data for first block' (visited %b)\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),16,(0x0000ffffU 
                                             & vlSelfRef.tb_disk__DOT__promseen2));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4704, "", false);
        }
        if (VL_UNLIKELY((((0U == vlSelfRef.tb_disk__DOT__n_shiftin) 
                          | (0U == vlSelfRef.tb_disk__DOT__n_cecc))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4707: Assertion failed in %Ntb_disk: the sequencer never opened the data path: ShiftIn %0d ComputeECC %0d\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_shiftin,
                         32,vlSelfRef.tb_disk__DOT__n_cecc);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4707, "", false);
        }
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_shmove)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4710: Assertion failed in %Ntb_disk: the input shift register never moved\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4710, "", false);
        }
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_irf)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4720: Assertion failed in %Ntb_disk: InRegFull never rose -- no complete word was assembled\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4720, "", false);
        }
        if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__fwaddr_last2) 
                          == (IData)(vlSelfRef.tb_disk__DOT__fwaddr_first))))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4722: Assertion failed in %Ntb_disk: the FIFO write address never advanced (%b) -- nothing was stored\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),4,(IData)(vlSelfRef.tb_disk__DOT__fwaddr_first));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4722, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...and WORDS REACH THE FIFO: %0d assembled, write address %b -> %b\ntb_disk:   ...so THE DISK READ RUNS: PROM PC walked %b, ShiftIn %0d, ComputeECC %0d, ShiftReg = %x\n",0,
                     32,vlSelfRef.tb_disk__DOT__n_irf,
                     4,(IData)(vlSelfRef.tb_disk__DOT__fwaddr_first),
                     4,vlSelfRef.tb_disk__DOT__fwaddr_last2,
                     16,(0x0000ffffU & vlSelfRef.tb_disk__DOT__promseen2),
                     32,vlSelfRef.tb_disk__DOT__n_shiftin,
                     32,vlSelfRef.tb_disk__DOT__n_cecc,
                     16,(IData)(vlSelfRef.tb_disk__DOT__shreg_last));
    }
    VL_WRITEF_NX("tb_disk:   PROM PROGRAM COUNTER -- visited %b (bit n = address n), last = %0#\ntb_disk:   THE FIFO -- fed word %x, ShiftReg = %x | InRegFull edges %0d, FifoWaddr %b -> %b | DskData = %x\n",0,
                 16,(0x0000ffffU & vlSelfRef.tb_disk__DOT__promseen2),
                 5,(IData)(vlSelfRef.tb_disk__DOT__promaddr2_last),
                 16,vlSelfRef.tb_disk__DOT__FEDWORD,
                 16,(IData)(vlSelfRef.tb_disk__DOT__shreg_last),
                 32,vlSelfRef.tb_disk__DOT__n_irf,4,
                 (IData)(vlSelfRef.tb_disk__DOT__fwaddr_first),
                 4,vlSelfRef.tb_disk__DOT__fwaddr_last2,
                 16,(((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_00) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_01) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_02) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_03))) 
                       << 0x0000000cU) | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_04) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_05) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_06) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_07))) 
                                          << 8U)) | 
                     ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_08) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_09) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_10) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_11))) 
                       << 4U) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_12) 
                                   << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_13) 
                                             << 2U)) 
                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_14) 
                                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_15))))));
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_co)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4754: Assertion failed in %Ntb_disk: the bit counter never reached terminal count after the tag write\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4754, "", false);
        }
        if (VL_UNLIKELY((VL_GTS_III(32, 2U, vlSelfRef.tb_disk__DOT__n_wclk)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4756: Assertion failed in %Ntb_disk: only %0d WordClock' edges -- the sequencer cannot step\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_wclk);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4756, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...so the counter RUNS: CO' asserted %0d, WordClock' edges %0d\n",0,
                     32,vlSelfRef.tb_disk__DOT__n_co,
                     32,vlSelfRef.tb_disk__DOT__n_wclk);
    }
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000020U, vlSelfRef.tb_disk__DOT__n_bclkb)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4767: Assertion failed in %Ntb_disk: 32 cable bit periods gave only %0d BitClock'B edges\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_bclkb);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4767, "", false);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_shld)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4769: Assertion failed in %Ntb_disk: ShiftRegLd' went low on %0d samples -- the chain was loading, not shifting\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_shld);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4769, "", false);
    }
    if (((0U == vlSelfRef.tb_disk__DOT__n_shiftin) 
         & (0U == vlSelfRef.tb_disk__DOT__n_cecc))) {
        VL_WRITEF_NX("tb_disk:   ...so the CLOCK runs and the register shifts, but ShiftIn and ComputeECC are both closed: the data waits on the b20 sequencer (open)\n",0);
    } else {
        VL_WRITEF_NX("tb_disk:   (note) the sequencer gates opened without a loaded command -- re-read this\n",0);
    }
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)) 
           & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10));
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn = 0U;
    tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46 = 0x00000040U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4788);
        tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46 
            = (tb_disk__DOT__unnamedblk1_47__DOT____Vrepeat46 
               - (IData)(1U));
    }
    if (VL_UNLIKELY(((1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd)) 
                            | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4792: Assertion failed in %Ntb_disk: the drive was released but NotReady=%b NotOnLine=%b -- the level is stuck, not tracking\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd),
                     1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4792, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...and released, the controller sees it go: NotReady=%b NotOnLine=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine));
    if (VL_UNLIKELY(((1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd) 
                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd)) 
                               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4805: Assertion failed in %Ntb_disk: a drive-status line reads ASSERTED with no drive attached: 1111%b1%b1%b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd),
                     1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4805, "", false);
    }
    if (VL_UNLIKELY(((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd) 
                               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4811: Assertion failed in %Ntb_disk: a per-drive line reads ASSERTED with no drive attached\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4811, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...and all 17 active-low cable lines idle DEASSERTED: no drive is fabricated\ntb_disk:   ...first Cont assertion at sample %0d of %0d, board selected there: %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__cont_first,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__cont_first_sel);
    if (VL_TESTPLUSARGS_I("slowio"s)) {
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_sel)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4835: Assertion failed in %Ntb_disk: DskEth never selected -- no command reached the board at 010B\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4835, "", false);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_sel 
                          != vlSelfRef.tb_disk__DOT__n_sel_free)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4837: Assertion failed in %Ntb_disk: DskEth selected on %0d samples but only %0d with the processor free\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_sel,
                         32,vlSelfRef.tb_disk__DOT__n_sel_free);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4837, "", false);
        }
        if ((VL_TESTPLUSARGS_I("ram"s) || VL_TESTPLUSARGS_I("ram16"s))) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_ram)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4846: Assertion failed in %Ntb_disk: TIOA=Ram' never asserted -- a write to 013B reached no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4846, "", false);
            }
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_r_muff)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4861: Assertion failed in %Ntb_disk: a write to 013B also selected Muff %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_muff);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4861, "", false);
            }
            if (VL_UNLIKELY((((! VL_TESTPLUSARGS_I("ram16"s)) 
                              & (((0U != vlSelfRef.tb_disk__DOT__n_r_cont) 
                                  | (0U != vlSelfRef.tb_disk__DOT__n_r_tag)) 
                                 | (0U != vlSelfRef.tb_disk__DOT__n_r_data)))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4864: Assertion failed in %Ntb_disk: a write to 013B also selected Cont %0d Tag %0d Data %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_cont,
                             32,vlSelfRef.tb_disk__DOT__n_r_tag,
                             32,vlSelfRef.tb_disk__DOT__n_r_data);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4864, "", false);
            }
            if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("ram16"s) 
                              && (0U == vlSelfRef.tb_disk__DOT__n_r_data))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4867: Assertion failed in %Ntb_disk: +ram16 never addressed DISKDATA, so nothing was read back\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4867, "", false);
            }
            if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("ram16"s) 
                              && (0U == vlSelfRef.tb_disk__DOT__n_r_cont))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4869: Assertion failed in %Ntb_disk: +ram16 never wrote DISKCONTROL, so the format-RAM address was never zeroed\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4869, "", false);
            }
            if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("ram16"s) 
                              && (0U == vlSelfRef.tb_disk__DOT__n_r_tag))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4871: Assertion failed in %Ntb_disk: +ram16 never wrote DISKTAG, so the bit counter was never released\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4871, "", false);
            }
        } else if (VL_TESTPLUSARGS_I("muff"s)) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_muff)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4875: Assertion failed in %Ntb_disk: TIOA=Muff' never asserted -- a write to 011B reached no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4875, "", false);
            }
            if (VL_UNLIKELY((((((0U != vlSelfRef.tb_disk__DOT__n_r_cont) 
                                | (0U != vlSelfRef.tb_disk__DOT__n_r_data)) 
                               | (0U != vlSelfRef.tb_disk__DOT__n_r_ram)) 
                              | (0U != vlSelfRef.tb_disk__DOT__n_r_tag))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4877: Assertion failed in %Ntb_disk: a write to 011B also selected Cont %0d Data %0d Ram %0d Tag %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_cont,
                             32,vlSelfRef.tb_disk__DOT__n_r_data,
                             32,vlSelfRef.tb_disk__DOT__n_r_ram,
                             32,vlSelfRef.tb_disk__DOT__n_r_tag);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4877, "", false);
            }
        } else if (VL_TESTPLUSARGS_I("tag"s)) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_tag)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4884: Assertion failed in %Ntb_disk: TIOA=Tag' never asserted -- a write to 014B reached no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4884, "", false);
            }
            if (VL_UNLIKELY((((((0U != vlSelfRef.tb_disk__DOT__n_r_cont) 
                                | (0U != vlSelfRef.tb_disk__DOT__n_r_muff)) 
                               | (0U != vlSelfRef.tb_disk__DOT__n_r_data)) 
                              | (0U != vlSelfRef.tb_disk__DOT__n_r_ram))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4886: Assertion failed in %Ntb_disk: a write to 014B also selected Cont %0d Muff %0d Data %0d Ram %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_cont,
                             32,vlSelfRef.tb_disk__DOT__n_r_muff,
                             32,vlSelfRef.tb_disk__DOT__n_r_data,
                             32,vlSelfRef.tb_disk__DOT__n_r_ram);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4886, "", false);
            }
        } else {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_cont)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4890: Assertion failed in %Ntb_disk: TIOA=Cont' never asserted -- the write reached the board but no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4890, "", false);
            }
            if (VL_UNLIKELY((((((0U != vlSelfRef.tb_disk__DOT__n_r_muff) 
                                | (0U != vlSelfRef.tb_disk__DOT__n_r_data)) 
                               | (0U != vlSelfRef.tb_disk__DOT__n_r_ram)) 
                              | (0U != vlSelfRef.tb_disk__DOT__n_r_tag))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4892: Assertion failed in %Ntb_disk: a write to 010B also selected Muff %0d Data %0d Ram %0d Tag %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_muff,
                             32,vlSelfRef.tb_disk__DOT__n_r_data,
                             32,vlSelfRef.tb_disk__DOT__n_r_ram,
                             32,vlSelfRef.tb_disk__DOT__n_r_tag);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4892, "", false);
            }
        }
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tioa_out10)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4903: Assertion failed in %Ntb_disk: no IOBout strobe found TIOA = %o: the address never survives to the data\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),8,(IData)(vlSelfRef.tb_disk__DOT__want_tioa));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4903, "", false);
        }
        if ((VL_TESTPLUSARGS_I("input"s) || VL_TESTPLUSARGS_I("ram16"s))) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_iobin)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4916: Assertion failed in %Ntb_disk: bIOin' never asserted -- the Pd<-Input in the loop never reached the board\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4916, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_ioen)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4918: Assertion failed in %Ntb_disk: the board never drove IOB back: enable never asserted during a read\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4918, "", false);
            }
        } else {
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_iobin)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4921: Assertion failed in %Ntb_disk: bIOin' asserted %0d times with no Pd<-Input in the loop\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_iobin);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4921, "", false);
            }
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_ioen)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4923: Assertion failed in %Ntb_disk: the board drove IOB on %0d samples with nobody reading it -- a bus fight\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_ioen);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4923, "", false);
            }
        }
        if (VL_TESTPLUSARGS_I("muff"s)) {
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_crc_edge)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4928: Assertion failed in %Ntb_disk: a write to 011B clocked the CONTROL register %0d times\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_crc_edge);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4928, "", false);
            }
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_tag_edge)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4930: Assertion failed in %Ntb_disk: a write to 011B fired the SEEK strobe %0d times\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_tag_edge);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4930, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_clridx)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4938: Assertion failed in %Ntb_disk: a DISKMUFF write with IOB.04 set never asserted ClearIndexTW\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4938, "", false);
            }
            if (VL_UNLIKELY((VL_GTES_III(32, vlSelfRef.tb_disk__DOT__n_idxtw_run, vlSelfRef.tb_disk__DOT__n_tot)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4940: Assertion failed in %Ntb_disk: ClearIndexTW fired %0d times but IndexTW stayed high on all %0d samples\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_clridx,
                             32,vlSelfRef.tb_disk__DOT__n_tot);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4940, "", false);
            }
        } else if (((! VL_TESTPLUSARGS_I("ram"s)) & 
                    (! VL_TESTPLUSARGS_I("ram16"s)))) {
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_clridx)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4947: Assertion failed in %Ntb_disk: ClearIndexTW asserted %0d times with no DISKMUFF write\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_clridx);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4947, "", false);
            }
            if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_idxtw_run 
                              != vlSelfRef.tb_disk__DOT__n_tot)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4949: Assertion failed in %Ntb_disk: IndexTW dropped without a DISKMUFF write (%0d of %0d)\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_idxtw_run,
                             32,vlSelfRef.tb_disk__DOT__n_tot);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4949, "", false);
            }
        }
        if (VL_UNLIKELY((VL_TESTPLUSARGS_I("tdata"s)))) {
            if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_tioa_out10 
                              != vlSelfRef.tb_disk__DOT__n_iobout)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4960: Assertion failed in %Ntb_disk: only %0d of %0d strobes carried the address\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_tioa_out10,
                             32,vlSelfRef.tb_disk__DOT__n_iobout);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4960, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_iob_lit)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4962: Assertion failed in %Ntb_disk: no strobe carried the FF literal -- the data is not independent of the address\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4962, "", false);
            }
            VL_WRITEF_NX("tb_disk:   ...so address and data are INDEPENDENT: all %0d strobes addressed, %0d carried the literal\n",0,
                         32,vlSelfRef.tb_disk__DOT__n_tioa_out10,
                         32,vlSelfRef.tb_disk__DOT__n_iob_lit);
        }
        if (VL_UNLIKELY((VL_TESTPLUSARGS_I("tlit"s)))) {
            if (VL_UNLIKELY((VL_GTS_III(32, 0x00000064U, vlSelfRef.tb_disk__DOT__n_r_cont)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4974: Assertion failed in %Ntb_disk: with T loaded from an FF literal the board selected only %0d times\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_cont);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4974, "", false);
            }
            if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("read"s) 
                              && VL_GTS_III(32, 0x0000000aU, vlSelfRef.tb_disk__DOT__n_crc_edge))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4976: Assertion failed in %Ntb_disk: the control register clocked only %0d times -- the loop is not sustaining writes\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_crc_edge);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4976, "", false);
            }
            VL_WRITEF_NX("tb_disk:   ...so an FF literal in T sustains the loop: %0d selects, %0d control-register writes\n",0,
                         32,vlSelfRef.tb_disk__DOT__n_r_cont,
                         32,vlSelfRef.tb_disk__DOT__n_crc_edge);
        }
        if (VL_UNLIKELY((VL_TESTPLUSARGS_I("qaddr"s)))) {
            if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_tioa_out10 
                              != vlSelfRef.tb_disk__DOT__n_iobout)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4989: Assertion failed in %Ntb_disk: with the address in Q only %0d of %0d strobes carried it -- it should be all of them\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_tioa_out10,
                             32,vlSelfRef.tb_disk__DOT__n_iobout);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 4989, "", false);
            }
            VL_WRITEF_NX("tb_disk:   ...so with the address in Q, ALL %0d strobes carry it (from T it was 32 of 960)\n",0,
                         32,vlSelfRef.tb_disk__DOT__n_tioa_out10);
        }
        if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram"s)))) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_ramcl)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5010: Assertion failed in %Ntb_disk: DISKRAM was addressed %0d times but the format-RAM address never stepped\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_ram);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 5010, "", false);
            }
            VL_WRITEF_NX("tb_disk:   ...so a DISKRAM write STEPS the format-RAM address (%0d edges while addressed)\n",0,
                         32,vlSelfRef.tb_disk__DOT__n_ramcl);
            if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("qaddr"s) 
                              && (0U == vlSelfRef.tb_disk__DOT__n_lastram))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5018: Assertion failed in %Ntb_disk: the format-RAM address stepped %0d times but never reached its LAST address\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_ramcl);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 5018, "", false);
            }
        } else if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_ramcl)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5020: Assertion failed in %Ntb_disk: the format-RAM address stepped %0d times with no DISKRAM write\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_ramcl);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 5020, "", false);
        }
        if (VL_UNLIKELY((VL_TESTPLUSARGS_I("read"s)))) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_rdblk)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5024: Assertion failed in %Ntb_disk: a Read op was loaded but ReadBlock never asserted\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 5024, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_actv)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5026: Assertion failed in %Ntb_disk: SetDebugMode + a transfer op did not make the controller Active\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 5026, "", false);
            }
            VL_WRITEF_NX("tb_disk:   ...so a REAL COMMAND loads and STARTS: ReadBlock and Active both up\n",0);
        } else if ((! VL_TESTPLUSARGS_I("ram16"s))) {
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_rdblk)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5032: Assertion failed in %Ntb_disk: ReadBlock asserted %0d times with no Read op loaded\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_rdblk);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 5032, "", false);
            }
        }
        if ((1U & (~ (IData)(((VL_TESTPLUSARGS_I("ram"s) 
                               || VL_TESTPLUSARGS_I("ram16"s)) 
                              || VL_TESTPLUSARGS_I("muff"s)))))) {
            if (VL_TESTPLUSARGS_I("tag"s)) {
                if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tag_edge)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5046: Assertion failed in %Ntb_disk: Tag_IOB never fired -- DISKTAG was addressed but the seek strobe never came\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name());
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5046, "", false);
                }
                if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_tag_edge 
                                  != vlSelfRef.tb_disk__DOT__n_tag_free)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5048: Assertion failed in %Ntb_disk: Tag_IOB fired %0d times but only %0d while DISKTAG was addressed\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 32,vlSelfRef.tb_disk__DOT__n_tag_edge,
                                 32,vlSelfRef.tb_disk__DOT__n_tag_free);
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5048, "", false);
                }
                if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tagclk)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5051: Assertion failed in %Ntb_disk: TagClock never clocked -- Tag_IOB fired but d16 did not pass it to the drive\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name());
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5051, "", false);
                }
                if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_crc_edge)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5053: Assertion failed in %Ntb_disk: a write to 014B also clocked the CONTROL register %0d times\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 32,vlSelfRef.tb_disk__DOT__n_crc_edge);
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5053, "", false);
                }
            } else {
                if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_crc_edge)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5059: Assertion failed in %Ntb_disk: ControlRegCl never fired -- DISKCONTROL was addressed but the register never clocked\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name());
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5059, "", false);
                }
                if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_crc_edge 
                                  != vlSelfRef.tb_disk__DOT__n_crc_free)))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5061: Assertion failed in %Ntb_disk: ControlRegCl fired %0d times but only %0d while DISKCONTROL was addressed\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 32,vlSelfRef.tb_disk__DOT__n_crc_edge,
                                 32,vlSelfRef.tb_disk__DOT__n_crc_free);
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5061, "", false);
                }
                if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                         >> 2U)) != 
                                  (1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                         >> 1U)))))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5079: Assertion failed in %Ntb_disk: DebugMode %b does not follow bIOB.06 %b (B[6] = SetDebugMode)\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                          >> 2U)),1,
                                 (1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                        >> 1U)));
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5079, "", false);
                }
                if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                         >> 1U)) != 
                                  (1U & (IData)(vlSelfRef.tb_disk__DOT__iobits)))))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5082: Assertion failed in %Ntb_disk: BlockTillIndex %b does not follow bIOB.07 %b (B[7] = SetBlockTillIndex)\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                          >> 1U)),1,
                                 (1U & (IData)(vlSelfRef.tb_disk__DOT__iobits)));
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5082, "", false);
                }
                if (VL_UNLIKELY((((1U & (IData)(vlSelfRef.tb_disk__DOT__ctl_final)) 
                                  == (1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                            >> 2U)))))) {
                    VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5085: Assertion failed in %Ntb_disk: EnableRun %b is not the COMPLEMENT of bIOB.05 %b (B[5] = ClearEnableRun)\n",0,
                                 64,VL_TIME_UNITED_Q(1),
                                 -12,vlSymsp->name(),
                                 1,(1U & (IData)(vlSelfRef.tb_disk__DOT__ctl_final)),
                                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                          >> 2U)));
                    VL_STOP_MT("verilog/verilator/tb_disk.sv", 5085, "", false);
                }
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
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5100: Assertion failed in %Ntb_disk: Output<- never asserted IOBout -- the instruction did not decode\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 5100, "", false);
    }
    if (VL_UNLIKELY((((! VL_TESTPLUSARGS_I("slowio"s)) 
                      & (0U != vlSelfRef.tb_disk__DOT__n_iobout))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5102: Assertion failed in %Ntb_disk: IOBout asserted %0d times with no slow-I/O loop loaded\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_iobout);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 5102, "", false);
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
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5131: Assertion failed in %Ntb_disk: DskEth has no local clock -- is CLK.display' driven?\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 5131, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_wdht)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) TWReq.03 is DispY's head task, not DskEth's\n",0);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_twr11)))) {
        VL_WRITEF_NX("tb_disk: OPEN -- TWReq.11 never asserted; WakeDWT needs a display list to fetch\n",0);
    }
    __Vfunc_tb_disk__DOT__WT__5425__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__5425__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5425__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5425__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5425__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47 
        = __Vfunc_tb_disk__DOT__WT__5425__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             5170);
        tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47 
            = (tb_disk__DOT__unnamedblk1_48__DOT____Vrepeat47 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_asserted = 0U;
    VL_WRITEF_NX("tb_disk: WORD TASK -- with channel A's WCB flags set and its FIFO not full, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_asserted);
    __Vfunc_tb_disk__DOT__WT__5426__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__5426__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5426__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5426__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5426__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48 
        = __Vfunc_tb_disk__DOT__WT__5426__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             5176);
        tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48 
            = (tb_disk__DOT__unnamedblk1_49__DOT____Vrepeat48 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_full = 0U;
    VL_WRITEF_NX("tb_disk:            ...and with the FIFO FULL, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_full);
    if (VL_UNLIKELY((((IData)(tb_disk__DOT__dwt_asserted) 
                      == (IData)(tb_disk__DOT__dwt_full))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) DWTWantsProc is a DispY signal\n",0);
    }
    __Vtask_tb_disk__DOT__set_cpreg_plain__5427__v = 0xf800U;
    __Vtask_tb_disk__DOT__strobe__5428__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5428__data = (0x000000ffU 
                                                & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__5427__v) 
                                                   >> 8U));
    __Vtask_tb_disk__DOT__strobe__5428__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5428__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5428__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5428__ss));
    __Vfunc_tb_disk__DOT__WT__5429__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5429__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5429__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5429__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5429__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5429__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5430__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5430__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5430__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5430__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5430__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5430__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5428__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5428__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5428__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5431__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5431__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5431__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5431__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5431__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5431__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5432__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5432__data = (0x000000ffU 
                                                & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__5427__v));
    __Vtask_tb_disk__DOT__strobe__5432__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5432__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5432__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5432__ss));
    __Vfunc_tb_disk__DOT__WT__5433__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5433__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5433__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5433__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5433__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5433__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5434__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5434__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5434__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5434__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5434__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5434__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5432__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5432__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5432__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5435__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5435__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5435__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5435__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5435__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5435__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__b4 = 0xc0U;
    __Vtask_tb_disk__DOT__parc_micro__5436__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__5436__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__5436__b1 = 3U;
    __Vtask_tb_disk__DOT__parc_micro__5436__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__5437__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5437__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__5437__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5437__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5437__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5437__ss));
    __Vfunc_tb_disk__DOT__WT__5438__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5438__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5438__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5438__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5438__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5438__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5439__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5439__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5439__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5439__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5439__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5439__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5437__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5437__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5437__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5440__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5440__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5440__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5440__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5440__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5440__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5441__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5441__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__5441__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5441__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5441__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5441__ss));
    __Vfunc_tb_disk__DOT__WT__5442__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5442__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5442__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5442__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5442__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5442__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5443__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5443__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5443__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5443__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5443__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5443__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5441__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5441__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5441__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5444__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5444__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5444__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5444__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5444__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5444__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5441__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5445__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5445__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5445__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5445__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5445__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5445__ss));
    __Vfunc_tb_disk__DOT__WT__5446__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5446__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5446__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5446__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5446__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5446__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5447__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5447__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5447__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5447__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5447__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5447__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5445__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5445__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5445__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5448__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5448__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5448__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5448__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5448__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5448__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5445__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2506);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5449__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5436__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__5449__data = __Vtask_tb_disk__DOT__parc_micro__5436__b1;
    __Vtask_tb_disk__DOT__strobe__5449__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5449__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5449__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5449__ss));
    __Vfunc_tb_disk__DOT__WT__5450__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5450__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5450__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5450__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5450__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5450__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5451__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5451__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5451__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5451__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5451__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5451__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5449__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5449__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5449__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5452__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5452__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5452__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5452__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5452__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5452__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5449__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5453__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5436__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__5453__data = __Vtask_tb_disk__DOT__parc_micro__5436__b2;
    __Vtask_tb_disk__DOT__strobe__5453__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5453__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5453__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5453__ss));
    __Vfunc_tb_disk__DOT__WT__5454__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5454__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5454__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5454__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5454__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5454__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5455__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5455__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5455__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5455__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5455__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5455__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5453__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5453__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5453__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5456__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5456__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5456__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5456__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5456__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5456__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5457__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5436__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__5457__data = __Vtask_tb_disk__DOT__parc_micro__5436__b3;
    __Vtask_tb_disk__DOT__strobe__5457__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5457__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5457__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5457__ss));
    __Vfunc_tb_disk__DOT__WT__5458__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5458__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5458__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5458__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5458__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5458__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5459__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5459__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5459__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5459__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5459__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5459__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5457__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5457__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5457__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5460__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5460__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5460__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5460__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5460__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5460__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5461__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5436__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__5461__data = __Vtask_tb_disk__DOT__parc_micro__5436__b4;
    __Vtask_tb_disk__DOT__strobe__5461__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5461__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5461__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5461__ss));
    __Vfunc_tb_disk__DOT__WT__5462__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5462__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5462__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5462__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5462__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5462__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5463__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5463__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5463__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5463__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5463__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5463__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5461__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5461__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5461__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5464__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5464__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5464__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5464__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5464__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5464__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__5466__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5466__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5466__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5466__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5466__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5466__ss));
    __Vfunc_tb_disk__DOT__WT__5467__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5467__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5467__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5467__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5467__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5467__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5468__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5468__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5468__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5468__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5468__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5468__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5466__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5466__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5466__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5469__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5469__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5469__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5469__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5469__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5469__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5466__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5470__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5470__data = 1U;
    __Vtask_tb_disk__DOT__strobe__5470__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5470__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5470__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5470__ss));
    __Vfunc_tb_disk__DOT__WT__5471__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5471__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5471__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5471__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5471__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5471__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5472__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5472__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5472__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5472__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5472__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5472__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5470__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5470__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5470__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5473__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5473__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5473__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5473__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5473__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5473__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5470__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5474__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5474__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5474__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5474__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5474__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5474__ss));
    __Vfunc_tb_disk__DOT__WT__5475__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5475__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5475__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5475__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5475__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5475__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5476__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5476__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5476__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5476__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5476__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5476__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5474__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5474__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5474__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5477__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5477__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5477__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5477__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5477__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5477__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5474__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__5478__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__5478__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5478__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5478__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5478__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__5478__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2518);
        __Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__5436__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__5436__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__5480__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__5480__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__5480__b2 = 0x0fU;
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__5480__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__5480__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__5481__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5481__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__5481__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5481__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5481__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5481__ss));
    __Vfunc_tb_disk__DOT__WT__5482__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5482__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5482__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5482__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5482__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5482__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5483__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5483__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5483__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5483__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5483__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5483__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5481__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5481__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5481__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5484__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5484__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5484__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5484__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5484__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5484__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5485__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__5485__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__5485__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5485__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5485__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5485__ss));
    __Vfunc_tb_disk__DOT__WT__5486__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5486__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5486__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5486__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5486__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5486__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5487__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5487__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5487__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5487__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5487__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5487__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5485__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5485__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5485__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5488__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5488__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5488__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5488__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5488__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5488__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5485__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5489__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__5489__data = 0U;
    __Vtask_tb_disk__DOT__strobe__5489__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5489__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5489__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5489__ss));
    __Vfunc_tb_disk__DOT__WT__5490__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5490__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5490__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5490__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5490__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5490__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5491__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5491__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5491__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5491__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5491__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5491__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5489__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5489__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5489__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5492__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5492__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5492__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5492__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5492__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5492__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5489__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2506);
        __Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__5480__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5493__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5480__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__5493__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__5480__b1;
    __Vtask_tb_disk__DOT__strobe__5493__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5493__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5493__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5493__ss));
    __Vfunc_tb_disk__DOT__WT__5494__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5494__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5494__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5494__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5494__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5494__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5495__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5495__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5495__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5495__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5495__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5495__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5493__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5493__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5493__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5496__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5496__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5496__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5496__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5496__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5496__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5493__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5497__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5480__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__5497__data = __Vtask_tb_disk__DOT__parc_micro__5480__b2;
    __Vtask_tb_disk__DOT__strobe__5497__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5497__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5497__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5497__ss));
    __Vfunc_tb_disk__DOT__WT__5498__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5498__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5498__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5498__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5498__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5498__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5499__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5499__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5499__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5499__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5499__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5499__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5497__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5497__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5497__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5500__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5500__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5500__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5500__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5500__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5500__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5497__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5501__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5480__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__5501__data = __Vtask_tb_disk__DOT__parc_micro__5480__b3;
    __Vtask_tb_disk__DOT__strobe__5501__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5501__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5501__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5501__ss));
    __Vfunc_tb_disk__DOT__WT__5502__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5502__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5502__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5502__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5502__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5502__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5503__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5503__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5503__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5503__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5503__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5503__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5501__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5501__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5501__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5504__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5504__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5504__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5504__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5504__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5504__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5501__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__5505__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__5480__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__5505__data = __Vtask_tb_disk__DOT__parc_micro__5480__b4;
    __Vtask_tb_disk__DOT__strobe__5505__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5505__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__5505__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__5505__ss));
    __Vfunc_tb_disk__DOT__WT__5506__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5506__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5506__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5506__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5506__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5506__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__5507__n = 6U;
    __Vfunc_tb_disk__DOT__WT__5507__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5507__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5507__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5507__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__5507__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1925);
        __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__5505__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__5505__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__5505__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__5508__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5508__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5508__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5508__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5508__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__5508__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1931);
        __Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__5505__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__ss = 1U;
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__data = 1U;
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__fn = 0U;
    vlSelf->__Vtask_tb_disk__DOT__strobe__5510__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    vlSelf->__Vtask_tb_disk__DOT__strobe__5510__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    vlSelf->__Vtask_tb_disk__DOT__strobe__5510__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__data) 
                                      << 1U) | (IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__ss));
    __Vfunc_tb_disk__DOT__WT__5511__n = 4U;
    __Vfunc_tb_disk__DOT__WT__5511__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5511__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5511__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5511__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__5510__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__5511__Vfuncout;
    co_return;}
