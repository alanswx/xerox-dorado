#!/usr/bin/env python3
"""Inspect otherwise-unused front-end diagnostics from a read-only native sample.

Replay one captured combinational evaluation in Icarus using the exact frozen
component and its captured physical RAM inputs. No live process is contacted or
changed. This checks capture consistency and exposes unsettled/repeated flags;
it is not an independent oracle for processor behavior or a firmware gate.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inspect(run, observation, out):
    assert not out.exists(), 'Preserve the existing observation review'
    provenance = json.loads(gzip.decompress((run / 'provenance.json.gz').read_bytes()))
    record = json.loads(observation.read_text())
    root = record['root']
    prefix = 'tb_native_cedar__DOT__dut__DOT__m__DOT__'
    files = prefix + 'r_front_evaluator__DOT__child__DOT__child__DOT__files__DOT__'
    operands = root['tb_native_cedar__DOT__operands']
    physical_r = (operands >> 48) & 0xffff
    raw_r = int(f'{physical_r:016b}'[::-1], 2)
    state = root[prefix + 'r_front_lower_q'] | (raw_r << 572)
    state |= root[prefix + 'r_front_upper_q'] << 588
    expected = root[prefix + 'r_front_next']
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    shutil.copy2(observation, inputs / 'observation.json')
    shutil.copy2(Path(__file__), inputs / Path(__file__).name)
    hashes = {}
    sources = [name for name in provenance['used_hdl'] if name.startswith('src/verilog/rtl/')]
    assert 'src/verilog/rtl/dorado_r_front_ready_step.sv' in sources
    assert 'src/verilog/rtl/dorado_r_front_cone.sv' in sources
    for relative in sources:
        name = Path(relative).name
        source = run / 'inputs' / relative
        assert digest(source) == provenance['inputs'][relative], relative
        shutil.copy2(source, inputs / name)
        hashes[relative] = digest(source)
    for name in ['rm_h', 'stk_h', 'rm_l', 'stk_l']:
        words = root[files + name]
        assert len(words) == 256 and all(0 <= word < 512 for word in words)
        (inputs / (name + '.hex')).write_text(''.join(f'{word:03x}\n' for word in words))
    bench = f'''`timescale 1ns/1ps
module tb;
reg host_clk=0;
wire [751:0] next_q;
wire [58:0] events,repeated;
wire unsettled;
dorado_r_front_ready_step dut(
 .host_clk(host_clk),.advance(1'b0),.state_q(752'h{state:x}),
 .clock_before(1'b{root[prefix + 'r_front_clock_before']}),
 .clock_after(1'b{root[prefix + 'CLK_ph_p___05F_BaseBd']}),
 .pins_before(246'h{root[prefix + 'r_front_before']:x}),
 .pins_after(246'h{root[prefix + 'r_front_live']:x}),
 .state_next(next_q),.clock_events(events),.repeated_events(repeated),.unsettled(unsettled));
initial begin
 $readmemh("rm_h.hex",dut.child.child.files.rm_h);
 $readmemh("stk_h.hex",dut.child.child.files.stk_h);
 $readmemh("rm_l.hex",dut.child.child.files.rm_l);
 $readmemh("stk_l.hex",dut.child.child.files.stk_l);
 #10;
 $display("FRONT_OBSERVATION next=%h expected=%h events=%h repeated=%h unsettled=%b",next_q,752'h{expected:x},events,repeated,unsettled);
 if(next_q!==752'h{expected:x}) $fatal(1,"captured evaluation is not consistent with settled component inputs");
 $display("FRONT_OBSERVATION_CONSISTENT");$finish;
end
endmodule
'''
    (inputs / 'tb.sv').write_text(bench)
    command = ['iverilog', '-g2012', '-s', 'tb', '-o', str(out / 'observation.vvp'),
               str(inputs / 'tb.sv'), *[str(inputs / Path(n).name) for n in sources]]
    compiled = subprocess.run(command, capture_output=True, text=True)
    (out / 'compile.log').write_text(compiled.stdout + compiled.stderr)
    assert compiled.returncode == 0, compiled.stderr
    tested = subprocess.run(['vvp', str(out / 'observation.vvp')], cwd=inputs,
                            capture_output=True, text=True)
    log = tested.stdout + tested.stderr
    (out / 'test.log').write_text(log)
    result = dict(status='CONSISTENT_NATIVE_FRONT_OBSERVATION' if tested.returncode == 0
                  else 'INCONSISTENT_NATIVE_FRONT_OBSERVATION',
                  scope=__doc__, observation_sha256=digest(observation),
                  tool_sha256=digest(Path(__file__)),
                  host_ticks=root['tb_native_cedar__DOT__host_ticks'],
                  source_hashes=hashes, compile_command=command,
                  compile_returncode=compiled.returncode, test_returncode=tested.returncode,
                  native_process_changed=False, firmware_qualified=False)
    if tested.returncode == 0:
        import re
        match = re.search(r'events=(\w+) repeated=(\w+) unsettled=([01])', log)
        assert match and 'FRONT_OBSERVATION_CONSISTENT' in log
        result.update(events=int(match[1], 16), repeated=int(match[2], 16),
                      unsettled=bool(int(match[3])))
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))
    return tested.returncode


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', type=Path, required=True)
    parser.add_argument('--observation', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    raise SystemExit(inspect(args.run.resolve(), args.observation.resolve(), args.output.resolve()))
