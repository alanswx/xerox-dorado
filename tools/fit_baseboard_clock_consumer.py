#!/usr/bin/env python3
"""Budget the complete primary source plus first board consumer at15ns.

Retain all195 producer bits and126 consumer bits at every CPU rotation.
Physical-Q/debug inputs use the same registered input shell; all2568 outputs
remain visible. Require the full two-engine original-package test matrix.
This is a component budget, not6502/RIOT/full-native timing or firmware proof.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
TOP = '''module clock_fit(input wire host_clk,input wire [15:0] pins,output reg [2567:0] result);
 reg [15:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 genvar p;
 generate for(p=0;p<8;p=p+1)begin: rotations
  wire [5:0] before_levels,after_levels,next_q;
  wire [2:0] count;
  wire [107:0] offsets;
  wire [35:0] masks,levels;
  wire [41:0] states;
  wire [48:0] events;
  wire [6:0] roots,maxes,pres,auxes;
  wire invalid;
  dorado_primary_clock_events #(.INITIAL_CPU_PHASE(p)) source(
   host_clk,launch[1],launch[0],before_levels,after_levels,count,offsets,masks,levels);
  dorado_baseboard_clock_step consumer(launch[7:2],before_levels,after_levels,count,levels,
   launch[8],launch[9],launch[15:10],launch[0]&&!launch[1],next_q,states,events,roots,maxes,pres,auxes,invalid);
  always @(posedge host_clk)result[321*p+:321]<={before_levels,after_levels,count,offsets,masks,levels,
   invalid,auxes,pres,maxes,roots,events,states,next_q};
 end endgenerate
endmodule
'''


def prepare(tests, out, candidate=None, consumer_proofs=None, source_proofs=None):
    assert not out.exists()
    t = json.loads((tests/'run-state.json').read_text())
    assert t['status'] == 'PASS_BASEBOARD_CLOCK_EVENT_CONSUMER_REFERENCES' and len(t['cases']) == 12
    assert all(c['passed'] for c in t['cases'])
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    names = ['dorado_oscillator_events.sv', 'dorado_clock_event_batch.sv',
             'dorado_primary_clock_events.sv', 'dorado_baseboard_clock_step.sv']
    for name in names:
        path = candidate/name if candidate is not None and (candidate/name).exists() else ROOT/'verilog/rtl'/name
        assert sha(path) == t['inputs'][name]
    proof_record = None
    top = TOP
    parallel = 'state_q,source_before,source_after,' in (tests/'inputs/dorado_baseboard_clock_step.sv').read_text()
    if parallel:
        assert consumer_proofs is not None and source_proofs is not None
        c = json.loads((consumer_proofs/'run-state.json').read_text())
        s = json.loads((source_proofs/'run-state.json').read_text())
        assert c['status'] == 'PASS_SINGLE_BB_TRANSFER_FULL_CONSUMER_EQUIVALENCE'
        if candidate is not None:
            assert c['stage_state_sha256'] == sha(candidate/'results.json')
        assert c['compared_bits'] == 126 and len(c['cases']) == 3
        assert all(row['passed'] for row in c['cases'])
        for proof_dir, state in [(consumer_proofs, c), (source_proofs, s)]:
            for row in state['cases']:
                d = proof_dir/(row['mode']+'-'+row['variant'] if 'mode' in row else row['variant'])
                assert sha(d/'proof.log') == row['log_sha256']
                for name, digest in row['files'].items():
                    assert sha(d/name) == digest
        assert sha(consumer_proofs/'positive/actual.sv') == t['inputs']['dorado_baseboard_clock_step.sv']
        assert s['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS'
        assert len(s['cases']) == 7 and all(row['passed'] for row in s['cases'])
        assert s['at_most_one_BB_transition_per_interval_proven'] and s['independent_BB_and_Ether_endpoints_proven']
        assert sha(source_proofs/'merge-positive/source.sv') == t['inputs']['dorado_primary_clock_events.sv']
        assert s['tests_state_sha256'] == sha(Path(t['schedule'])/'run-state.json')
        assert t['schedule_state_sha256'] == s['tests_state_sha256']
        proof_record = dict(consumer_proofs=str(consumer_proofs), consumer_state_sha256=sha(consumer_proofs/'run-state.json'),
                            source_proofs=str(source_proofs), source_state_sha256=sha(source_proofs/'run-state.json'))
    else:
        assert consumer_proofs is None and source_proofs is None
        top = top.replace('consumer(launch[7:2],before_levels,after_levels,count', 'consumer(launch[7:2],before_levels,count', 1)
    out.mkdir(parents=True)
    qsf = fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in names))
    audit = fixture.AUDIT.replace('CONTROL_CLOCK_', 'BB_CONSUMER_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=2568} {error "Incomplete producer/consumer output coverage"}
 puts "BB_CONSUMER_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {*launch*}]''')
    for name, text in {'top.sv': top, 'clock.sdc': fixture.SDC, 'clock_fit.qsf': qsf,
                       'clock_fit.qpf': 'PROJECT_REVISION = "clock_fit"\n', 'audit.tcl': audit}.items():
        (out/name).write_text(text)
    for name in names:
        shutil.copy2(tests/'inputs'/name, out/name)
    for name in ['fit_control_clock.py', Path(__file__).name]:
        shutil.copy2(ROOT/'tools'/name, out/name)
    p = dict(scope=__doc__, tests=str(tests), tests_state_sha256=sha(tests/'run-state.json'),
             clock_period_ns=15, CPU_rotations=8, result_bits=2568,
             producer_bits_per_rotation=195, consumer_bits_per_rotation=126,
             assembly_permitted=False, native_qualified=False, native_consumers_integrated=False,
             physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
             files={p.name: sha(p) for p in out.iterdir() if p.is_file()})
    p.update(candidate=str(candidate) if candidate else None, full_output_equivalence=proof_record)
    (out/'provenance.json').write_text(json.dumps(p, indent=2)+'\n')
    print('PREPARED_COMPLETE_BASEBOARD_CONSUMER_15NS_COMPONENT', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['tests', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    for name in ['candidate', 'consumer-proofs', 'source-proofs']:
        p.add_argument('--'+name, type=Path)
    a = p.parse_args()
    prepare(a.tests.resolve(), a.output.resolve(), a.candidate.resolve() if a.candidate else None,
            a.consumer_proofs.resolve() if a.consumer_proofs else None,
            a.source_proofs.resolve() if a.source_proofs else None)
