#!/usr/bin/env python3
"""Prove exact clock-record merging and added crystal phase state separately.

The merge proof extracts the actual combinational body byte-for-byte. Its
independent reference ranks events by distinct preceding timestamps, rather
than repeatedly selecting minima. The predicate states the existing source
interface geometry, not a guest protocol assumption. Phase induction extracts
the actual counter/level update and compares independently scaled10MHz and
23530kHz phases for arbitrary reset/advance histories. Original CPU/pixel
induction and full absolute-time simulator references remain separate gates.
No native consumer, firmware, clock-rate or FPGA timing qualification follows.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from test_r_front_forward_view import sha

MERGE_PORTS = '''module actual_merge(
 input wire reset,advance,input wire [5:0] levels_before,
 input wire [2:0] inner_count,input wire [15:0] inner_offsets,inner_masks,
 input wire bb_due,ether_due,input wire [18:0] bb_remaining,input wire [17:0] ether_remaining,
 output reg [5:0] levels_after,output reg [2:0] event_count,
 output reg [107:0] offsets,output reg [35:0] masks,levels);
 localparam [17:0] INTERVAL=18'd141180,UNUSED=18'h3ffff;
BODY
endmodule
'''
MERGE_TOP = '''module merge_equivalence(
 input wire reset,advance,input wire [5:0] levels_before,
 input wire [2:0] inner_count,input wire [15:0] inner_offsets,inner_masks,
 input wire bb_due,ether_due,input wire [18:0] bb_remaining,input wire [17:0] ether_remaining,
 output wire equivalent,valid,output wire [194:0] actual,expected);
 wire [5:0] actual_after;
 wire [2:0] actual_count;
 wire [107:0] actual_offsets;
 wire [35:0] actual_masks,actual_levels;
 actual_merge dut(reset,advance,levels_before,inner_count,inner_offsets,inner_masks,
  bb_due,ether_due,bb_remaining,ether_remaining,actual_after,actual_count,actual_offsets,actual_masks,actual_levels);
 reg [17:0] t[0:5];reg [5:0] m[0:5];reg used[0:5];integer rank[0:5];
 reg [5:0] cursor,joined;
 reg [2:0] count;
 reg [107:0] offsets;
 reg [35:0] masks,levels;
 reg geometry,already,found;
 reg [17:0] timestamp;
 always @*begin
  geometry=(inner_count<=4) && (!bb_due || (bb_remaining>0 && bb_remaining<=141180)) &&
   (!ether_due || (ether_remaining>0 && ether_remaining<=141180));
  for(integer i=0;i<6;i++)begin t[i]=0;m[i]=0;used[i]=0;rank[i]=0;end
  for(integer i=0;i<4;i++)begin
   used[i]=(i<int'(inner_count));t[i]=18'(inner_offsets[4*i+:4])*18'd11765;
   m[i]={2'b0,inner_masks[4*i+:4]};
   if(used[i])begin
    geometry=geometry && inner_offsets[4*i+:4]>0 && inner_offsets[4*i+:4]<=12 && m[i]!=0;
    if(i>0)geometry=geometry && inner_offsets[4*i+:4]>inner_offsets[4*(i-1)+:4];
   end
  end
  used[4]=bb_due;t[4]=18'(bb_remaining);m[4]=16;
  used[5]=ether_due;t[5]=ether_remaining;m[5]=32;
  // Rank by the number of DISTINCT active source times before this event.
  for(integer i=0;i<6;i++)for(integer j=0;j<6;j++)begin
   already=0;
   for(integer k=0;k<6;k++)if(k<j && used[k] && t[k]==t[j])already=1;
   if(used[i] && used[j] && t[j]<t[i] && !already)rank[i]=rank[i]+1;
  end
  cursor=levels_before;count=0;offsets=0;masks=0;levels=0;joined=0;timestamp=0;found=0;
  if(advance && !reset)for(integer r=0;r<6;r++)begin
   joined=0;timestamp=0;found=0;
   for(integer i=0;i<6;i++)if(used[i] && rank[i]==r)begin joined=joined|m[i];timestamp=t[i];found=1;end
   if(found)begin
    cursor=cursor^joined;offsets[18*r+:18]=timestamp;masks[6*r+:6]=joined;levels[6*r+:6]=cursor;count=count+1;
   end
  end
 end
 assign valid=!advance || reset || geometry;
 assign actual={levels_before,actual_after,actual_count,actual_offsets,actual_masks,actual_levels};
 assign expected={levels_before,cursor,count,offsets,masks,levels};
 assign equivalent=!valid || actual==expected;
endmodule
'''
PHASE = '''module phase_equivalence(input wire clk,reset,advance,output wire equivalent);
ACTUAL_DECLS
ACTUAL_UPDATE
 reg [3:0] bb_reference=0;
 reg [13:0] ether_reference=0;
 reg bb_reference_level=0,ether_reference_level=0;
 wire [4:0] bb_reference_sum={1'b0,bb_reference}+5'd3;
 wire [14:0] ether_reference_sum={1'b0,ether_reference}+15'd7059;
 wire bb_reference_due=bb_reference_sum>=10;
 wire ether_reference_due=ether_reference_sum>=10000;
 always @(posedge clk)begin
  if(reset)begin bb_reference<=0;ether_reference<=0;bb_reference_level<=0;ether_reference_level<=0;end
  else if(advance)begin
   bb_reference<=4'(bb_reference_due ? bb_reference_sum-5'd10 : bb_reference_sum);
   ether_reference<=14'(ether_reference_due ? ether_reference_sum-15'd10000 : ether_reference_sum);
   bb_reference_level<=bb_reference_level^bb_reference_due;
   ether_reference_level<=ether_reference_level^ether_reference_due;
  end
 end
 assign equivalent=bb_reference<10 && ether_reference<10000 &&
  bb_phase==19'(bb_reference)*19'd47060 && ether_phase==18'(ether_reference)*18'd20 &&
  bb_level==bb_reference_level && ether_level==ether_reference_level &&
  bb_due==bb_reference_due && ether_due==ether_reference_due &&
  (!bb_due || bb_remaining==19'(10-bb_reference)*19'd47060) &&
  (!ether_due || ether_remaining==18'(10000-ether_reference)*18'd20);
endmodule
'''


def extract(raw, mode):
    if mode == 'merge':
        begin = raw.index(' reg [17:0] candidate_time')
        end = raw.index(' always @(posedge clk)begin', begin)
        body = raw[begin:end]
        assert raw[:begin]+body+raw[end:] == raw
        return MERGE_PORTS.replace('BODY', body), MERGE_TOP
    quotient = ' reg [3:0] bb_phase_tenths=0;' in raw
    begin = raw.index(' reg [3:0] bb_phase_tenths=0;' if quotient else ' reg [18:0] bb_phase=0;')
    end = raw.index(' assign levels_before=', begin)
    clock_begin = raw.index(' always @(posedge clk)begin', end)
    clock_end = raw.index('\nendmodule', clock_begin)
    declarations, update = raw[begin:end], raw[clock_begin:clock_end]
    assert raw[:begin]+declarations+raw[end:clock_begin]+update+raw[clock_end:] == raw
    result = PHASE.replace('ACTUAL_DECLS', declarations).replace('ACTUAL_UPDATE', update)
    if quotient:
        # Prove the implementation range as a conjunction, never an assumption.
        result = result.replace(' assign equivalent=', ' assign equivalent=bb_phase_tenths<10 && ', 1)
    return result, None


def run(tests, out, yosys):
    assert not out.exists()
    t = json.loads((tests/'run-state.json').read_text())
    assert t['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES' and len(t['cases']) == 12
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    assert all(c['passed'] for c in t['cases'])
    out.mkdir(parents=True)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        tests=str(tests), tests_state_sha256=sha(tests/'run-state.json'), cases=[],
        merged_output_bits=195, merged_clock_lanes=6, native_consumers_integrated=False,
        physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    cases = [('merge', v) for v in ['positive', 'drop-bb', 'drop-ether', 'round-ether', 'capacity-four']]
    cases += [('phase', 'positive'), ('phase', 'advance-held')]
    for mode, variant in cases:
        d = out/(mode+'-'+variant)
        d.mkdir()
        name = 'dorado_primary_clock_events.sv' if variant == 'positive' else variant+'.sv'
        raw = (tests/'inputs'/name).read_text()
        actual, reference = extract(raw, mode)
        (d/'source.sv').write_text(raw)
        (d/'actual.sv').write_text(actual)
        if reference:
            (d/'reference.sv').write_text(reference)
        positive = variant == 'positive'
        top = 'merge_equivalence' if mode == 'merge' else 'phase_equivalence'
        sources = '/work/actual.sv /work/reference.sv' if reference else '/work/actual.sv'
        strategy = ('-seq 2 -tempinduct -maxsteps 6' if positive else '-seq 4') if mode == 'phase' else ''
        flag = 'verify' if positive else 'falsify'
        script = f'''read_verilog -sv {sources}
hierarchy -check -top {top}
proc
flatten
memory_map
opt
check -assert
sat {strategy} -timeout 120 -prove equivalent 1 -{flag} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env = os.environ.copy()
        env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
        env['YOWASP_MOUNT'] = '/work='+str(d)
        command = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
        with (d/'proof.log').open('x') as log:
            proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row = dict(mode=mode, variant=variant, command=command, pid=proc.pid, started=time.time(),
                files={p.name: sha(p) for p in d.iterdir() if p.is_file() and p.suffix != '.log'})
            state['cases'].append(row)
            save()
            code = proc.wait()
        log = (d/'proof.log').read_text()
        marker = ('Induction step proven: SUCCESS!' if mode == 'phase' else 'no model found: SUCCESS!') if positive else 'model found: FAIL!'
        passed = code == 0 and marker in log
        row.update(returncode=code, passed=passed, finished=time.time(), log_sha256=sha(d/'proof.log'))
        save()
        print(mode, variant, 'PASS' if passed else 'FAIL', flush=True)
        if not passed:
            state.update(status='TERMINAL_PRIMARY_CLOCK_PROOF_NOT_ESTABLISHED', finished=time.time())
            save()
            return 1
    state.update(status='PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS', finished=time.time(),
        merge_proven_for_all_valid_source_records=True,
        crystal_phase_unbounded_induction=True, reset_advance_protocol_assumptions=False)
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['tests', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    raise SystemExit(run(a.tests.resolve(), a.output.resolve(), a.yosys.resolve()))
