#!/usr/bin/env python3
"""Compare actual owned cache histories, including unknown publication data.

Power-up owners are unconstrained in the theorem; each independent trial starts
with one concrete power-up assignment. Cache reset never initializes owners.
After a known canonical commit, publish X/Z through each of844 next-owner,
401 pin and one final-root bits; also test external R aliases and refresh.
Compare all1416 cache bits and every observable cache interface using !==.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from generate_owned_r_packet import cache
from prove_owned_r_packet import observed,make_top
from test_r_front_forward_view import sha

TB='''`timescale 1ns/1ps
module tb_owned_context;
 reg clk=0,reset=0,advance=0,guard=1,pair=0,root=1,refresh=0,settled=1;
 reg [843:0] next_held=0;reg [400:0] pins=0;reg [15:0] external_r=0;
 reg [268:0] prefix=0;
 wire equivalent,interfaces_equal,original_ready,native_ready,original_fast,native_fast;
 wire original_hit,native_hit,original_matches,native_matches;
 owned_packet_cache_equivalence dut(clk,reset,advance,guard,pair,root,refresh,settled,
  next_held,pins,external_r[7:0],external_r[15:8],prefix,
  equivalent,interfaces_equal,original_ready,native_ready,original_fast,native_fast,
  original_hit,native_hit,original_matches,native_matches);
 integer checks=0,trials=0,unknown_context=0,external_cases=0,known_cases=0;
 reg unknown_phase=0;
 reg [31:0] seed=32'h8c56f039;
 reg [843:0] base_owner;reg [400:0] base_pins;
 task automatic compare;
  begin
   #1;
   if({dut.original_ready,dut.original_fast,dut.original_hit,dut.original_saved,dut.original_cp,dut.original_after} !==
      {dut.native_ready,dut.native_fast,dut.native_hit,dut.native_saved,dut.native_cp,dut.native_after})begin
    $display("OWNED_CONTEXT original=%b/%b/%b native=%b/%b/%b match=%b/%b owner=%h tag=%h pins=%h tagpins=%h roots=%b/%b",dut.original_ready,dut.original_fast,dut.original_hit,dut.native_ready,dut.native_fast,dut.native_hit,dut.original.canonical_match,dut.actual.canonical_match,dut.current_q,dut.original.canonical_q,dut.r_front_before,dut.original.canonical_pins,dut.r_front_clock_before,dut.original.canonical_root);
    $fatal(1,"OWNED_CONTEXT interface mismatch trial=%0d root=%b unknown_phase=%b",trials,root,unknown_phase);
   end
   if(dut.original_state!==dut.native_state)$fatal(1,"OWNED_CONTEXT state mismatch trial=%0d",trials);
   checks++;
  end
 endtask
 task automatic edge_clock;
  begin #1;clk=1;#1;clk=0;compare;end
 endtask
 task automatic fresh_trial;
  begin
   // Independent arbitrary architectural power-up, never a DUT reset feature.
   unknown_phase=0;clk=0;reset=1;advance=0;guard=1;pair=0;root=1;refresh=0;settled=1;
   next_held=base_owner;pins=base_pins;external_r=base_owner[587:572];prefix={269{1'b1}};
   dut.r_front_lower_q=base_owner[571:0];dut.r_front_upper_q=base_owner[843:588];
   dut.r_front_before=base_pins;dut.r_front_clock_before=1;
   #1;edge_clock;reset=0;#1;edge_clock;
   if(original_ready!==1 || native_ready!==1)$fatal(1,"OWNED_CONTEXT preparation");
   advance=1;edge_clock;advance=0;compare;
   if(original_fast!==1 || native_fast!==1)$fatal(1,"OWNED_CONTEXT canonical priming");
   trials++;
  end
 endtask
 initial begin
  for(integer background=0;background<2;background++)begin
   for(integer b=0;b<844;b++)begin
    seed={seed[30:0],seed[31]^seed[21]^seed[1]^seed[0]};base_owner[b]=background ? seed[0]:0;
   end
   for(integer b=0;b<401;b++)begin
    seed={seed[30:0],seed[31]^seed[21]^seed[1]^seed[0]};base_pins[b]=background ? seed[0]:0;
   end
   fresh_trial;pair=1;root=1;advance=1;edge_clock;advance=0;compare;known_cases++;
   if(dut.r_front_clock_before!==1)$fatal(1,"OWNED_CONTEXT lost final root");
   // The entire final context publishes on the same edge as the cache tag.
   for(integer b=0;b<1246;b++)begin
    for(integer mode=0;mode<2;mode++)begin
     fresh_trial;unknown_phase=1;
     if(b<844)next_held[b]=mode ? 1'bz:1'bx;
     else if(b<1245)pins[b-844]=mode ? 1'bz:1'bx;
     else root=mode ? 1'bz:1'bx;
     // Inputs may become unknown before an accepted edge; known cache entry
     // is sufficient to accept this abstract cache-level publication.
     compare;advance=1;edge_clock;advance=0;compare;
     // A later idle edge must retain exactly the same four-state cache data.
     edge_clock;unknown_context++;
    end
   end
   for(integer b=0;b<17;b++)begin
    for(integer mode=0;mode<2;mode++)begin
     fresh_trial;
     if(b<16)external_r[b]=mode ? 1'bz:1'bx;else refresh=mode ? 1'bz:1'bx;
     compare;edge_clock;external_cases++;
    end
   end
  end
  if(unknown_context!=4984 || external_cases!=68 || known_cases!=2)$fatal(1,"OWNED_CONTEXT coverage");
  $display("OWNED_CONTEXT_FOUR_STATE_PASS context_cases=%0d external_cases=%0d known_cases=%0d trials=%0d checks=%0d",unknown_context,external_cases,known_cases,trials,checks);
  $finish;
 end
endmodule
'''


def run(proof,out,mutation):
    assert not out.exists();s=json.loads((proof/'run-state.json').read_text())
    assert s['status']=='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT'
    for n,h in s['files'].items():assert sha(proof/n)==h
    raw=(Path(s['source'])/'candidate.sv').read_text();bp=(proof/'native-backplane.v').read_text()
    out.mkdir(parents=True)
    for n,v in {'generic.sv':observed(cache(raw),'generic_cache'),
                'actual.sv':observed(cache(raw,True,mutation),'specialized_cache'),
                'top.sv':make_top(raw,bp),'tb.sv':TB}.items():(out/n).write_text(v)
    for n in [Path(__file__).name,'generate_owned_r_packet.py','prove_owned_r_packet.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        mutation=mutation,files={p.name:sha(p) for p in out.iterdir()},actions=[],hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,cmd in [('compile',['iverilog','-g2012','-s','tb_owned_context','-o',str(out/'test.vvp'),*[str(out/n) for n in ['generic.sv','actual.sv','top.sv','tb.sv']]]),
                      ('test',['vvp','-i',str(out/'test.vvp')])]:
        with (out/(label+'.log')).open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=child.pid,started=time.time());state['actions'].append(row);save();code=child.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=state['actions'][0]['returncode']==0 and ((code!=0 and 'OWNED_CONTEXT interface mismatch' in log and 'unknown_phase=1' in log) if mutation else
        (code==0 and 'OWNED_CONTEXT_FOUR_STATE_PASS context_cases=4984 external_cases=68 known_cases=2' in log))
    state.update(status=('PASS_OWNED_CONTEXT_UNKNOWN_COUNTEREXAMPLE' if mutation else 'PASS_OWNED_CONTEXT_FOUR_STATE') if passed else 'TERMINAL_OWNED_CONTEXT_FOUR_STATE_NOT_ESTABLISHED',
        finished=time.time(),test_log_sha256=sha(out/'test.log') if (out/'test.log').exists() else None);save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--mutation',choices=['drop-unknown-fallback'])
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.mutation))
