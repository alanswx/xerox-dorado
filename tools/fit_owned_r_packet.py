#!/usr/bin/env python3
"""Fit generic/specialized versions of the SAME native-owned packet caller.

422 real external input bits replace1652 arbitrary context/owner inputs:
828 closed owners plus401 pins/one root now have the actual native publication.
Keep16 external R aliases, one9216-bit SRAM and all4943 output timing endpoints.
Compare these two owned fixtures; do not compare I/O/area with the old fixture
as though their arbitrary input domains were identical. No native selection.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from generate_owned_r_packet import caller,cache
from generate_parallel_r_fault import packet
from generate_cached_r_packet import stage
from test_r_front_forward_view import sha

TOP='''module clock_fit(input wire host_clk,input wire [421:0] pins,output reg [4942:0] result);
 reg [421:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 wire reset,advance,use_pair,ca,refresh;wire [400:0] pa;wire [15:0] visible_r;
 assign {reset,advance,use_pair,ca,refresh,pa,visible_r}=launch;
 wire [2469:0] first_result,last_result;wire ready;wire [1:0] forwarded;wire [843:0] held_q;
 dorado_r_front_owned_packet actual(host_clk,advance,use_pair,ca,refresh,reset,pa,
  visible_r[7:0],visible_r[15:8],first_result,last_result,ready,forwarded,held_q);
 always @(posedge host_clk)result<={first_result,last_result,ready,forwarded};
endmodule
'''


def checked(directory):
    s=json.loads((directory/'run-state.json').read_text())
    for n,h in s['files'].items():assert sha(directory/n)==h
    assert s['mutation'] is None and s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS'
    assert s['all100_clock_counts_match'] and s['all_original_assertions_retained']
    assert s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1'
    assert s['trace_links']['records']==14884 and s['trace_links']['discontinuities']==0
    assert s['actions'][-1]['returncode']==0 and sha(directory/'test.log')==s['test_log_sha256']
    return s


def prepare(tests,peer,out):
    assert not out.exists();s=checked(tests);other=checked(peer)
    assert s['specialized']==other['specialized'] and {s['engine'],other['engine']}=={'icarus','verilator'}
    assert s['summary']==other['summary'] and s['proof_state_sha256']==other['proof_state_sha256']
    proof=Path(s['proof']);p=json.loads((proof/'run-state.json').read_text())
    assert sha(proof/'run-state.json')==s['proof_state_sha256'] and p['status']=='PASS_SOURCE_BOUND_OWNED_PACKET_CANONICAL_CONTEXT'
    for n,h in p['files'].items():assert sha(proof/n)==h
    for row in p['cases']:
        assert row['passed'] and row['returncode']==0
        d=proof/row['mode'];assert sha(d/'proof.log')==row['log_sha256']
        for n,h in row['files'].items():assert sha(d/n)==h
    raw=(tests/'candidate.sv').read_text();bp=(proof/'native-backplane.v').read_text()
    assert (tests/'caller.sv').read_text()==caller(raw,bp)
    assert (tests/'packet.sv').read_text()==packet(raw)+'\n'+stage(raw,True)+'\n'+cache(raw,s['specialized'])
    out.mkdir(parents=True);paths=[row['path'] for row in s['source_files']]+['packet.sv','forward.sv','caller.sv']
    for n in paths:
        dest=out/n;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(tests/n,dest)
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in paths))
    audit=fixture.AUDIT.replace('CONTROL_CLOCK_','OWNED_PACKET_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=4943} {error "Incomplete owned packet results"}
 puts "OWNED_PACKET_OUTPUT_BITS [get_collection_size $ports]"
 set inputs [get_ports {pins[*]}]
 if {[get_collection_size $inputs]!=422} {error "Incorrect actual caller input domain"}
 puts "OWNED_PACKET_INPUT_BITS [get_collection_size $inputs]"
 set launches [get_registers {*launch*}]''')
    for n,v in {'top.sv':TOP,'clock_fit.qsf':qsf,'clock.sdc':fixture.SDC,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':audit}.items():(out/n).write_text(v)
    for n in ['fit_control_clock.py',Path(__file__).name,'generate_owned_r_packet.py','test_owned_r_packet.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    record=dict(scope=__doc__,clock_period_ns=15,input_bits=422,result_bits=4943,stored_architectural_owner_bits=828,owned_input_snapshot_bits=402,
        original_external_R_alias_bits=16,single_physical_SRAM_bits=9216,specialized=s['specialized'],source_cache_state_bits=1416,
        tests=str(tests),tests_state_sha256=sha(tests/'run-state.json'),peer=str(peer),peer_state_sha256=sha(peer/'run-state.json'),
        proof_state_sha256=sha(proof/'run-state.json'),four_state_state_sha256=s['four_state_state_sha256'],bad_unknown_state_sha256=s['bad_unknown_state_sha256'],
        prefix_key_unchanged=True,whole_packet_final_root_retained=True,refresh_comparison_retained=True,
        assembly_permitted=False,native_qualified=False,source_device_epochs_qualified=False,hardware_changed=False,hardware_requires_user_release=True,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('PREPARED_OWNED_PACKET_422_INPUT_4943_OUTPUT_15NS',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['tests','peer','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.tests.resolve(),a.peer.resolve(),a.output.resolve())
