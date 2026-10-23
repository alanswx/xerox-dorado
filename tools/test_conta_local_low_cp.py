#!/usr/bin/env python3
"""Probe local StopTasks data/clock ordering during LOW-root CP maintenance.

Start tasking, stop it, then clear a just-loaded TaskingOn before d17's next
capture. Retain a pending task request so old/new StopTasks data epochs differ.
Also sweep both CP banks with root LOW. This is a targeted component probe.
"""
import argparse
from pathlib import Path
from unittest.mock import patch

import test_conta_local as component


def bench():
 raw=component.bench()
 marker=' initial begin\n  for(integer k=0;k<23;k++)counts[k]=0;'
 assert raw.count(marker)==1
 prefix=raw.split(marker)[0]
 return prefix+r'''
 reg found;
 integer low_transactions=0;
 initial begin
  for(integer k=0;k<23;k++)counts[k]=0;
  #20000;seed(0);start();
  event_pins(1,pins);p=pins;p[83:76]=8'h46;p[91:84]=8'h01;p[123]=0;p[124]=1;p[125]=0;event_pins(1,p);
  repeat(16)cycle();event_pins(1,pins);p=pins;p[83:76]=0;event_pins(1,p);repeat(16)cycle();
  if(!local_flags[0]||state[99])$fatal(1,"LOW CP setup did not store TaskingOff");
  event_pins(1,pins);p=pins;p[20]=1;event_pins(1,p);repeat(8)cycle();
  if(state[41])$fatal(1,"LOW CP setup switched while tasking stopped");
  event_pins(1,pins);p=pins;p[83:76]=8'hc6;event_pins(1,p);
  found=0;
  for(integer n=0;n<32 && !found;n++)begin
   event_pins(!base,pins);
   if(state[83:76]==8'h36 && !state[99])found=1;
  end
  if(!found||!base||local_flags[0])$fatal(1,"LOW CP setup missed pending TaskingOn capture");
  found=0;
  for(integer n=0;n<32 && !found;n++)begin
   event_pins(!base,pins);
   if(!base&&!clocks[22]&&!state[99])found=1;
  end
  if(!found||local_flags[0])$fatal(1,"LOW CP setup did not open task-clock window");
  p=pins;p[122]=1;p[121:119]=3'b111;p[118:112]=7'd16;event_pins(0,p);
  p[122]=0;event_pins(0,p);p[122]=1;event_pins(0,p);
  if(!local_flags[0]||state[99])$fatal(1,"LOW CP MIR clear did not reassert StopTasks");
  $display("CONTA_LOCAL_ASYNC root=%b tasking_n=%b switch=%b StopTasks=%b",base,state[99],state[41],local_flags[0]);
  // Complete LOW-root transactions followed by normal root cycles.
  for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)begin
   p=pins;p[122]=1;p[121:119]=~3'(bank);p[118:112]=7'(data);event_pins(0,p);
   p[122]=0;event_pins(0,p);p[122]=1;event_pins(0,p);
   event_pins(1,pins);event_pins(0,pins);low_transactions++;
  end
  if(low_transactions!=256||counts[22]<16)$fatal(1,"LOW CP coverage incomplete");
  $display("CONTA_LOCAL_LOW_CP transactions=%0d tasking_edges=%0d",low_transactions,counts[22]);
  $display("CONTA_LOCAL PASS checks=%0d cp=%0d mir=%0d task_changes=%0d tasks=%h",checks,cp_edges,mir_edges,task_changes,seen_tasks);
  $finish;
 end
endmodule
'''


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--engine',choices=['both','icarus','verilator'],default='both')
 p.add_argument('--profile',choices=[x[0]for x in component.PROFILES],default='balanced')
 p.add_argument('--mutations',action='store_true');a=p.parse_args()
 source=bench()
 mutants={'new_StopTasks_data_epoch':('task_data_pins(state_q,pins_after)','task_data_pins(q,pins_after)')}
 with patch.object(component,'bench',return_value=source),patch.object(component,'MUTATIONS',mutants),patch.object(component,'MUTATION_PROFILES',{x[0]for x in component.PROFILES}):
  component.run(a.output.resolve(),a.engine,a.profile,a.mutations)
