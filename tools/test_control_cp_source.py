#!/usr/bin/env python3
"""Compare actual CP decoder arrivals in the full original control graph.

Keep all156 Qs,56 event counts,94 functional flags, settled identity and18
decoder observations. Exhaust real CP words at both root levels and exercise
root/strobe coincidences with the actual memory/control/MIR feedback. These
declared delay profiles are digital arrival experiments, not native timing.
"""
import argparse
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_control_hold as parent
from generate_control_cp_source_reference import generate
from prove_control_fall_fusion import sources
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha
from generate_cp_block_capture import rewrite,route

WRAPPER='''module dorado_control_hold_step(input wire [155:0] q,
 input wire before_root,after_root,input wire [397:0] before_pins,after_pins,
 output wire [155:0] next_q,output wire [55:0] events,repeated,
 output wire unsettled,output wire [93:0] flags);
 FUNCTIONS
 wire [400:0] before_full={3'b0,before_pins},after_full={3'b0,after_pins};
 wire [400:0] before_cp_pins=cp_source_pins(before_full,cp_bus(before_full));
 wire [400:0] after_cp_pins=cp_source_pins(after_full,cp_bus(after_full));
 wire split=(before_root!=after_root)&&(before_cp_pins[292:282]!=after_cp_pins[292:282]);
 wire [10:0] prefix_cp=split ? after_cp_pins[292:282]:before_cp_pins[292:282];
 wire [268:0] prefix;
 // Prefix output is a pure uncached producer of these same original inputs.
 // It has no dependency on the supplied saved-prefix input.
 dorado_control_hold_ordered_step #(.WAVES(5)) actual(q,before_root,after_root,
  before_cp_pins[397:0],after_cp_pins[397:0],next_q,events,repeated,unsettled,
  1'bARRIVAL,flags,,,1'b0,prefix,prefix_cp,after_cp_pins[292:282],prefix);
endmodule
'''

PROGRAM=''' reg [397:0] p;reg [17:0] observed_sets=0;
 integer words=0,collisions=0;
 task automatic set_word(input integer bank,word);
  begin
   p=reference_pins;p[292]=1;p[291:289]=~3'(bank);
   p[288:282]=7'(word>>1);p[340]=word[0];p[344]=word[8];
  end
 endtask
 initial begin
  seed(0);event_step(0,reference_pins);start();repeat(128)cycle();
  for(integer level=0;level<2;level++)begin
   for(integer bank=4;bank<8;bank++)for(integer word=0;word<512;word++)begin
    seed(0);set_word(bank,word);event_step(level[0],p);
    p[292]=0;event_step(level[0],p);observed_sets|=ref_sets;
    p[292]=1;event_step(level[0],p);words++;
   end
  end
  // Running producer and root coincidences, with independent physical reset
  // fixtures between words. The reference supplies every resulting Q/clock.
  for(integer direction=0;direction<2;direction++)begin
   for(integer bank=4;bank<8;bank++)for(integer pattern=0;pattern<16;pattern++)begin
    seed(0);start();set_word(bank,pattern==15 ? 511:1<<(pattern%9));
    event_step(!direction[0],p);
    p[292]=0;event_step(direction[0],p);observed_sets|=ref_sets;
    p[292]=1;event_step(direction[0],p);collisions++;
   end
  end
  if(words!=4096 || collisions!=128 || observed_sets!==18'h3ffff)
   $fatal(1,"CONTROL_CP_SOURCE coverage words=%0d collisions=%0d sets=%h",words,collisions,observed_sets);
  for(integer i=0;i<56;i++)$display("CONTROL_CP_SOURCE_CLOCK channel=%0d edges=%0d",i,counts[i]);
  $display("CONTROL_CP_SOURCE_PASS checks=%0d words=%0d collisions=%0d Qs=156 clocks=56 flags=94 sets=%h",checks,words,collisions,observed_sets);
  $finish;
 end
endmodule
'''


