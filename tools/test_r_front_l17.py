#!/usr/bin/env python3
"""Compare the 798-Q original processor graph with actual ContA l17 feedback.

Retain all physical RAM, clocks and functional-port comparisons. Additional
state belongs to the original MC10176 package; no native timing is implied.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch
import test_r_front_visible as visible
from generate_r_front_l17_reference import generate as reference_generate
from generate_r_front_l17_candidate import generate as candidate_generate


def run(a):
    parent=visible.masked.commands.parent;original=parent.run
    candidate=a.output.resolve().parent/(a.output.name+'-candidate.sv')
    metadata=candidate_generate(candidate)
    mutation=a.mutation
    if mutation:
        s=candidate.read_text()
        old,new={'external':('state_q[106],old_local[5]','pins_after[129],old_local[5]'),
                 'clock0':('c[23]=base | q[6] | !q[7] | !q[20]','c[23]=base | q[6] | !q[7] | !q[25]')}[mutation]
        assert s.count(old)==1;candidate.write_text(s.replace(old,new))
    def integrated(*args,**kwargs):
        old_bench=parent.bench
        def bench(*bargs,**bkwargs):
            raw=old_bench(*bargs,**bkwargs)
            for old,new in [('dorado_r_front_commands_step dut','dorado_r_front_l17_step dut'),
                            ('[791:0]','[797:0]'),('[316:0]','[325:0]'),('rp[316:145]','rp[325:145]'),
                            ('[67:0]','[68:0]'),('0:67','0:68'),('i<68','i<69'),('channel<68','channel<69'),
                            ('i<792','i<798'),('[103:0] conta_next','[109:0] conta_next')]:
                raw=raw.replace(old,new)
            raw=raw.replace(' reg [99:0] front_changed=0;', ' reg [99:0] front_changed=0;')
            raw=raw.replace('integer checks=0;', 'reg [5:0] l17_changed=0;integer checks=0;')
            raw=raw.replace('q=expected;cb=next_root;', 'l17_changed|=q[797:792]^expected[797:792];q=expected;cb=next_root;')
            marker='   fixture[787]=1;';assert raw.count(marker)==1
            raw=raw.replace(marker,"   fixture[797:792]=6'b111111;\n"+marker)
            marker='   // IM data settles';assert raw.count(marker)==1
            directed=r'''
   // Original UseDMD / MidasOn / B_Dbuf and the l17 -> d17 feedback.
   // Obsolete external slot297 remains independent of the actual l17 Q.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[204:202]=3'b110;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer data=0;data<2;data++)for(integer command_no=0;command_no<4;command_no++)begin
     event_step(1,p);p[317]=1'(data);p[297]=!data[0];
     p[253:246]=im_ff(command_no==0?8'h61:command_no==1?8'h60:command_no==2?8'h7d:8'h00);
     event_step(1,p);for(integer n=0;n<48;n++)event_step(!cb,p);
   end
   $display("R_FRONT_L17_DIRECTED changed=%h clock_edges=%0d",l17_changed,totals[68]);
   p=SEED;event_step(1,p);
'''
            raw=raw.replace(marker,directed+'\n'+marker)
            marker='   $display("R_FRONT_LOCAL_PROBE PASS';assert raw.count(marker)==1
            raw=raw.replace(marker,'   if(l17_changed!==6\'h3f||totals[68]<10)$fatal(1,"L17 physical state/clock coverage incomplete");\n'+marker)
            return raw
        with patch.object(parent,'CANDIDATE',str(candidate.relative_to(parent.ROOT))),patch.object(parent,'generate',reference_generate),patch.object(parent,'bench',bench):
            return original(*args,**kwargs,extra_sources=['verilog/cells/cell_MC10197.v'])
    with patch.object(parent,'run',integrated):rc=visible.run(a)
    p=a.output.resolve()/'run-state.json';r=json.loads(p.read_text())
    r.update(experiment='actual_ContA_l17_feedback',l17_candidate=metadata,l17_mutation=mutation,
             physical_bits=798,clock_channels=69,native_integrated=False,physical_timing_qualified=False)
    p.write_text(json.dumps(r,indent=2)+'\n');return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--engine',choices=['icarus','verilator'],default='verilator')
    p.add_argument('--profile',choices=visible.masked.commands.parent.PROFILES,default='balanced')
    p.add_argument('--iterations',type=int,default=1024);p.add_argument('--mutation',choices=['external','clock0'])
    sys.exit(run(p.parse_args()))
