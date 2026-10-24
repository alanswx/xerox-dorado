#!/usr/bin/env python3
"""Stage a complete single/pair processor/one-SRAM15ns implementation budget.

Keep both2470-bit subevent results, ready and forwarding. Preserve all actual
processor/control/latch logic and the one four-bank SRAM owner. The registered
fixture shell adds no architectural guest stage. No assembly/native selection,
input timestamp qualification or Dorado-speed claim follows from this fit.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
TOP='''module clock_fit(input wire host_clk,input wire [1650:0] pins,output reg [4942:0] result);
 reg [1650:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 wire advance,use_pair,cb,ca,refresh;wire [843:0] held_q;wire [400:0] pb,pa;
 assign {advance,use_pair,held_q,cb,ca,pb,pa,refresh}=launch;
 wire [2469:0] first_result,last_result;wire ready;wire [1:0] forwarded;
 dorado_r_front_packet_step actual(host_clk,advance,use_pair,held_q,cb,ca,pb,pa,refresh,first_result,last_result,ready,forwarded);
 always @(posedge host_clk)result<={first_result,last_result,ready,forwarded};
endmodule
'''


def prepare(tests,proof,out):
    assert not out.exists();s=json.loads((tests/'run-state.json').read_text())
    assert s['status']=='PASS_FULL_PROCESSOR_PACKET_SINGLE_COMMIT' and s['mutation'] is None
    p=json.loads((proof/'run-state.json').read_text());assert p['status']=='PASS_ORIGINAL_BANK_WRITE_FORWARDING'
    assert len(p['cases'])==3 and all(v['passed'] for v in p['cases'])
    paths=[]
    for row in s['source_files']:
        assert sha(tests/row['path'])==row['output_sha256'];paths.append(row['path'])
    for name,h in s['files'].items():assert sha(tests/name)==h
    out.mkdir(parents=True)
    for name in [*paths,'packet.sv','forward.sv']:
        dest=out/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(tests/name,dest)
    sources=[*paths,'packet.sv','forward.sv']
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in sources))
    report=fixture.AUDIT.replace('CONTROL_CLOCK_','PROCESSOR_PACKET_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=4943} {error "Incomplete first/last packet output coverage"}
 puts "PROCESSOR_PACKET_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {*launch*}]''')
    for name,text in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':report}.items():
        (out/name).write_text(text)
    for name in ['fit_control_clock.py',Path(__file__).name]:shutil.copy2(ROOT/'tools'/name,out/name)
    r=dict(scope=__doc__,clock_period_ns=15,input_bits=1651,result_bits=4943,subevent_result_bits=2470,
        all844_closed_and_physical_Qs_retained=True,clock_channels_per_subevent=100,physical_SRAM_banks=4,physical_SRAM_bits=9216,
        tests=str(tests),tests_state_sha256=sha(tests/'run-state.json'),forwarding_proof=str(proof),forwarding_state_sha256=sha(proof/'run-state.json'),
        assembly_permitted=False,native_qualified=False,input_timestamps_or_real60ns_qualified=False,
        hardware_changed=False,hardware_requires_user_release=True,
        files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n');print('PREPARED_FULL_PROCESSOR_PACKET_15NS_COMPONENT',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['tests','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.tests.resolve(),a.proof.resolve(),a.output.resolve())
