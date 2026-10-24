#!/usr/bin/env python3
"""Prepare all eight primary-clock rotations in the established15ns shell.

Require the complete two-engine simulator matrix and all merge/phase proofs.
Keep every195-bit record vector at each rotation, using the same registered
input/result shell, virtual data I/O and unwaived15ns constraints. This is
only a producer implementation budget, never full native closure or speed.
Run the copied unchanged fit_control_clock.py runner on misterubuntu only;
it has no assembly or deployment mode and does not access a MiSTer.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
TOP = '''module clock_fit(input wire host_clk,input wire [1:0] pins,
 output reg [1559:0] result);
 reg [1:0] input_stage;
 reg [1:0] launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 genvar p;
 generate for(p=0;p<8;p=p+1)begin: rotations
  wire [5:0] before_levels,after_levels;
  wire [2:0] count;
  wire [107:0] offsets;
  wire [35:0] masks,levels;
  dorado_primary_clock_events #(.INITIAL_CPU_PHASE(p)) source(
   .clk(host_clk),.reset(launch[1]),.advance(launch[0]),
   .levels_before(before_levels),.levels_after(after_levels),.event_count(count),
   .offsets(offsets),.masks(masks),.levels(levels));
  always @(posedge host_clk)result[195*p+:195]<={before_levels,after_levels,count,offsets,masks,levels};
 end endgenerate
endmodule
'''


def prepare(tests, proofs, out, source=None):
    assert not out.exists()
    t = json.loads((tests/'run-state.json').read_text())
    p = json.loads((proofs/'run-state.json').read_text())
    assert t['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES' and len(t['cases']) == 12
    assert all(c['passed'] for c in t['cases'])
    assert p['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS' and len(p['cases']) == 7
    assert all(c['passed'] for c in p['cases']) and p['tests_state_sha256'] == sha(tests/'run-state.json')
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    out.mkdir(parents=True)
    qsf = fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in [
            'dorado_oscillator_events.sv', 'dorado_clock_event_batch.sv', 'dorado_primary_clock_events.sv']))
    audit = fixture.AUDIT.replace('CONTROL_CLOCK_', 'PRIMARY_CLOCK_')
    audit = audit.replace(' set launches [get_registers {*launch*}]', ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=1560} {error "Missing complete eight-rotation output port coverage"}
 puts "PRIMARY_CLOCK_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {*launch*}]''')
    files = {'top.sv': TOP, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
        'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': audit}
    for name, value in files.items():
        (out/name).write_text(value)
    for name in ['dorado_oscillator_events.sv', 'dorado_clock_event_batch.sv', 'dorado_primary_clock_events.sv']:
        path = source if source is not None and name == 'dorado_primary_clock_events.sv' else ROOT/'verilog/rtl'/name
        assert sha(path) == t['inputs'][name]
        shutil.copy2(tests/'inputs'/name, out/name)
    for name in [Path(__file__).name, 'fit_control_clock.py']:
        shutil.copy2(ROOT/'tools'/name, out/name)
    record = dict(scope=__doc__, component='all_primary_clock_event_producer', clock_period_ns=15,
        input_staging_bits=2, launch_bits=2, result_bits=1560, CPU_rotations=8,
        tests=str(tests), tests_state_sha256=sha(tests/'run-state.json'),
        proofs=str(proofs), proofs_state_sha256=sha(proofs/'run-state.json'),
        source_input_and_output_staging_only=True, original_constraints_unchanged=True,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
        assembly_permitted=False, native_qualified=False, physical_timing_qualified=False,
        firmware_qualified=False, speedup_qualified=False, user_FPGA_untouched=True)
    if source is not None:
        record.update(candidate_source=str(source), candidate_source_sha256=sha(source))
    (out/'provenance.json').write_text(json.dumps(record, indent=2)+'\n')
    print('PREPARED_ALL_PRIMARY_CLOCK_15NS_COMPONENT', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['tests', 'proofs', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--source', type=Path, help='Exact independently qualified primary-merger candidate')
    a = p.parse_args()
    prepare(a.tests.resolve(), a.proofs.resolve(), a.output.resolve(), a.source.resolve() if a.source else None)
