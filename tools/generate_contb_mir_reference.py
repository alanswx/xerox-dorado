#!/usr/bin/env python3
"""Extract ContB MIR, its own clock fanout and original maintenance enables.

Preserve separate ASEL.0 copies, all unused package halves, CBHold E43,
CLKEnable'a's actual board input, and both boards' rMIRa contributions.
IM data/parity, BNT comparison, DMD and upstream enables remain boundaries.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_processor_capture_reference import graph,ROOT
from sil_netlist import load_board

BOARD='ContB-Rev-Cd'
MIR=['b23','b24','c24','g24','h24','i24','j24','k24','h01','i01','j01']
SEQ={**{p:(2*i,2)for i,p in enumerate(MIR)},'c04':(22,6),'g03':(28,6),'h03':(34,6)}
COMB=['b01','c22','d23','d24','l01','g13','c20','i20','j05','g04','k01','d05','h04','i03','i04','l03','k02']
DATA=['ASEL.0','ASEL.1','ASEL.2','BSEL.0','BSEL.1','BSEL.2','LC.0','LC.1','LC.2',
      'ALUF.0','ALUF.1','ALUF.2','ALUF.3','RSTK.0','RSTK.1','RSTK.2','RSTK.3','IMLH']
FLAGS=["ASEL.0'a","ASEL.0'","ASEL.1'","ASEL.2'","BSEL.0'","BSEL.1'","BSEL.2'",
       'LC.0','LC.1','LC.2','ALUF.0','ALUF.1','ALUF.2','ALUF.3','RSTK.0','RSTK.1','RSTK.2','RSTK.3',
       "ASEL.0'mem",'IMLH','StopMIRClk','rMIRa','RepeatCurCc','StopMIRClkEn','IMLHPEenable','IMRHPEenable']


def plan(root):
    bindings={"CLK.cb'":'pins[0]',"CLKEnable'a":'pins[1]',"StartCycle'a":'pins[2]',
              'CBHold':'pins[3]',"BNTGtCT'a":'pins[4]','UseDMD':'pins[5]',
              "IMLHPE'":'pins[7]',"IMRHPE'":'pins[8]','CRamClock':'pins[9]',
              "CPAddr.0'":'pins[10]',"CPAddr.1'":'pins[11]',"CPAddr.2'":'pins[12]',"CPStrb'":'pins[13]',
              **{f'CPOut.{i}':f'pins[{14+i}]'for i in range(4)},'CPOut.8':'pins[18]',
              **{f'DMD.{i:02}':f'pins[{16+i}]'for i in range(3,12)},
              **{'d'+n:f'pins[{28+i}]'for i,n in enumerate(DATA)},"UseDMDEnable'":'pins[46]'}
    clocks=[f'@{p}.{h}'for p in MIR for h in ['ca','cb']]+["ManClk.0'","ManClk.7'","ManClk.1'"]
    spec=dict(bn=BOARD,comb=COMB,seq=SEQ,module='contb_mir_reference',bindings={},clock_taps=clocks,flag_taps=FLAGS)
    external=graph(root,out=None,inspect=True,**spec)
    extras={n:47+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
    spec['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
    return spec,extras


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False);spec,extras=plan(root);r=graph(root,out=out,**spec)
    width=47+len(extras);p=out/'contb_mir_reference.sv';s=p.read_text()
    assert s.count('input wire [27:0] pins')==1;s=s.replace('input wire [27:0] pins',f'input wire [{width-1}:0] pins')
    # Original .wl gives the on-board h03 driver; preserve the additional
    # ContA j02.14 driver on this named backplane wire explicitly.
    signal=None
    for line in s.splitlines():
        if line.endswith('; // rMIRa'):signal=line.split()[1].removesuffix(';')
    assert signal
    before=' assign '+signal+'='+signal+'_h03_2;';assert s.count(before)==1
    s=s.replace(before,' assign '+signal+'='+signal+'_h03_2 | pins[6];')
    b=load_board(str(root/'chm/sil'/(BOARD+'.dm!1_')/(BOARD+'.wl')))
    ca=load_board(str(root/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
    assert {(x['pkg'],x['pin'])for x in b.nets['rMIRa']['pins']if x['dir']=='out'}=={('h03',2)}
    assert {(x['pkg'],x['pin'])for x in ca.nets['rMIRa']['pins']if x['dir']=='out'}=={('j02',14)}
    assert ca.nets['rMIRa']['backplane']==b.nets['rMIRa']['backplane']=={'C177'}
    assert b.nets['CBHold']['backplane']=={'E43'}
    p.write_text(s)
    r.update(scope=__doc__,pin_count=width,extra_boundaries=extras,backplane_reset=dict(net='rMIRa',local='ContB h03.2',remote='ContA j02.14 / pins[6]',resolution='wired OR'),native_integrated=False)
    (out/'manifest.json').write_text(json.dumps(dict(boards=[r],physical_bits=40,clock_channels=25,flags=len(FLAGS),pin_count=width,scope=__doc__,native_integrated=False),indent=2)+'\n')
    return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=generate(ROOT,a.output.resolve());print('CONTB_MIR_REFERENCE',r['physical_bits'],r['pin_count'],r['extra_boundaries'])
