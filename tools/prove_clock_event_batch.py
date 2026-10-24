#!/usr/bin/env python3
"""Prove the common CPU/pixel batch for arbitrary reset/advance histories.

An independent original four-flop divider and1.25ns pixel counter compare
every ordered timestamp/mask/level at ALL eight initial CPU rotations.
State relations strengthen temporal induction; no assumptions are added.
Observational aliases invert byte-for-byte to the simulator-tested source.
This qualifies the source schedule, not native board consumers or FPGA timing.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from test_oscillator_events import sha

CASE = '''module clock_batch_case #(parameter integer PHASE=0)(input wire clk,reset,advance,output wire equivalent);
 wire [3:0] before_levels,after_levels;
 wire [2:0] count,actual_phase;
 wire [15:0] offsets,masks,levels;
 wire [31:0] actual_remainder;
 dorado_clock_event_batch #(.INITIAL_CPU_PHASE(PHASE)) actual(
  .clk(clk),.reset(reset),.advance(advance),.levels_before(before_levels),.levels_after(after_levels),
  .event_count(count),.offsets(offsets),.masks(masks),.levels(levels),
  .observed_cpu_phase(actual_phase),.observed_pixel_remainder(actual_remainder));
 function automatic [4:0] divider_step(input [4:0] s);
  reg [4:0] n;
  begin n=s;n[4]=!s[4];
   if(s[4])begin n[1]=s[3];n[0]=s[1];end
   else begin n[3]=!s[2];n[2]=s[3];end
   divider_step=n;
  end
 endfunction
 function automatic [4:0] divider_at(input [2:0] p);
  reg [4:0] s;
  begin s=5'b10001;for(integer i=0;i<8;i++)if(i<int'(p))s=divider_step(s);divider_at=s;end
 endfunction
 function automatic [3:0] source_levels(input [4:0] s,input pixel);
  source_levels={pixel,!s[3]|s[0],s[0],!s[3]};
 endfunction
 reg [4:0] divider=divider_at(3'(PHASE)),next_divider;
 reg [2:0] phase=3'(PHASE);
 reg [3:0] pixel_phase=0,next_pixel_phase;
 reg [3:0] cursor,next_levels,changed;
 reg [15:0] expected_offsets,expected_masks,expected_levels;
 reg [2:0] expected_count;
 always @*begin
  cursor=source_levels(divider,pixel_phase[3]);next_divider=divider;next_pixel_phase=pixel_phase;
  expected_count=0;expected_offsets=0;expected_masks=0;expected_levels=0;
  next_levels=cursor;changed=0;
  if(advance && !reset)for(integer step=1;step<=12;step++)begin
   next_pixel_phase=next_pixel_phase+4'd1;
   if(step%3==0)next_divider=divider_step(next_divider);
   next_levels=source_levels(next_divider,next_pixel_phase[3]);changed=cursor^next_levels;
   if(changed!=0)begin
    expected_offsets[4*expected_count+:4]=4'(step);
    expected_masks[4*expected_count+:4]=changed;
    expected_levels[4*expected_count+:4]=next_levels;
    expected_count=expected_count+3'd1;
   end
   cursor=next_levels;
  end
 end
 always @(posedge clk)begin
  if(reset)begin divider<=divider_at(3'(PHASE));pixel_phase<=0;phase<=3'(PHASE);end
  else if(advance)begin divider<=next_divider;pixel_phase<=next_pixel_phase;phase<=phase+3'd4;end
 end
 assign equivalent=(actual_phase==phase) && (divider==divider_at(phase)) &&
  (phase[1:0]==2'(PHASE)) && (pixel_phase[1:0]==0) &&
  (count<=4) && ((advance && !reset) ? count>=2 : count==0) &&
  (actual_remainder==32'(pixel_phase[2:0])*32'd500000) &&
  (before_levels==source_levels(divider,pixel_phase[3])) &&
  ({count,offsets,masks,levels,after_levels}==
   {expected_count,expected_offsets,expected_masks,expected_levels,cursor});
endmodule
module clock_batch_equivalence(input wire clk,reset,advance,output wire equivalent);
 wire [7:0] checks;
 for(genvar p=0;p<8;p++)begin: rotations
  clock_batch_case #(.PHASE(p)) check_case(clk,reset,advance,checks[p]);
 end
 assign equivalent=&checks;
endmodule
'''


def run(tests, out, yosys, variant):
    assert not out.exists()
    s = json.loads((tests/'run-state.json').read_text())
    assert s['status'] == 'PASS_COMMON_CPU_PIXEL_EVENT_BATCH'
    for name, digest in s['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    raw = (tests/'inputs'/(variant+'.sv')).read_text()
    old = ' output reg [15:0] offsets,masks,levels\n'
    new = ''' output reg [15:0] offsets,masks,levels,
 output wire [2:0] observed_cpu_phase,output wire [31:0] observed_pixel_remainder
'''
    before = '.interval_units(pixel_interval));'
    after = '.interval_units(pixel_interval),.observed_remainder(observed_pixel_remainder));'
    alias = ' assign observed_cpu_phase=cpu_phase;\n'
    assert raw.count(old) == raw.count(before) == 1
    exposed = raw.replace(old, new, 1).replace(before, after, 1).replace(' function automatic scp_at', alias+' function automatic scp_at', 1)
    assert exposed.replace(new, old, 1).replace(after, before, 1).replace(alias, '', 1) == raw
    pixel = (tests/'inputs/dorado_oscillator_events.sv').read_text()
    old_pixel = '    output wire [31:0] first_offset_units,second_offset_units,interval_units\n'
    new_pixel = '    output wire [31:0] first_offset_units,second_offset_units,interval_units,observed_remainder\n'
    pixel_alias = '    assign observed_remainder=remainder;\n'
    observed_pixel = pixel.replace(old_pixel, new_pixel, 1).replace('    wire active=advance', pixel_alias+'    wire active=advance', 1)
    assert observed_pixel.replace(new_pixel, old_pixel, 1).replace(pixel_alias, '', 1) == pixel and observed_pixel != pixel
    out.mkdir(parents=True)
    for name, text in [('original.sv', raw), ('actual.sv', exposed), ('pixel-original.sv', pixel),
                       ('pixel-observed.sv', observed_pixel), ('reference.sv', CASE)]:
        (out/name).write_text(text)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    positive = variant == 'positive'
    strategy = '-seq 2 -tempinduct -maxsteps 6' if positive else '-seq 4'
    script = f'''read_verilog -sv /work/pixel-observed.sv /work/actual.sv /work/reference.sv
hierarchy -check -top clock_batch_equivalence
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove equivalent 1 -{'verify' if positive else 'falsify'} -show-ports -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        variant=variant, tests_state_sha256=sha(tests/'run-state.json'), cpu_rotations=8,
        interval_ns=15, time_quantum_ps=1250, assumptions_added=False,
        exact_observational_alias_inverse=True, native_consumer_qualified=False,
        physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()})

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    env = os.environ.copy()
    env['YOWASP_MOUNT'] = '/work='+str(out)
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    save()
    with (out/'proof.log').open('x') as log:
        p = subprocess.Popen([str(yosys), '-Q', '-T', '-s', '/work/proof.ys'], env=env, stdout=log, stderr=subprocess.STDOUT)
        state['pid'] = p.pid
        save()
        code = p.wait()
    log = (out/'proof.log').read_text()
    expected = 'Induction step proven: SUCCESS!' if positive else 'SAT proof finished - model found: FAIL!'
    passed = code == 0 and expected in log
    state.update(status=('PASS_COMMON_CLOCK_BATCH_INDUCTION' if positive else 'PASS_CLOCK_BATCH_COUNTEREXAMPLE')
                 if passed else 'TERMINAL_CLOCK_BATCH_PROOF_NOT_ESTABLISHED', returncode=code,
                 unbounded_induction_proven=passed and positive, finished=time.time(), proof_log_sha256=sha(out/'proof.log'))
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['tests', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--variant', choices=['positive', 'drop-second', 'late-cpu', 'lose-coincident', 'advance-held', 'reverse-order'], default='positive')
    a = p.parse_args()
    raise SystemExit(run(a.tests.resolve(), a.output.resolve(), a.yosys.resolve(), a.variant))
