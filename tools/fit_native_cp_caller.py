#!/usr/bin/env python3
"""Time the source-proven owned caller at15ns, retaining every result field.

Unlike the earlier generic shell, before-pins/root and828closed owners are
actual commit registers. The other16visible-R bits remain external, as in
the real backplane.420live inputs replace1651independent generic inputs; all
1401result indices, the original15ns SDC and four SRAM banks remain. This
scope is the native caller slice, not a passing generic fit or full core.
"""
import argparse
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from report_r_front_late_hold_fit import TCL
from test_r_front_forward_view import sha
from test_r_front_task_event import replace

BP='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
ROOT=Path(__file__).resolve().parents[1]


def prepare(stage,proof,generic_fit,out,priority_proof=None,priority_comparisons=()):
    assert not out.exists()
    s=json.loads((stage/'r-front-control-native.json').read_text())
    assert s['owned_context_specialization_requested']and not s['assembly_permitted']
    for name,h in s['source_hashes'].items():assert sha(stage/name)==h,name
    p=json.loads((proof/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT'and p['unbounded_induction_proven']
    assert p['native_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert p['owner_power_up_state']=='unconstrained'and not p['input_or_protocol_assumptions_added']
    for name,h in p['source_files'].items():assert sha(proof/name)==h,name
    assert sha(proof/'proof.log')==p['proof_log_sha256']
    f=json.loads((generic_fit/'provenance.json').read_text())
    assert f['direct_cp_projection']and f['settling_waves']==5 and f['result_bits']==1401
    assert (generic_fit/'clock.sdc').read_text()==fixture.SDC
    bp=(stage/BP).read_text()
    block=re.search(r'  always @\(posedge storage_clk\)if\(contb_advance && control_ready\)begin\n    r_front_lower_q.*?\n  end',bp,re.S)[0]
    held=re.search(r'  assign r_front_held=([^;]+);',bp)[0]
    top=(generic_fit/'top.sv').read_text()
    top=replace(top,'input wire [1650:0] pins,output reg [1400:0] result',
                     'input wire [419:0] pins,output reg [1400:0] result')
    top=replace(top,'reg [1650:0] input_stage,launch;', 'reg [419:0] input_stage,launch;')
    top=replace(top,''' assign {q,before_clock,after_clock,before_controls,after_controls,advance,refresh_before_capture}=launch[1649:0];
 wire cp_reset=launch[1650];wire cp_ready;''',
        ''' wire cp_reset,cp_ready;wire [15:0] visible_r;
 assign {visible_r,after_controls,after_clock,advance,cp_reset}=launch;
 assign refresh_before_capture=1'b0;''')
    owners=''' // Exact real native owner/snapshot block; no guest state importer.
 wire storage_clk=host_clk,contb_advance=advance,control_ready=cp_ready;
 wire CLK_ca_p_=after_clock;
 wire [843:0] r_front_next=next_q;
 wire [400:0] r_front_live=after_controls;
 reg [571:0] r_front_lower_q;reg [255:0] r_front_upper_q;
 reg [400:0] r_front_before;reg r_front_clock_before;
 wire [7:0] r_front_visible_r_ProcH=visible_r[7:0],r_front_visible_r_ProcL=visible_r[15:8];
 wire [843:0] r_front_held;
 OWNER_BLOCK
 HELD_EXPRESSION
 assign q=r_front_held;
 assign before_controls=r_front_before;assign before_clock=r_front_clock_before;
'''.replace('OWNER_BLOCK',block).replace('HELD_EXPRESSION',held)
    top=replace(top,' dorado_r_front_control_step evaluator(',owners+' dorado_r_front_control_step evaluator(')
    original_result=re.search(r' always @\(posedge host_clk\) result<=([^;]+);',(generic_fit/'top.sv').read_text())[0]
    assert original_result in top
    rtl={Path(n).name:(stage/n).read_text()for n in s['extra_RTL_sources']}
    assert len(rtl)==30
    priority=None
    if priority_proof is not None:
        from balance_control_priority import transform
        from fit_cp_prefix_cache import gate
        priority=json.loads((priority_proof/'run-state.json').read_text())
        assert priority['status']=='PASS_BALANCED_PRIORITY_ALL_ATOMIC_OUTPUTS'
        assert not priority['assumptions_added']and not priority['broken_low_bit_control']
        assert priority['state_and_input_bits']==955 and priority['atomic_result_bits']==363
        assert [(r['waves'],r['passed'])for r in priority['results']]==[(5,True),(24,True)]
        for name,h in priority['files'].items():assert sha(priority_proof/name)==h,name
        original=rtl['dorado_r_front_control_step.sv']
        assert priority['original_sha256']==sha(stage/'src/verilog/rtl/dorado_r_front_control_step.sv')
        assert (priority_proof/'original.sv').read_text()==original
        rewritten=transform(original)
        assert (priority_proof/'candidate.sv').read_text()==rewritten
        assert len(priority_comparisons)==2
        gates=[]
        for source,engine in zip(priority_comparisons,['icarus','verilator']):
            state,checked=gate(source,engine)
            assert state['balanced_task_priority']and state['direct_cp_projection']and state['settling_waves']==5
            gates.append(checked)
        assert gates[0]['candidate_sha256']==gates[1]['candidate_sha256']
        rtl['dorado_r_front_control_step.sv']=rewritten
        priority=dict(directory=str(priority_proof),state_sha256=sha(priority_proof/'run-state.json'),
                      atomic_results=priority['results'],complete_original_comparisons=gates,
                      source_change='Only two priority_raw function bodies; actual owner/capture/cache logic identical')
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'for name in rtl))
    audit=TCL.replace('late-hold-flat-v5-reports','native-owned-caller-reports').replace(r'launch\[388\]',r'launch\[389\]')
    files={**rtl,'top.sv':top,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,
           'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':audit}
    out.mkdir(parents=True)
    for name,raw in files.items():(out/name).write_text(raw)
    for name in [Path(__file__).name,'fit_control_clock.py','prove_native_cp_context.py',
                 *(['balance_control_priority.py']if priority else[])]:
        (out/name).write_bytes((ROOT/'tools'/name).read_bytes())
    record=dict(status='PREPARED_SOURCE_PROVEN_NATIVE_OWNED_CALLER_FIT',scope=__doc__,
        stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        context_proof=str(proof),context_proof_sha256=sha(proof/'run-state.json'),
        generic_fixture_source=str(generic_fit),generic_full15ns_rejection_retained=True,
        component='native_owned_caller',clock_period_ns=15,input_staging_bits=420,launch_bits=420,result_bits=1401,
        closed_native_owner_bits=828,external_visible_R_bits=16,saved_before_pins_and_root_bits=402,
        physical_Q_bits=844,ram_bits=9216,physical_SRAM_banks=4,
        original_result_indices_retained=[0,1400],original_result_assignment_byte_identical=True,
        owner_commit_block_byte_identical=True,SDC_byte_identical=True,added_timing_exceptions=False,
        assembly_permitted=False,native_qualified=False,physical_timing_qualified=False,speedup_qualified=False,
        balanced_priority_proof=priority,
        files={path.name:sha(path)for path in out.iterdir()if path.is_file()})
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['stage','proof','generic-fit','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--priority-proof',type=Path)
    parser.add_argument('--priority-comparison',action='append',type=Path,default=[])
    a=parser.parse_args();prepare(a.stage.resolve(),a.proof.resolve(),a.generic_fit.resolve(),a.output.resolve(),
        a.priority_proof.resolve()if a.priority_proof else None,[p.resolve()for p in a.priority_comparison])
