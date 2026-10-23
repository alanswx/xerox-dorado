#!/usr/bin/env python3
"""Couple original ContA clocks/tasks/MIR to ContB and MemC Hold producers.

156 physical Qs:110 ContA,40 ContB,6 MemC. Close actual registered clock
copies, MemSH, Hold, ASEL/BSEL, UseDMD, StopMIRClk, shared rMIRa and CP bus.
MCR/hold-data/IM/parity/DMD/B-bus producers remain explicit boundaries.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_r_front_l17_reference import generate as ca_generate
from generate_contb_hold_reference import generate as memory_generate,ROOT
from generate_conta_mir_reference import FLAGS as MIR_FLAGS
from generate_conta_local_reference import LOCAL_FLAGS
from generate_processor_capture_reference import graph

SEAMS=["CLKEnable'a","CLKEnable'b","MemClkEnable'a",'MemSH','UseDMD','rMIRa','@j02.p14',"StartCycle'a","BNTGtCT'a"]


def generate(root,out):
    out.mkdir(parents=True,exist_ok=False)
    ca_parent=ca_generate(root,out/'ca-source-audit');hold=memory_generate(root,out/'hold-source-audit')
    old=ca_parent['boards'][0]
    spec=dict(bn=old['board'],comb=sorted(set(old['packages'])-set(old['sequential'])),seq=old['sequential'],
              module='control_hold_conta',bindings=old['boundary_bindings'],clock_taps=old['clock_taps'],flag_taps=MIR_FLAGS+LOCAL_FLAGS+SEAMS)
    ca=graph(root,out=out,**spec);p=out/'control_hold_conta.sv';s=p.read_text()
    s=s.replace('input wire [27:0] pins','input wire [325:0] pins,input wire cb_reset')
    name=next(line.split()[1].removesuffix(';')for line in s.splitlines()if line.endswith('; // rMIRa'))
    before=' assign '+name+'='+name+'_j02_14;';assert s.count(before)==1
    s=s.replace(before,before[:-1]+' | cb_reset;');p.write_text(s)
    for p in (out/'hold-source-audit').glob('*.sv'):
        s=p.read_text()
        if p.name=='contb_hold_reference.sv':
            old='output wire [33:0] flags);';assert s.count(old)==1;s=s.replace(old,'output wire [33:0] flags,output wire ca_root);')
            old='.p4(cb_root));';assert s.count(old)==1;s=s.replace(old,'.p4(cb_root),.p2(ca_root));')
        (out/p.name).write_text(s)
    s='''`timescale 1ns/1ps
module control_hold_reference(input wire fixture_load,input wire [155:0] fixture_state,
 input wire base,input wire [397:0] pins,output wire [155:0] state_q,
 output wire [55:0] clocks,output wire [93:0] flags);
 wire [59:0] af;wire [33:0] bf;wire ca_root;
 reg [325:0] ap;reg [71:0] bp;
 always @*begin
  ap=pins[325:0];ap[69]=bf[26];ap[294]=bf[0];ap[205]=bf[4];ap[293]=bf[20];
  bp=pins[397:326];bp[0]=base;bp[1]=af[51];bp[2]=af[58];bp[4]=af[59];bp[5]=af[55];bp[6]=af[57];
  bp[56]=af[53];bp[57]=af[52];bp[58]=af[54];
  bp[10]=pins[291];bp[11]=pins[290];bp[12]=pins[289];bp[13]=pins[292];
  bp[15]=pins[282];bp[16]=pins[283];bp[17]=pins[284];
 end
 control_hold_conta a(fixture_load,fixture_state[109:0],ca_root,1'b0,1'b0,ap,m.b.h03.p2,
   state_q[109:0],clocks[24:0],af);
 contb_hold_reference m(fixture_load,fixture_state[155:110],bp,state_q[155:110],clocks[55:25],bf,ca_root);
 assign flags={bf,af};
endmodule
'''
    (out/'control_hold_reference.sv').write_text(s)
    r=dict(boards=[ca,*hold['boards']],physical_bits=156,clock_channels=56,flag_count=94,pin_count=398,
           state_map=dict(ContA='109:0',ContB='149:110',MemC='155:150'),scope=__doc__,
           CA_flag_seams=dict(zip(SEAMS,range(51,60))),
           closed_sources=['Hold/CBHold','CLKEnable a/b','MemClkEnable a','MemSH','StartCycle a','BNTGtCT a','UseDMD','ASEL0 a/BSEL0','StopMIRClk','wired-OR rMIRa','shared CP address/strobe and CPOut1..3'],
           original_clock_source=hold['clock_source'],native_integrated=False,physical_timing_qualified=False)
    assert sum(b['physical_bits']for b in r['boards'])==156
    (out/'manifest.json').write_text(json.dumps(r,indent=2)+'\n');return r


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    r=generate(ROOT,p.parse_args().output.resolve());print('CONTROL_HOLD_REFERENCE',r['physical_bits'],r['clock_channels'])
