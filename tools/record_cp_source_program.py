#!/usr/bin/env python3
"""Re-run the original full producer program with actual CP source closure.

Keep all original stimulus, capture/window assertions and final coverage tests.
Use the qualified corrected single-event processor/read owner to establish an
independent original-cell trace before qualifying a combined packet replay.
The new full program may reveal additional capture bugs; a trace is qualified
only if every original assertion and coverage requirement passes.
"""
import argparse
import json
from pathlib import Path
import shutil
from unittest.mock import patch

import test_r_front_cp_source as parent
from test_external_r_packet import sha


def main_program(raw):
    start=raw.index(' initial begin\n   if($value$plusargs("iterations=%d",iterations))')
    end=raw.index('\n end\n',start)+len('\n end')
    return start,end,raw[start:end]


def full_bench(original_bench,trace_failure=False):
    def generate(raw,*args,**kwargs):
        _,_,program=main_program(raw)
        actual=original_bench(raw,*args,**kwargs)
        start,end,old=main_program(actual)
        marker='   $finish;'
        assert program.count(marker)==1
        extra='''   for(integer channel=0;channel<100;channel++)
    $display("FULL_CP_CLOCK channel=%0d edges=%0d",channel,totals[channel]);
   $display("FULL_PROCESSOR_CP_SOURCE_PASS checks=%0d original_program=1 Qs=844 RAM=9216 clocks=100",checks);
'''
        new=program.replace(marker,extra+marker,1)
        assert new.replace(extra,'',1)==program
        actual=actual[:start]+new+actual[end:]
        assert actual[start:start+len(new)]==new
        if trace_failure:
            marker='     if({expected_source[201:145],expected_source[126:0],expected_fa} !=='
            probe='''     if(checks>=760 && checks<=762)begin
      $display("ORIGINAL_CP_CROSSING check=%0d before_root=%b after_root=%b before=%h after=%h expected_Q=%h reference_Q=%h",checks,cb,ca,dut.source_before,dut.source_after,expected,reference);
      for(integer bit_no=0;bit_no<202;bit_no++)
       if((bit_no<127 || bit_no>144) && expected_source[bit_no]!==ref_source_pins[bit_no])
        $display("ORIGINAL_CP_SOURCE_DIFF check=%0d bit=%0d expected=%b actual=%b",checks,bit_no,expected_source[bit_no],ref_source_pins[bit_no]);
      for(integer bit_no=0;bit_no<8;bit_no++)
       if(expected_fa[bit_no]!==ref_fa_n[bit_no])
        $display("ORIGINAL_CP_FA_DIFF check=%0d bit=%0d expected=%b actual=%b",checks,bit_no,expected_fa[bit_no],ref_fa_n[bit_no]);
     end
'''
            assert actual.count(marker)==1
            actual=actual.replace(marker,probe+marker,1)
            arrival=''' always @(original.l.g15.p13 or rb_address or original.l.g14.state_q or original.l.g15.state_q)
  if(monitor && checks>=760 && checks<=762)
   $display("ORIGINAL_RBASE_ARRIVAL t=%0.3f check=%0d root=%b WE_n=%b address=%h g14=%h file=%h a19_select=%b candidate_WE=%b/%b candidate_A=%h/%h",$realtime,checks,root,
    original.l.g15.p13,rb_address,original.l.g14.state_q,original.l.g15.state_q,original.l.a19.p9,
    dut.child.child.before_clocks[46],dut.child.child.after_clocks[46],
    dut.child.child.before_front_data[11:8],dut.child.child.pre_data[11:8]);
'''
            assert actual.count('endmodule')==1
            actual=actual.replace('endmodule',arrival+'endmodule',1)
        return actual
    return generate


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','original-trace','cp-source','output']:
        p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--trace-failure',action='store_true')
    p.add_argument('--profile',choices=parent.PROFILES,default='later_control')
    p.add_argument('--phase-ps',type=int,help='Diagnostic SE10231 delay override; not a device specification')
    a=p.parse_args();out=a.output.resolve();invoked_source=Path(__file__).read_bytes()
    staged=a.profile=='later_control'
    timing=parent.PROFILES[a.profile]
    actual_timing=timing if a.phase_ps is None else (timing[0],timing[1],a.phase_ps)
    with patch.object(parent,'bench',full_bench(parent.bench,a.trace_failure)),patch.dict(parent.PROFILES,{a.profile:actual_timing}):
        code=parent.run(a.source.resolve(),a.original_trace.resolve(),a.cp_source.resolve(),out,a.engine,
                        block_capture=True,repro=False,collisions=False,ready_zero=False,profile=a.profile,
                        prblock_capture=True,interior_read=staged,interior_sequencer=staged,interior_tags=staged,
                        read_owner=staged,owner_probe=staged)
    (out/Path(__file__).name).write_bytes(invoked_source)
    state=json.loads((out/'run-state.json').read_text())
    state.update(original_full_stimulus_program=True,original_final_coverage_assertions_retained=True,
                 source_program_tool_sha256=sha(out/Path(__file__).name),ordinary_paired_path_tested=False,
                 passive_failure_trace=a.trace_failure,
                 timing_control_uses_unstaged_original_caller=not staged,
                 diagnostic_phase_override_ps=a.phase_ps,
                 scope=__doc__)
    if not code:
        state['status']='PASS_ORIGINAL_FULL_PROGRAM_WITH_CP_SOURCE_CLOSURE'
    (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    raise SystemExit(code)
