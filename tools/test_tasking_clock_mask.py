#!/usr/bin/env python3
"""Check task-clock masking at an actual LOW NoDispatch maintenance reset.

Independent original156-Q/56-clock/94-flag graph, including the real MemC
Hold source. Component initial fixtures are explicit physical FF states;
they never enter a native run. A frozen-reset negative must miss a real edge.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_control_hold as parent
from test_control_hold_ordered import descriptor
from generate_r_front_control_candidate import generate

DIRECTED=r'''
  seed(0);start();p=reference_pins;p[253:246]=im_ff(8'h62);p[261:254]=1;
  p[326+28]=1;p[326+31]=0;event_step(1,p);repeat(64)cycle();
  if(!flags[46])$fatal(1,"Task-clock fixture did not load actual TaskingOff instruction");
  sample=q;
  // Model the independent physical copies at a LOW Clock0 window. Switchb
  // inhibits TaskingOff until NoDispatch's asynchronous reset clears it.
  sample[41:40]=3;sample[43]=0;sample[7]=1;sample[6]=0;
  sample[23]=1;sample[25]=1;sample[8]=1;sample[9]=1;
  sample[10]=0;sample[11]=0;sample[15]=1;sample[17]=0;
  for(integer held=0;held<2;held++)begin
    fixture_hold=held[0];seed(sample);p=reference_pins;
    p[253:246]=im_ff(8'h62);p[261:254]=1;p[326+28]=1;p[326+31]=0;
    p[326+60]=!held[0];p[326+61]=1;event_step(0,p);
    if(root||q[69]||q[41:40]!==2'b11||flags[43]||clocks[12]||flags[86]!==held[0])
      $fatal(1,"Task-clock LOW fixture invalid held=%0d root=%b NoDispatch=%b Switch=%b Stop=%b clock=%b Hold=%b",held,root,q[69],q[41:40],flags[43],clocks[12],flags[86]);
    p[292]=1;p[291:289]=7;p[288:282]=8;event_step(0,p);
    p[292]=0;event_step(0,p);previous_switch=counts[12];
    p[292]=1;event_step(0,p);
    if(!q[69]||q[41:40]!==0||counts[12]!=previous_switch+1||!clocks[12])
      $fatal(1,"Task-clock real NoDispatch reset/edge not exercised held=%0d",held);
    $display("TASKING_MASK_RESET held=%0d LOW_root=1 Switch_before=11 Switch_after=%b actual_clock_edges=%0d",held,q[41:40],counts[12]-previous_switch);
  end
  $display("CONTROL_HOLD PASS checks=%0d physical=156 clocks=56 reset_cases=2",checks);$finish;
'''


def run(out,engine,profile,frozen):
    arrival=descriptor(profile)
    old='dorado_control_hold_step dut(q,before_root,after_root,before_pins,after_pins,next_q,events,repeated,unsettled,flags);'
    new=f"dorado_control_hold_ordered_step dut(q,before_root,after_root,before_pins,after_pins,next_q,events,repeated,unsettled,1'b{arrival},flags);"
    tb=parent.TB.replace(old,new);assert tb!=parent.TB
    marker=' reg [397:0] p;reg [155:0] sample;reg found;integer previous_mir,late_captures=0;'
    assert tb.count(marker)==1;tb=tb.replace(marker,' integer previous_switch;\n'+marker)
    tb=tb.replace('module tb_control_hold;','module tb_control_hold;\n reg fixture_hold=0;',1)
    old='fixture[150]=1;fixture[151]=0;fixture[153:152]=3;fixture[154]=0;';assert tb.count(old)==1
    tb=tb.replace(old,"fixture[150]=1;fixture[151]=0;fixture[153:152]={1'b1,!fixture_hold};fixture[154]=0;")
    first=tb.index(' initial begin\n  seed(');end=tb.index('\n end\nendmodule',first)
    tb=tb[:first]+' initial begin\n'+DIRECTED+tb[end:]
    def candidate(path):
        full=path.with_name('full844.sv');metadata=generate(full,old_control_capture=True,mask_tasking_switch=True)
        raw=full.read_text();start=raw.index('// Experimental original ContA/ContB/MemC phase and Hold composition.')
        ctrl=raw[start:]
        if frozen:
            old='tasking_view=0;';assert ctrl.count(old)==1
            ctrl=ctrl.replace(old,'tasking_view=tasking_view;',1)
        path.write_text(ctrl)
        return dict(scope=__doc__,full_candidate_sha256=metadata['output_sha256'],output_sha256=hashlib.sha256(ctrl.encode()).hexdigest(),frozen_NoDispatch_view=frozen)
    with patch.object(parent,'TB',tb),patch.object(parent,'candidate_generate',candidate):rc=parent.run(out,engine,profile)
    p=out/'run-state.json';r=json.loads(p.read_text());r.update(experiment='tasking_clock_mask_actual_NoDispatch_reset',frozen_NoDispatch_view=frozen,
        initial_component_fixture_only=True,native_state_imported=False,expected_arrival_descriptor=arrival)
    p.write_text(json.dumps(r,indent=2)+'\n');return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--profile',choices=parent.PROFILES,default='balanced')
    p.add_argument('--frozen-reset',action='store_true');a=p.parse_args();sys.exit(run(a.output.resolve(),a.engine,a.profile,a.frozen_reset))
