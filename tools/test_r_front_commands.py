#!/usr/bin/env python3
"""Probe actual original branch/IM/TPC command producers with processor state.

All 792 physical Qs, 9216 RAM bits and 68 clocks remain compared per event.
WIM is an asynchronous control qualifier; d17 samples its old MIR data epoch.
This experiment is not native boot, physical timing or speed qualification.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_r_front_local as parent
from generate_r_front_commands_reference import CANDIDATE, generate


def run(a):
    candidate = CANDIDATE
    if a.mutation:
        source = (parent.ROOT/CANDIDATE).read_text()
        edits = {
            'count_external': ('r[235]=count_branch(mir,p)', 'r[235]=!p[311]'),
            'read_external': ('!old_local[0],old_memory[0]', '!old_local[0],pins_after[128]'),
            'wim_external': ('c=p[8:0];c[8]=m[1];', 'c=p[8:0];c[8]=p[8];'),
            'read_new_epoch': ('   if(capture[22])n[103:98]=', '   old_memory=memory_commands(n[97:76]);\n   if(capture[22])n[103:98]='),
        }
        before, after = edits[a.mutation]
        assert source.count(before) == 1
        p = a.output.resolve().parent/(a.output.name+'-candidate.sv')
        assert not p.exists()
        p.write_text(source.replace(before, after))
        candidate = str(p.relative_to(parent.ROOT))
    original_bench = parent.bench
    def bench(*args, **kwargs):
        raw = original_bench(*args, **kwargs)
        assert raw.count('dorado_r_front_local_step dut(') == 1
        raw = raw.replace('dorado_r_front_local_step dut(', 'dorado_r_front_commands_step dut(')
        raw = raw.replace('[314:0]', '[316:0]').replace('rp[314:145]', 'rp[316:145]')
        marker = '     if(ready_flags!==ref_ready_flags)'
        assert raw.count(marker) == 1
        raw = raw.replace(marker,'''     if(dut.settled_commands !== {original.fc[64],original.fc[63],original.fc[65]})
       $fatal(1,"Original IM/TPC command producer mismatch check=%0d",checks);
'''+marker)
        if a.directed:
            marker = ' reg [316:0] p;'
            assert raw.count(marker) == 1
            raw = raw.replace(marker,''' integer command_intervals=0,last_command_start,one_command_intervals,write_im_phases=0;
 reg command_old_start;
'''+marker)
            marker = '   // IM data settles'
            assert raw.count(marker) == 1
            directed = r'''
   // Actual Return-class opcodes: all eight d20 decoder selections. The
   // obsolete WIM/RIM/count inputs remain independent of these MIR values.
   if(!cb)event_step(1,p);
   p=SEED;p[8:0]=9'h178;p[293]=0;p[294]=0;p[205]=1;p[204:202]=3'b110;p[253:246]=0;p[263]=0;event_step(1,p);
   cp_write(0,40);cp_write(1,64);cp_write(0,8);cp_write(1,0);cp_write(0,0);
   for(integer command_no=0;command_no<8;command_no++)begin
     if(!cb)event_step(1,p);
     p[261:254]=8'he2|8'(command_no<<2);event_step(1,p);
     for(integer n=0;n<32;n++)event_step(!cb,p);
     if(dut.settled_commands !== {command_no!=5,command_no!=7,(command_no==1||command_no==3)})
       $fatal(1,"Actual IM/TPC command decode mismatch command=%0d",command_no);
     if(q[786] !== (command_no==1||command_no==3))
       $fatal(1,"Actual RIM/RTPC d17 capture mismatch command=%0d",command_no);
     last_command_start=-1;one_command_intervals=0;
     for(integer n=0;n<96;n++)begin
       command_old_start=q[25];event_step(!cb,p);
       if(!command_old_start&&q[25])begin
         if(last_command_start>=0)begin
           if(counts[0]-last_command_start != (command_no[0]?6:2))
             $fatal(1,"Actual command cadence mismatch command=%0d clocks=%0d",command_no,counts[0]-last_command_start);
           one_command_intervals++;command_intervals++;
         end
         last_command_start=counts[0];
       end
       if(command_no==7&&!q[30])write_im_phases++;
     end
     if(one_command_intervals<4)$fatal(1,"Actual command cadence not exercised command=%0d",command_no);
   end
   if(write_im_phases<4)$fatal(1,"Actual WriteIM phase never opened");
   // FF branch selector 3 (count zero), its FC neighbor, and the ASEL gate.
   event_step(1,p);p[261:254]=8'h01;event_step(1,p);
   for(integer asel=0;asel<2;asel++)for(integer bsel=0;bsel<2;bsel++)
     for(integer variant=0;variant<3;variant++)begin
       event_step(1,p);p[294]=1'(asel);p[205]=1'(bsel);
       p[253:246]=im_ff(variant==0?8'h33:variant==1?8'h32:8'h73);event_step(1,p);
       for(integer n=0;n<32;n++)event_step(!cb,p);
       if(dut.settled_pins[235] !== (bsel!=0&&(variant==0||(variant==2&&asel!=0))))
         $fatal(1,"Actual FF count-branch mismatch ASEL=%0d BSEL=%0d variant=%0d",asel,bsel,variant);
     end
   // JCN count branch and three neighboring non-count encodings.
   for(integer variant=0;variant<4;variant++)begin
     event_step(1,p);p[253:246]=0;p[294]=0;p[205]=1;
     p[261:254]=variant==0?8'hc2:variant==1?8'hc0:variant==2?8'hc3:8'he2;event_step(1,p);
     for(integer n=0;n<32;n++)event_step(!cb,p);
     if(dut.settled_pins[235] !== (variant==0))
       $fatal(1,"Actual JCN count-branch mismatch variant=%0d",variant);
   end
   $display("R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=%0d WriteIM=%0d",command_intervals,write_im_phases);
   p=SEED;event_step(1,p);
'''
            raw = raw.replace(marker, directed+'\n'+marker)
        return raw
    with patch.object(parent, 'CANDIDATE', candidate), patch.object(parent, 'generate', generate), patch.object(parent, 'bench', bench):
        rc = parent.run(a.base.resolve(), a.output.resolve(), a.iterations, a.engine, a.profile,
                        True, True, True, True, None, True)
    path = a.output.resolve()/'run-state.json'
    record = json.loads(path.read_text())
    record.update(experiment='actual_ContA_branch_IM_TPC_commands',
                  directed_commands=a.directed, command_mutation=a.mutation,
                  full_arrival_contract_qualified=False, physical_timing_qualified=False)
    path.write_text(json.dumps(record, indent=2)+'\n')
    return rc


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], default='verilator')
    p.add_argument('--profile', choices=parent.PROFILES, default='balanced')
    p.add_argument('--iterations', type=int, default=1024)
    p.add_argument('--directed', action='store_true')
    p.add_argument('--mutation', choices=['count_external', 'read_external', 'wim_external', 'read_new_epoch'])
    sys.exit(run(p.parse_args()))