def run(source,cp_source,out,engine,profile,repro_block=False,cp_block_capture=False,forced_early=None,
        repro_normal_block=False,bad_normal_block=False):
    assert not (repro_block and repro_normal_block)
    assert forced_early is None or cp_block_capture
    assert not bad_normal_block or (cp_block_capture and repro_normal_block)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    cp=json.loads((cp_source/'manifest.json').read_text());verify(cp_source,cp['files'])
    # Keep the original fall helper. Its extraction copies all terminal
    # functions, while the new CP view belongs to the parameterized terminal
    # only; it must not introduce that parameter into the unrelated helper.
    raw=(source/'candidate.sv').read_text();atomic,fall=sources(raw)
    descriptor=route(parent.ROOT,parent.PROFILES[profile][0],parent.PROFILES[profile][1])
    if cp_block_capture:
        raw=rewrite(raw,bool(descriptor['early'] if forced_early is None else forced_early));atomic,_=sources(raw)
    if bad_normal_block:
        marker='cp_block_at_tpc=source[93];'
        assert atomic.count(marker)==1
        atomic=atomic.replace(marker,'cp_block_at_tpc=p[93];',1)
    fn=(cp_source/'functions.sv').read_text()
    arrival=int(parent.PROFILES[profile][2]<parent.PROFILES[profile][0])
    def candidate(path):
        code=atomic+'\n'+fall+'\n'+module(raw,'dorado_control_certified_fall_step')+'\n'+module(raw,'dorado_control_hold_ordered_step')+'\n'
        code+=WRAPPER.replace('FUNCTIONS',fn).replace('ARRIVAL',str(arrival))
        path.write_text(code)
        return dict(scope=__doc__,source_state_sha256=sha(source/'run-state.json'),CP_source_manifest_sha256=sha(cp_source/'manifest.json'),
                    current_atomic_and_terminal_unchanged=not cp_block_capture,
                    refresh_arrival=arrival,output_sha256=sha(path))
    tb=parent.TB
    marker=' reg [397:0] p;reg [155:0] sample;reg found;integer previous_mir,late_captures=0;'
    assert tb.count(marker)==1
    tb=tb[:tb.index(marker)]+PROGRAM
    if repro_block or repro_normal_block:
        first=tb.index(' initial begin',tb.index(' task automatic set_word'))
        tb=tb[:first]+''' initial begin
  seed(0);start();set_word(4,16);event_step(0,p);
  p[292]=0;event_step(1,p);
  $display("CONTROL_CP_BLOCK_REPRO_PASS");
  $display("CONTROL_HOLD PASS CP_block_reproduction=1");$finish;
 end
 always @(reference_pins[292] or ref_sets or original.a.e23.state_q or
          original.a.g15.p2 or original.a.f15.ca or original.a.f15.state_q)
  if(monitor && checks>=128)$display("CP_BLOCK_TRACE t=%0.3f check=%0d root=%b strobe=%b sBLOCK=%b Block_n=%b TPC_D=%b TPC_C=%b TPC_Q=%b predicted=%b prefix_Block=%b",$realtime,checks,root,reference_pins[292],ref_sets[17],original.a.e23.state_q[1],original.a.f15.p7,original.a.f15.ca,original.a.f15.state_q[0],next_q[42],dut.actual.prefix_q[93]);
endmodule
'''
        if repro_normal_block:
            tb=tb.replace('seed(0);start();set_word(4,16);event_step(0,p);\n  p[292]=0;event_step(1,p);',
                          'seed(0);start();p=reference_pins;p[263]=0;event_step(0,p);\n  event_step(1,p);',1)
            tb=tb.replace('CONTROL_CP_BLOCK_REPRO_PASS','CONTROL_NORMAL_BLOCK_REPRO_PASS',1)
            tb=tb.replace('CP_block_reproduction=1','normal_block_reproduction=1',1)
    tb=tb.replace('wire [93:0] flags,ref_flags;','wire [93:0] flags,ref_flags;wire [17:0] ref_sets;',1)
    assert tb.count('reference,clocks,ref_flags);')==1
    tb=tb.replace('reference,clocks,ref_flags);','reference,clocks,ref_flags,ref_sets);',1)
    marker='   if(ref_flags!==expected_flags)'
    assert tb.count(marker)==1
    tb=tb.replace(marker,'   if(ref_sets!==dut.after_cp_pins[281:264])$fatal(1,"Control CP source set-level mismatch check=%0d",checks);\n'+marker,1)
    # The parent runner recognizes its original summary prefix only. Preserve
    # that marker after the stronger directed-source coverage has passed.
    tb=tb.replace('  $finish;','  $display("CONTROL_HOLD PASS CP_source_closed=1");$finish;',1)
    with patch.object(parent,'generate',generate),patch.object(parent,'candidate_generate',candidate),patch.object(parent,'TB',tb):
        rc=parent.run(out,engine,profile)
    state=json.loads((out/'run-state.json').read_text())
    state.update(experiment='actual_CP_set_decoders_in_original_control_graph',CP_source_closed=True,
                 all_original_Q_clock_flag_checks_retained=True,normal_memory_MIR_data_preserved=True,
                 source_epoch_order_qualification_pending=True,block_reproduction_only=repro_block)
    state.update(CP_TPC_block_capture_enabled=cp_block_capture,CP_TPC_block_capture_descriptor=descriptor,
                 forced_CP_TPC_block_epoch=forced_early,normal_block_reproduction_only=repro_normal_block,
                 bad_normal_block_epoch=bad_normal_block)
    (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','cp-source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus')
    p.add_argument('--profile',choices=parent.PROFILES,default='later_control')
    group=p.add_mutually_exclusive_group()
    group.add_argument('--repro-block',action='store_true')
    group.add_argument('--repro-normal-block',action='store_true')
    p.add_argument('--cp-block-capture',action='store_true')
    p.add_argument('--force-early',type=int,choices=[0,1])
    p.add_argument('--bad-normal-block',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.cp_source.resolve(),a.output.resolve(),a.engine,a.profile,a.repro_block,a.cp_block_capture,a.force_early,a.repro_normal_block,a.bad_normal_block))
