#!/usr/bin/env python3
"""Measure current physical RbBypass with all1400 registered result fields.

Retain the independent next-transaction forecast check in simulation and add
both current-wire epochs. The registered bypass field represents the actual
wire, rather than a forecast through next-state registers. No observer field
or timing constraint is removed. Actual native consumer/capture timing and
original60ns qualification remain separate requirements.
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
        assert r['engine']==engine and r['status']=='PASS_R_FRONT_PHYSICAL_FORWARD'and not r['mutation']
        assert r['actions'][-1]['returncode']==0 and r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        assert r['physical_forwarding_before_and_after_commit_checked']and r['next_capture_forecast_still_checked']and r['original_reference_unchanged']
        for row in r['source_files']:
            p=d/row['path'];assert sha(p)==row['output_sha256']
            if row['changed']:
                if p.name in changed:assert sha(changed[p.name])==sha(p)
                changed[p.name]=p
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
        gates.append(dict(directory=str(d),engine=engine,summary=r['summary'],clocks=clocks,run_state_sha256=sha(d/'run-state.json')))
    assert gates[0]['clocks']==gates[1]['clocks']and len(changed)==4
    record=json.loads((parent/'provenance.json').read_text());assert record['component']=='r_front_control_current_memory'and record['clock_period_ns']==15
    for n,h in record['files'].items():assert sha(parent/n)==h,n
    assert(parent/'clock.sdc').read_text()==fixture.SDC
    out.mkdir(parents=True)
    for n in record['files']:(out/n).write_bytes((parent/n).read_bytes())
    for name,p in changed.items():
        if name=='tb_r_front_local.sv':continue
        target='dorado_r_front_control_step.sv'if name=='candidate.sv'else name
        assert(out/target).exists(),target
        (out/target).write_bytes(p.read_bytes())
    for n in [Path(__file__).name,'test_r_front_forward_view.py']:(out/n).write_bytes((ROOT/'tools'/n).read_bytes())
    result=dict(scope=__doc__,component='r_front_control_physical_forward',clock_period_ns=15,physical_Q_bits=844,ram_bits=9216,
                input_staging_bits=1650,launch_bits=1650,result_bits=1400,closed_latch_encoding_bits=80,functional_gates=gates,
                bypass_is_current_physical_observation=True,forecast_still_reference_checked=True,all_functional_observations_preserved=True,
                parent_provenance_sha256=sha(parent/'provenance.json'),quartus_genvar_syntax=record['quartus_genvar_syntax'],
                shell_bytes_unchanged=sha(out/'top.sv')==sha(parent/'top.sv'),tested_candidate_sha256=sha(out/'dorado_r_front_control_step.sv'),
                files={p.name:sha(p)for p in out.iterdir()if p.is_file()},assembly_permitted=False,native_qualified=False,
                native_consumer_timing_qualified=False,full_producer_arrival_contract_qualified=False,all_profiles_qualified=False)
    assert result['shell_bytes_unchanged']
    (out/'provenance.json').write_text(json.dumps(result,indent=2)+'\n');print('PREPARED_PHYSICAL_FORWARD_15NS_COMPONENT_BUDGET',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','icarus','verilator','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),[a.icarus.resolve(),a.verilator.resolve()],a.output.resolve())
