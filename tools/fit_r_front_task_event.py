#!/usr/bin/env python3
"""Fit the shared physical f14 capture event in the unchanged15ns shell.

Require two full balanced independent-original comparisons and retain the
entire1400-bit result interface, input staging, RAM arrays and constraints.
This measures a component budget; it authorizes no native assembly/load.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import fit_control_clock as fixture

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(parent,runs,out):
    assert not out.exists()and len(runs)==2
    gates=[];changed={}
    for engine,d in zip(['icarus','verilator'],runs):
        r=json.loads((d/'run-state.json').read_text());log=(d/'test.log').read_text()
        assert r['engine']==engine and r['status']=='PASS_R_FRONT_TASK_EVENT' and not r['mutation']
        assert r['actions'][-1]['returncode']==0 and r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        assert r['independent_reference_and_bench_unchanged']and r['task_data_epoch_unchanged']
        assert r['canonical_clock']==dict(board='ContA',package='f14',joint_channel=12,child_channel=22,event_and_repeat_forwarded=True)
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
        for row in r['source_files']:
            p=d/row['path'];assert sha(p)==row['output_sha256']
            if row['changed']:
                key=p.name
                if key in changed:assert sha(changed[key])==sha(p)
                changed[key]=p
        gates.append(dict(directory=str(d),engine=engine,summary=r['summary'],clocks=clocks,run_state_sha256=sha(d/'run-state.json')))
    assert gates[0]['clocks']==gates[1]['clocks']and len(changed)==10
    record=json.loads((parent/'provenance.json').read_text())
    assert record['component']=='r_front_control_closed_read_windows'and record['clock_period_ns']==15
    for n,h in record['files'].items():assert sha(parent/n)==h,n
    assert(parent/'clock.sdc').read_text()==fixture.SDC
    out.mkdir(parents=True)
    for n in record['files']:(out/n).write_bytes((parent/n).read_bytes())
    for name,p in changed.items():
        target='dorado_r_front_control_step.sv'if name=='candidate.sv'else name
        assert(out/target).exists(),target
        (out/target).write_bytes(p.read_bytes())
    for n in [Path(__file__).name,'test_r_front_task_event.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    result=dict(scope=__doc__,component='r_front_control_canonical_task_event',clock_period_ns=15,physical_Q_bits=844,ram_bits=9216,
                input_staging_bits=1650,launch_bits=1650,result_bits=1400,functional_gates=gates,
                parent_provenance_sha256=sha(parent/'provenance.json'),quartus_genvar_syntax=record['quartus_genvar_syntax'],
                input_and_result_shell_unchanged=sha(out/'top.sv')==sha(parent/'top.sv'),
                tested_candidate_sha256=sha(out/'dorado_r_front_control_step.sv'),
                files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False,
                full_producer_arrival_contract_qualified=False,all_profiles_qualified=False)
    assert result['input_and_result_shell_unchanged']
    (out/'provenance.json').write_text(json.dumps(result,indent=2)+'\n');print('PREPARED_CANONICAL_TASK_EVENT_15NS_COMPONENT_BUDGET',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','icarus','verilator','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),[a.icarus.resolve(),a.verilator.resolve()],a.output.resolve())
