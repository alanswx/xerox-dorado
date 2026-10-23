#!/usr/bin/env python3
"""Couple original MemC phase/hold packages to original ContB MIR clocks.

MemC l02, e22/e23, clock fanout and all three Hold drivers are physical sources.
BaseBd j02/k01/l01 distribute the same source pulse to both boards. Upstream
Control-A, memory hold-data, MCR and IM/DMD remain explicit boundaries. No
candidate equations drive this reference and no native timing is implied.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_contb_mir_reference import generate as cb_generate,ROOT
from generate_processor_capture_reference import graph
from sil_netlist import load_board

BOARD='MemC-Rev-Be'
SEQ={'l02':(0,2),'e22':(2,2),'e23':(4,2)}
COMB=['l01','l06','h12','i18','j18','d23']
FLAGS=['Hold','CBHold','PRhold','MXHold','IOHold','IfuHold','Hia',"ppSH'"]


def plan(root):
    bindings={"CLK.mc'":'base',"MemClkEnable'a":'pins[56]',"CLKEnable'b":'pins[57]',
              'MemSH':'pins[58]','DisHold':'pins[59]','MemC15.sil+2':'pins[60]',
              "dMDhold'":'pins[61]','MemC15.sil+4':'pins[62]','MemC15.sil+5':'pins[63]'}
    spec=dict(bn=BOARD,comb=COMB,seq=SEQ,module='contb_hold_memc',bindings={},
              clock_taps=['@l02.ca','@l02.cb','@e22.ca','@e22.cb','@e23.ca','@e23.cb'],flag_taps=FLAGS)
    external=graph(root,out=None,inspect=True,**spec)
    extras={n:64+i for i,n in enumerate(sorted(set(external)-set(bindings)))}
    spec['bindings']={n:bindings[n]if n in bindings else f'pins[{extras[n]}]'for n in external}
    return spec,extras


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False);cb=cb_generate(root,out/'cb-source-audit')
    spec,extras=plan(root);mc=graph(root,out=out,**spec);width=64+len(extras)
    p=out/'contb_hold_memc.sv';s=p.read_text().replace('input wire [27:0] pins',f'input wire [{width-1}:0] pins');p.write_text(s)
    (out/'contb_mir_reference.sv').write_bytes((out/'cb-source-audit/contb_mir_reference.sv').read_bytes())
    bb=load_board(str(root/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl'))
    pins={(p['pkg'],p['pin']):n for n,r in bb.nets.items()for p in r['pins']}
    checks={('j02',2):'BaseBd12.sil+3',('j02',3):'BaseBd12.sil+1',
            ('k01',7):'BaseBd12.sil+3',('k01',11):'BaseBd12.sil+3',
            ('k01',4):"CLK.mc'",('k01',13):"CLK.mc'",('l01',7):'BaseBd12.sil+1',
            ('l01',11):'BaseBd12.sil+1',('l01',4):"CLK.cb'"}
    for key,n in checks.items():assert pins[key]==n,(key,n)
    assert all(bb.packages[p]['type']=='MC10210'for p in ['j02','k01','l01'])
    assert all((p,i)not in pins for p in ['k01','l01']for i in [5,6,9,10])
    s=f'''`timescale 1ns/1ps
module contb_hold_reference(input wire fixture_load,input wire [45:0] fixture_state,
 input wire [{width-1}:0] pins,output wire [45:0] state_q,
 output wire [30:0] clocks,output wire [33:0] flags);
 wire cb_pre,mc_pre,cb_root,mc_a,mc_b;wire [7:0] memory_flags;wire [55:0] cb_pins;
 // Original BaseBd pulse distribution; source pulse generation is external.
 cell_MC10210 source_fanout(.p5(pins[0]),.p6(1'b0),.p7(1'b0),.p2(mc_pre),.p3(cb_pre),
   .p9(1'b0),.p10(1'b0),.p11(1'b0));
 cell_MC10210 cb_fanout(.p7(cb_pre),.p11(cb_pre),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),.p4(cb_root));
 cell_MC10210 mc_fanout(.p7(mc_pre),.p11(mc_pre),.p5(1'b0),.p6(1'b0),.p9(1'b0),.p10(1'b0),.p4(mc_a),.p13(mc_b));
 contb_hold_memc m(fixture_load,fixture_state[45:40],mc_a|mc_b,1'b0,1'b0,pins,
   state_q[45:40],clocks[30:25],memory_flags);
 assign cb_pins={{pins[55:4],memory_flags[1],pins[2:1],cb_root}};
 contb_mir_reference b(fixture_load,fixture_state[39:0],1'b0,1'b0,1'b0,cb_pins,
   state_q[39:0],clocks[24:0],flags[25:0]);
 assign flags[33:26]=memory_flags;
endmodule
'''
    (out/'contb_hold_reference.sv').write_text(s)
    result=dict(boards=[cb,mc],physical_bits=46,clock_channels=31,flag_count=34,pin_count=width,
                memory_extra_boundaries=extras,obsolete_external_pin=3,scope=__doc__,
                physical_map=dict(ContB='39:0',MemC_l02='41:40',MemC_e22='43:42',MemC_e23='45:44'),
                clock_source=dict(board='BaseBd-Rev-Am',physical_pin_checks=len(checks),
                    wirelist_sha256=hashlib.sha256((root/'chm/sil/BaseBd-Rev-Am.dm!1_/BaseBd-Rev-Am.wl').read_bytes()).hexdigest(),
                    contract='Original j02/k01/l01 parallel pulse fanout. Analog board skew and pulse generation remain unqualified.'),
                native_integrated=False,physical_timing_qualified=False)
    (out/'manifest.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    r=generate(ROOT,p.parse_args().output.resolve());print('CONTB_HOLD_REFERENCE',r['physical_bits'],r['pin_count'],r['memory_extra_boundaries'])
