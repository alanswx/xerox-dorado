#!/usr/bin/env python3
"""Specialize only the exact 50MHz/four-sample oscillator remainder state.

Its rational remainder alternates between zero and two million, so one bit
represents it exactly. Preserve every output equation, level update, reset,
pause and other parameter combinations. Prove all136 output/state bits against
the original oscillator for arbitrary reset/advance histories, with an explicit
reachable-state invariant rather than assumptions. A stopped phase is a required
counterexample. Native consumers and physical timing remain separate gates.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from test_r_front_forward_view import sha

OLD_DECL = '    reg [WIDTH-1:0] remainder=0;'
NEW_DECL = '    wire [WIDTH-1:0] remainder;'
STATE_ANCHOR = '    wire active=advance && !reset;'
NEW_STATE = '''    generate if(SAMPLES_PER_INSTRUCTION==4 && FREQUENCY_KHZ==50000)begin: exact_pixel
        reg pixel_half=0;
        assign remainder=pixel_half ? WIDTH'(2000000) : WIDTH'(0);
        always @(posedge clk)begin
            if(reset)pixel_half<=0;
            else if(advance)pixel_half<=!pixel_half;
        end
    end else begin: rational
        reg [WIDTH-1:0] phase_remainder=0;
        assign remainder=phase_remainder;
        always @(posedge clk)begin
            if(reset)phase_remainder<=0;
            else if(advance)phase_remainder<=WIDTH'(reduced);
        end
    end endgenerate
'''
OLD_UPDATE = '''        if(reset)begin remainder<=0;level_before<=0;end
        else if(advance)begin remainder<=WIDTH'(reduced);level_before<=level_after;end'''
NEW_UPDATE = '''        if(reset)begin level_before<=0;end
        else if(advance)begin level_before<=level_after;end'''
TOP = '''module pixel_phase_equivalence(input wire clk,reset,advance,
 output wire equivalent,output wire [135:0] actual,expected);
 wire [31:0] r0,r1,a0,a1,b0,b1,c0,c1;
 wire [1:0] n0,n1;
 wire [5:0] f0,f1;
 pixel_reference original(.clk(clk),.reset(reset),.advance(advance),.edge_count(n0),
 .rising(f0[5]),.falling(f0[4]),.rising_first(f0[3]),.last_at_tick(f0[2]),
 .level_before(f0[1]),.level_after(f0[0]),.first_offset_units(a0),
 .second_offset_units(b0),.interval_units(c0),.observed_remainder(r0));
 pixel_candidate optimized(.clk(clk),.reset(reset),.advance(advance),.edge_count(n1),
 .rising(f1[5]),.falling(f1[4]),.rising_first(f1[3]),.last_at_tick(f1[2]),
 .level_before(f1[1]),.level_after(f1[0]),.first_offset_units(a1),
 .second_offset_units(b1),.interval_units(c1),.observed_remainder(r1));
 assign expected={r0,n0,f0,a0,b0,c0};assign actual={r1,n1,f1,a1,b1,c1};
 assign equivalent=(r0==0 || r0==2000000) && actual==expected;
endmodule
'''


def prepare(source, out):
    assert not out.exists()
    raw = source.read_text()
    assert raw.count(OLD_DECL) == raw.count(OLD_UPDATE) == raw.count(STATE_ANCHOR) == 1
    candidate = raw.replace(OLD_DECL, NEW_DECL, 1).replace(OLD_UPDATE, NEW_UPDATE, 1)
    candidate = candidate.replace(STATE_ANCHOR, NEW_STATE+STATE_ANCHOR, 1)
    assert candidate.replace(NEW_STATE, '', 1).replace(NEW_DECL, OLD_DECL, 1).replace(NEW_UPDATE, OLD_UPDATE, 1) == raw
    out.mkdir(parents=True)
    (out/'original.sv').write_text(raw)
    (out/'dorado_oscillator_events.sv').write_text(candidate)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PREPARED_EXACT_PIXEL_PHASE',
        specialized_samples=4, specialized_frequency_khz=50000,
        original_output_equations_and_level_update_preserved=True,
        other_parameter_recurrence_preserved=True, extra_virtual_cycles=0,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
        physical_timing_qualified=False, firmware_qualified=False), indent=2)+'\n')
    print('PREPARED_EXACT_PIXEL_PHASE', flush=True)


def expose(raw, name):
    old = '    output wire [31:0] first_offset_units,second_offset_units,interval_units\n'
    new = '    output wire [31:0] first_offset_units,second_offset_units,interval_units,observed_remainder\n'
    anchor = '    wire active=advance && !reset;'
    alias = "    assign observed_remainder=32'(remainder);\n"
    assert raw.count(old) == raw.count(anchor) == 1
    result = raw.replace('module dorado_oscillator_events #(', 'module '+name+' #(', 1)
    result = result.replace(old, new, 1).replace(anchor, alias+anchor, 1)
    assert result.replace('module '+name+' #(', 'module dorado_oscillator_events #(', 1).replace(new, old, 1).replace(alias, '', 1) == raw
    return result


def prove(stage, out, yosys, negative):
    assert not out.exists()
    metadata = json.loads((stage/'results.json').read_text())
    for name, digest in metadata['files'].items():
        assert sha(stage/name) == digest
    original = (stage/'original.sv').read_text()
    candidate = (stage/'dorado_oscillator_events.sv').read_text()
    if negative:
        old = 'else if(advance)pixel_half<=!pixel_half;'
        assert candidate.count(old) == 1
        candidate = candidate.replace(old, 'else if(advance)pixel_half<=pixel_half;', 1)
    out.mkdir(parents=True)
    for name, raw in [('original.sv', original), ('candidate.sv', candidate),
                      ('reference.sv', expose(original, 'pixel_reference')),
                      ('actual.sv', expose(candidate, 'pixel_candidate')), ('top.sv', TOP)]:
        (out/name).write_text(raw)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    strategy = '-seq 4 -falsify' if negative else '-seq 2 -tempinduct -maxsteps 6 -verify'
    script = f'''read_verilog -sv /work/reference.sv /work/actual.sv /work/top.sv
hierarchy -check -top pixel_phase_equivalence
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove equivalent 1 -show-ports -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    env = os.environ.copy()
    env['YOWASP_MOUNT'] = '/work='+str(out)
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), stage=str(stage),
                 stage_state_sha256=sha(stage/'results.json'), compared_bits=136,
                 protocol_or_state_assumptions=False, negative_control=negative,
                 files={p.name: sha(p) for p in out.iterdir() if p.is_file()})

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    with (out/'proof.log').open('x') as log:
        process = subprocess.Popen([str(yosys), '-Q', '-T', '-s', '/work/proof.ys'], env=env, stdout=log, stderr=subprocess.STDOUT)
        state['pid'] = process.pid
        save()
        code = process.wait()
    marker = 'model found: FAIL!' if negative else 'Induction step proven: SUCCESS!'
    passed = code == 0 and marker in (out/'proof.log').read_text()
    state.update(status=('PASS_PIXEL_PHASE_COUNTEREXAMPLE' if negative else 'PASS_EXACT_PIXEL_PHASE_INDUCTION')
                 if passed else 'TERMINAL_PIXEL_PHASE_NOT_ESTABLISHED', returncode=code,
                 unbounded_induction_proven=passed and not negative, finished=time.time(), proof_log_sha256=sha(out/'proof.log'))
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path)
    p.add_argument('--stage', type=Path)
    p.add_argument('--yosys', type=Path)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--negative-control', action='store_true')
    args = p.parse_args()
    if args.source:
        assert not args.stage and not args.yosys and not args.negative_control
        prepare(args.source.resolve(), args.output.resolve())
    else:
        assert args.stage and args.yosys
        raise SystemExit(prove(args.stage.resolve(), args.output.resolve(), args.yosys.resolve(), args.negative_control))
