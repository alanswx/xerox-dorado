#!/usr/bin/env python3
"""Measure corrected native844 component sources in the unchanged15ns shell.

Two complete balanced original-reference comparisons establish a bounded
experimental gate. The broader profile matrix and native ROM runs are separate.
All state/events/invariants/functional ports stay registered; no assembly,
deployment, full-machine timing or speed qualification is implied.
"""
import argparse
import hashlib
import json
from pathlib import Path

import fit_control_clock as fixture
from fit_r_front_ready import FIELDS as PARENT_FIELDS
from test_r_front_control_extended_matrix import BUILD,check
from review_r_front_control import read_run

ROOT=Path(__file__).resolve().parents[1]
FIELDS=[(n,{'next_q':844,'events':100,'repeated':100}.get(n,w))for n,w in PARENT_FIELDS]+[
    ('conta_next',110),('shared_state_mismatch',1),('control_flags',94)]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(stage,out,tasking_mask_runs=None):
    assert not out.exists()
    gates=[check(BUILD/f'r-front-control-extended-{e}-v4-20261023',True)for e in ['icarus','verilator']]
    assert gates[0]['clocks']==gates[1]['clocks']
    record=json.loads((stage/'r-front-control-native.json').read_text())
    assert record['pre_event_control_capture']and record['extended_component_directed']
    assert not record['assembly_permitted']and not record['firmware_qualified']
    assert record['owned_read_input_placeholders']==list(range(127,145))
    assert not record['partitioned_component_input_cones']and not record['verilator_split_live_pins']
    for n,h in record['source_hashes'].items():assert sha(stage/n)==h,n
    assert sha(stage/'src/verilog/rtl/dorado_r_front_control_step.sv')=='d8c8f7443887d57739e9abc0896ba283ac07b3d86bfea9bf5d849de35024d7d5'
    stored=[n for n,w in FIELDS if n not in ['source_pins','fa_n']]
    assert sum(w for n,w in FIELDS if n in stored)==1400
    top='''module clock_fit(input wire host_clk,input wire [1649:0] pins,output reg [1399:0] result);
 reg [1649:0] input_stage,launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [843:0] q;
 wire [400:0] before_controls,after_controls;
 wire before_clock,after_clock,advance,refresh_before_capture;
 assign {q,before_clock,after_clock,before_controls,after_controls,advance,refresh_before_capture}=launch;
'''
    top+=''.join(' wire '+(''if w==1 else f'[{w-1}:0] ')+n+';\n'for n,w in FIELDS)
    top+=' dorado_r_front_control_step evaluator(host_clk,advance,q,before_clock,after_clock,before_controls,after_controls,refresh_before_capture,\n  '+','.join(n for n,w in FIELDS)+');\n'
    top+=' always @(posedge host_clk) result<={'+','.join(stored)+'};\nendmodule\n'
    rtl={Path(n).name:stage/n for n in record['extra_RTL_sources']};assert len(rtl)==28
    override=None
    if tasking_mask_runs:
        assert len(tasking_mask_runs)==2
        masked=[]
        for d,e in zip(tasking_mask_runs,['icarus','verilator']):
            r,log=read_run(d)
            assert r['engine']==e and r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['actions'][-1]['returncode']==0
            assert r['masked_tasking_switch_clock']and r['pre_event_control_capture']and r['extended_actual_directed']
            assert not r['solver_rejection_deferred_for_diagnostic']and r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
            assert sha(d/'inputs/tb_r_front_local.sv')==sha(BUILD/f'r-front-control-extended-{e}-v4-20261023/inputs/tb_r_front_local.sv')
            for row in r['adapters']['files']:assert sha(d/'adapters'/row['output'])==sha(stage/'src/verilog/rtl'/row['output'])
            masked.append(dict(directory=str(d),engine=e,candidate_sha256=sha(d/'candidate.sv'),run_state_sha256=sha(d/'run-state.json'),summary=r['summary']))
        assert masked[0]['candidate_sha256']==masked[1]['candidate_sha256']
        assert masked[0]['candidate_sha256']!=sha(rtl['dorado_r_front_control_step.sv'])
        override=dict(native_stage_candidate_sha256=sha(rtl['dorado_r_front_control_step.sv']),replacement_candidate_sha256=masked[0]['candidate_sha256'],balanced_comparisons=masked)
        rtl['dorado_r_front_control_step.sv']=tasking_mask_runs[0]/'candidate.sv'
    qsf=fixture.QSF.replace('set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv\n',
        ''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'for n in rtl))
    out.mkdir(parents=True)
    for n,s in {'top.sv':top,'clock.sdc':fixture.SDC,'clock_fit.qsf':qsf,'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':fixture.AUDIT}.items():(out/n).write_text(s)
    for n,p in rtl.items():(out/n).write_bytes(p.read_bytes())
    for n in ['fit_control_clock.py',Path(__file__).name]:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    (out/'functional-gates.json').write_text(json.dumps(gates,indent=2)+'\n')
    (out/'native-stage-source.json').write_bytes((stage/'r-front-control-native.json').read_bytes())
    r=dict(scope=__doc__,component='r_front_control_old_capture',clock_period_ns=15,physical_Q_bits=844,ram_bits=9216,
           input_staging_bits=1650,launch_bits=1650,result_bits=1400,observation_outputs_registered=False,
           observation_only_outputs=['source_pins','fa_n'],functional_gates=gates,native_stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
           tested_candidate_sha256=sha(out/'dorado_r_front_control_step.sv'),tasking_mask_experiment=override,files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
           assembly_permitted=False,native_qualified=False,full_producer_arrival_contract_qualified=False,all_profiles_qualified=False)
    (out/'provenance.json').write_text(json.dumps(r,indent=2)+'\n');print('PREPARED_BOUNDED_NATIVE844_15NS_COMPONENT_BUDGET',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--tasking-mask-icarus',type=Path);p.add_argument('--tasking-mask-verilator',type=Path)
    a=p.parse_args();assert bool(a.tasking_mask_icarus)==bool(a.tasking_mask_verilator)
    runs=[a.tasking_mask_icarus.resolve(),a.tasking_mask_verilator.resolve()]if a.tasking_mask_icarus else None
    prepare(a.stage.resolve(),a.output.resolve(),runs)
