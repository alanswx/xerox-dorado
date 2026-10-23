#!/usr/bin/env python3
"""Probe actual LOW-clock B-bus changes into original l17 -> d17 feedback.

Data-only LOW-root events must retain l17, then the next clk1 edge captures
its current input. Both directions pass through the actual d17 feedback.
The complete 798-Q/RAM/clock graph still compares at every event.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch
import test_r_front_l17 as l17


def run(a):
    parent=l17.visible.masked.commands.parent;original=parent.bench
    def bench(*args,**kwargs):
        raw=original(*args,**kwargs);marker='   // IM data settles';assert raw.count(marker)==1
        directed=r'''
   // Keep the actual MidasOn d17 Q high while exercising the B-input gate.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[204:202]=3'b110;
   p[253:246]=im_ff(8'h61);p[261:254]=8'h01;p[317]=0;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer n=0;n<48;n++)event_step(!cb,p);
   if(!q[788])$fatal(1,"L17 late-B setup missed actual MidasOn state");
   for(integer direction=0;direction<2;direction++)begin
     found=0;
     for(integer n=0;n<64&&!found;n++)begin
       event_step(!cb,p);if(!cb&&!clocks[68]&&q[788])found=1;
     end
     if(!found)$fatal(1,"L17 late-B setup missed LOW clk1 window");
     if(q[794]!==direction[0])$fatal(1,"L17 late-B previous feedback value wrong");
     p[317]=!direction[0];event_step(0,p);
     if(q[794]!==direction[0])$fatal(1,"L17 changed from B data without clock");
     event_step(1,p);
     if(q[794]!==!direction[0])$fatal(1,"L17 did not capture late B data on clk1");
     for(integer n=0;n<32;n++)event_step(!cb,p);
     if(q[789]!==!direction[0]||q[795]!==!direction[0])
       $fatal(1,"L17 late-B did not traverse actual d17 feedback");
   end
   $display("R_FRONT_L17_LATE_B directions=2 old_hold=1 capture=1 d17_feedback=1");
   p=SEED;event_step(1,p);
'''
        return raw.replace(marker,directed+'\n'+marker)
    a.mutation=None
    with patch.object(parent,'bench',bench):rc=l17.run(a)
    p=a.output.resolve()/'run-state.json';s=json.loads(p.read_text())
    s.update(l17_late_B_bus=True)
    p.write_text(json.dumps(s,indent=2)+'\n');return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='verilator');p.add_argument('--profile',choices=l17.visible.masked.commands.parent.PROFILES,default='balanced')
    p.add_argument('--iterations',type=int,default=1024)
    sys.exit(run(p.parse_args()))
