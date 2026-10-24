#!/usr/bin/env python3
"""Compare complete serial and parallel control fall/rise budgets at15ns.

Preserve all363 intermediate-fall and363 final combined result bits. Use the
same555-bit registered input shell and unwaived15ns/2ns-I/O constraints for
both variants. The sparse/fused function requires the proved canonical entry
and stable pins; this does NOT implement a whole packet's fallback/cache/RAM
ownership or qualify the complete processor, original60ns cadence or speed.
"""
import argparse
import json
from pathlib import Path
import shutil

import fit_control_clock as fixture
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
COMMON='''module dorado_control_fall_rise(
 input wire [155:0] q,input wire [397:0] pins,input wire refresh,
 output wire [362:0] falling_result,combined_result);
 wire [155:0] fall_q,rise_q;
 wire [55:0] fall_events,fall_repeated,rise_events,rise_repeated;
 wire fall_unsettled,rise_unsettled;
 wire [93:0] fall_flags,rise_flags;
 IMPLEMENTATION
 assign falling_result={fall_q,fall_events,fall_repeated,fall_unsettled,fall_flags};
 assign combined_result={final_q,fall_events|rise_events,
  fall_repeated|rise_repeated|(fall_events&rise_events),fall_unsettled|rise_unsettled,rise_flags};
endmodule
'''
SERIAL=''' dorado_control_hold_atomic_step #(.WAVES(5)) falling(q,1'b1,1'b0,pins,pins,
  fall_q,fall_events,fall_repeated,fall_unsettled,refresh,fall_flags);
 dorado_control_hold_atomic_step #(.WAVES(5)) rising(fall_q,1'b0,1'b1,pins,pins,
  rise_q,rise_events,rise_repeated,rise_unsettled,refresh,rise_flags);
 wire [155:0] final_q=rise_q;
'''
PARALLEL=''' dorado_control_fall_step falling(q,pins,refresh,
  fall_q,fall_events,fall_repeated,fall_unsettled,fall_flags);
 dorado_control_hold_atomic_step #(.WAVES(5)) rising(q,1'b0,1'b1,pins,pins,
  rise_q,rise_events,rise_repeated,rise_unsettled,refresh,rise_flags);
 wire [155:0] low_initial_q={q[155:31],1'b0,q[29:0]};
 wire [155:0] low_rise_q;
 dorado_control_hold_atomic_step #(.WAVES(5)) low_cram_rise(low_initial_q,1'b0,1'b1,pins,pins,
  low_rise_q,,,,refresh,);
 wire [155:0] final_q={rise_q[155:31],low_rise_q[30]|fall_q[30],rise_q[29:0]};
'''
TOP='''module clock_fit(input wire host_clk,input wire [554:0] pins,output reg [725:0] result);
 reg [554:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 wire [362:0] falling,combined;
 dorado_control_fall_rise actual(launch[554:399],launch[398:1],launch[0],falling,combined);
 always @(posedge host_clk)result<={falling,combined};
endmodule
'''


def prepare(proof,audit,out,variant):
    assert not out.exists() and variant in ['serial','parallel']
    p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_CERTIFIED_FALL_AND_PARALLEL_RISE_FULL_CONTROL'
    assert len(p['cases'])==11 and all(c['passed'] for c in p['cases'])
    for name,digest in p['files'].items():
        assert sha(proof/name)==digest
    for row in p['cases']:
        d=proof/f"{row['mode']}-{row['variant']}-w{row['waves']}"
        for name,digest in row['files'].items():
            assert sha(d/name)==digest
        assert sha(d/'proof.log')==row['log_sha256']
    a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_ROOT_BINDING_AND_PACKET_GEOMETRY'
    assert a['intervals']==160000 and a['CPU_rotations']==8
    for name,digest in a['files'].items():
        assert sha(audit/'inputs'/name)==digest
    out.mkdir(parents=True)
    for name in ['atomic.sv','dorado_control_fall_step.sv']:
        shutil.copy2(proof/name,out/name)
    (out/'fusion.sv').write_text(COMMON.replace('IMPLEMENTATION',SERIAL if variant=='serial' else PARALLEL))
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n' for n in ['atomic.sv','dorado_control_fall_step.sv','fusion.sv']))
    report=fixture.AUDIT.replace('CONTROL_CLOCK_','CONTROL_FUSION_').replace(' set launches [get_registers {*launch*}]',
        ''' set ports [get_ports {result[*]}]
 if {[get_collection_size $ports]!=726} {error "Incomplete fall/final output coverage"}
 puts "CONTROL_FUSION_OUTPUT_BITS [get_collection_size $ports]"
 set launches [get_registers {*launch*}]''')
    for name,text in {'top.sv':TOP,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
                      'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':report}.items():
        (out/name).write_text(text)
    for name in ['fit_control_clock.py',Path(__file__).name]:
        shutil.copy2(ROOT/'tools'/name,out/name)
    r=dict(scope=__doc__,variant=variant,clock_period_ns=15,input_bits=555,result_bits=726,
        intermediate_bits=363,final_bits=363,physical_control_Q_bits=156,clock_channels=56,flags=94,
        proof=str(proof),proof_state_sha256=sha(proof/'run-state.json'),
        audit=str(audit),audit_state_sha256=sha(audit/'results.json'),
        domain='actual identity prefix at HIGH root, identical pins through both edges; other transactions require the original path',
        assembly_permitted=False,native_qualified=False,complete_processor_or_packet_cache_qualified=False,
        firmware_qualified=False,speedup_qualified=False,hardware_changed=False,hardware_requires_user_release=True,
        files={p.name:sha(p) for p in out.iterdir() if p.is_file()})
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n')
    print('PREPARED_COMPLETE_CONTROL_FALL_RISE_COMPONENT',variant,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['proof','audit','output']:
        p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--variant',choices=['serial','parallel'],required=True)
    a=p.parse_args();prepare(a.proof.resolve(),a.audit.resolve(),a.output.resolve(),a.variant)
