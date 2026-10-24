#!/usr/bin/env python3
"""Compare all active clock events with exact absolute-time original references.

Both simulators check every offset/mask/post-event level through the complete
300us joint period at all eight CPU rotations, with resets and transport
pauses. Five source/ordering corruptions must fail after successful compilation.
No native consumer, frequency change, FPGA timing or firmware pass follows.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from audit_native_primary_clocks import absolute_reference, TICKS, INTERVAL
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
TB = '''`timescale 1ns/1ps
module tb_primary_clock_events;
 reg clk=0,reset=1,advance=0;
 reg [194:0] expected[0:159999];
 wire [194:0] observed[0:7];
 string reference_path;
 integer samples=0,held_checks=0,resets=0,maximum=0;
 for(genvar p=0;p<8;p++)begin: rotations
  wire [5:0] before_levels,after_levels;
  wire [2:0] count;
  wire [107:0] offsets;
  wire [35:0] masks,levels;
  dorado_primary_clock_events #(.INITIAL_CPU_PHASE(p)) actual(
   .clk(clk),.reset(reset),.advance(advance),.levels_before(before_levels),.levels_after(after_levels),
   .event_count(count),.offsets(offsets),.masks(masks),.levels(levels));
  assign observed[p]={before_levels,after_levels,count,offsets,masks,levels};
 end
 task pulse;
  begin clk=0;#5;clk=1;#5;clk=0;#1;end
 endtask
 task restart;
  begin reset=1;advance=0;pulse;reset=0;#1;resets=resets+1;end
 endtask
 task sample(input integer tick);
  begin
   advance=1;#1;
   for(integer p=0;p<8;p++)begin
    if(observed[p] !== expected[p*20000+tick])
     $fatal(1,"PRIMARY_CLOCK mismatch rotation=%0d tick=%0d got=%h expected=%h",p,tick,observed[p],expected[p*20000+tick]);
    if(int'(observed[p][182:180])>maximum)maximum=int'(observed[p][182:180]);
    samples=samples+1;
   end
   pulse;
  end
 endtask
 task hold_transport;
  reg [5:0] saved[0:7];
  begin
   advance=0;#1;
   for(integer p=0;p<8;p++)saved[p]=observed[p][194:189];
   repeat(3)begin
    pulse;
    for(integer p=0;p<8;p++)begin
     if(observed[p][194:189] !== saved[p] || observed[p][188:183] !== saved[p] || observed[p][182:0] !== 183'b0)
      $fatal(1,"PRIMARY_CLOCK held source advanced rotation=%0d",p);
     held_checks=held_checks+1;
    end
   end
  end
 endtask
 initial begin
  if(!$value$plusargs("expected=%s",reference_path))$fatal(1,"Missing reference path");
  $readmemh(reference_path,expected);
  for(integer trial=0;trial<3;trial++)begin
   restart;
   for(integer tick=0;tick<17+7*trial;tick++)begin
    sample(tick);
    if(tick%5==0)hold_transport;
   end
  end
  restart;
  for(integer tick=0;tick<20000;tick++)begin
   sample(tick);
   if(tick%97==0)hold_transport;
  end
  if(maximum!=5 || resets!=4 || held_checks<5000)$fatal(1,"PRIMARY_CLOCK incomplete coverage");
  $display("PRIMARY_CLOCK_PASS rotations=8 intervals=20000 samples=%0d joint_period_ns=300000 maximum=%0d resets=%0d held_checks=%0d",samples,maximum,resets,held_checks);
  $finish;
 end
endmodule
'''
MUTATIONS = {
    'drop-bb': ("candidate_mask[4]=6'b010000;", "candidate_mask[4]=6'b000000;"),
    'drop-ether': ("candidate_mask[5]=6'b100000;", "candidate_mask[5]=6'b000000;"),
    'round-ether': ('candidate_time[5]=ether_remaining;', "candidate_time[5]=(ether_remaining/18'd11765)*18'd11765;"),
    'capacity-four': ('record<6;record++', 'record<4;record++'),
    'advance-held': ('else if(advance)begin', "else if(1'b1)begin"),
}


def run(audit, out):
    assert not out.exists()
    a = json.loads((audit/'results.json').read_text())
    assert a['status'] == 'PASS_SOURCE_BOUND_ALL_PRIMARY_CLOCK_SCHEDULE_AUDIT'
    assert a['compared_intervals'] == 160000 and a['required_record_capacity'] == 5
    for name, digest in a['source_hashes'].items():
        assert sha(audit/name) == digest
    out.mkdir(parents=True)
    inputs = out/'inputs'
    inputs.mkdir()
    for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        shutil.copy2(ROOT/'verilog/rtl'/name, inputs/name)
    assert sha(inputs/'dorado_clock_event_batch.sv') == a['source_hashes']['cpu-pixel-source.sv']
    (inputs/'tb.sv').write_text(TB)
    shutil.copy2(ROOT/'tools/audit_native_primary_clocks.py', inputs/'audit_native_primary_clocks.py')
    shutil.copy2(audit/'results.json', inputs/'source-audit.json')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    with (inputs/'expected.hex').open('x') as stream:
        for rotation in range(8):
            cursor, buckets = absolute_reference(rotation)
            assert len(buckets) == TICKS
            for records in buckets:
                before, offsets, masks, levels = cursor, 0, 0, 0
                for i, (offset, mask) in enumerate(records):
                    assert i < 6 and 0 < offset <= INTERVAL
                    cursor ^= mask
                    offsets |= offset << (18*i)
                    masks |= mask << (6*i)
                    levels |= cursor << (6*i)
                packed = (before << 189) | (cursor << 183) | (len(records) << 180) | (offsets << 72) | (masks << 36) | levels
                stream.write(format(packed, '049x')+'\n')
    raw = (inputs/'dorado_primary_clock_events.sv').read_text()
    for variant, (before, after) in MUTATIONS.items():
        assert raw.count(before) == 1, variant
        changed = raw.replace(before, after, 1)
        assert changed.replace(after, before, 1) == raw
        (inputs/(variant+'.sv')).write_text(changed)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        audit=str(audit), audit_sha256=sha(audit/'results.json'),
        inputs={p.name: sha(p) for p in inputs.iterdir() if p.is_file()}, cases=[],
        active_sources=['CPU SCP/ECP', '50MHz pixel', '10MHz BaseBoard', '23530kHz Ethernet'],
        compared_nominal_intervals=160000, joint_period_ns=300000, native_consumers_integrated=False,
        native_RTL_changed=False, hardware_changed=False, FPGA_timing_qualified=False, speedup_qualified=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for engine in ['icarus', 'verilator']:
        for variant in ['positive', *MUTATIONS]:
            d = out/(engine+'-'+variant)
            d.mkdir()
            actual = inputs/('dorado_primary_clock_events.sv' if variant == 'positive' else variant+'.sv')
            paths = [inputs/'dorado_oscillator_events.sv', inputs/'dorado_clock_event_batch.sv', actual, inputs/'tb.sv']
            expected = '+expected='+str(inputs/'expected.hex')
            if engine == 'icarus':
                commands = [['iverilog', '-g2012', '-s', 'tb_primary_clock_events', '-o', str(d/'test.vvp'), *map(str, paths)],
                            ['vvp', '-i', str(d/'test.vvp'), expected]]
            else:
                commands = [['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal', '--top-module', 'tb_primary_clock_events',
                             '--Mdir', str(d/'obj'), '-o', 'test', *map(str, paths)], [str(d/'obj/test'), expected]]
            case = dict(engine=engine, variant=variant, expected_rejection=variant != 'positive', actions=[])
            state['cases'].append(case)
            save()
            for label, command in zip(['compile', 'test'], commands):
                with (d/(label+'.log')).open('x') as log:
                    p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
                    action = dict(label=label, command=command, pid=p.pid, started=time.time())
                    case['actions'].append(action)
                    save()
                    action.update(returncode=p.wait(), finished=time.time())
                    save()
                if action['returncode'] != 0:
                    break
            log = (d/'test.log').read_text() if (d/'test.log').exists() else ''
            if variant == 'positive':
                passed = action['returncode'] == 0 and 'PRIMARY_CLOCK_PASS rotations=8 intervals=20000 samples=160576 joint_period_ns=300000 maximum=5 resets=4' in log
            else:
                passed = case['actions'][0]['returncode'] == 0 and action['returncode'] != 0 and (
                    'PRIMARY_CLOCK mismatch' in log or 'PRIMARY_CLOCK held source advanced' in log)
            case.update(passed=passed, log_sha256=sha(d/'test.log') if (d/'test.log').exists() else None)
            save()
            print(engine, variant, 'PASS' if passed else 'FAIL', flush=True)
            if not passed:
                state.update(status='TERMINAL_PRIMARY_CLOCK_MATRIX_NOT_ESTABLISHED', finished=time.time())
                save()
                return 1
    state.update(status='PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES', finished=time.time(),
        original_CPU_pixel_producer_unchanged=True, all_five_corruptions_rejected_by_both_engines=True)
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--audit', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    raise SystemExit(run(a.audit.resolve(), a.output.resolve()))
