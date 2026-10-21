#!/usr/bin/env python3
"""Compare retained conditions in a bounded original-Initial/map experiment."""
import argparse
import gzip
import json
from pathlib import Path
import re

from prepare_r_front_condition_probe import MONITOR,sha


def inspect(control,candidate):
    rows=[];inputs={};inherited_outputs={}
    for directory in [control,candidate]:
        provenance=directory/'provenance.json.gz'
        r=json.loads(gzip.decompress(provenance.read_bytes()))
        state=json.loads((directory/'run-state.json').read_text())
        assert state['status']=='TERMINAL_RECEIVE_FILTER_PROBE'
        assert all(a['returncode']==0 for a in state['actions'])
        assert r['condition_probe_only']and r['probe_only']
        parent=Path(r['condition_probe_parent']);pp=parent/'provenance.json.gz'
        assert sha(pp)==r['condition_probe_parent_provenance_sha256']
        pr=json.loads(gzip.decompress(pp.read_bytes()))
        # V32's first preparer also copied the parent's private frame output.
        # Classify that single extra file explicitly; all declared source
        # inputs and every reconstructed testbench byte remain required.
        extras=set(r['inputs'])-set(pr['inputs'])
        assert not(set(pr['inputs'])-set(r['inputs']))
        assert extras<={'dorado_frame.pgm'}
        inherited_outputs[str(directory)]=sorted(extras)
        changed=[]
        for name,h in r['inputs'].items():
            p=directory/'executed-inputs'/name;assert sha(p)==h,name
            original=parent/'executed-inputs'/name
            if name in extras:
                assert sha(original)==h
                continue
            assert sha(original)==pr['inputs'][name]
            if h!=pr['inputs'][name]:
                changed.append(name);assert p.read_text().replace(MONITOR+'\n','')==original.read_text()
        assert changed==['verilog/verilator/tb_exec.sv']
        log=(directory/'test.log').read_text();parent_log=(parent/'test.log').read_text()
        assert 'INITIAL ROM PASS:'in log
        phases=lambda text:[line for line in text.splitlines()if line.startswith('BOOT_DIAG_PHASE ')]
        assert len(phases(log))==6000 and phases(log)==phases(parent_log)
        assert (directory/'register-map-inspection.txt').read_bytes()==(parent/'register-map-inspection.txt').read_bytes()
        rows.append([dict(re.findall(r'(\w+)=([^ ]+)',line))for line in log.splitlines()if line.startswith('BOOT_DIAG_CONDITION ')])
        assert len(rows[-1])==6000
        for name in ['provenance.json.gz','test.log','run-state.json','register-map-inspection.txt']:
            inputs[str(directory/name)]=sha(directory/name)
    assert [r['sample']for r in rows[0]]==[r['sample']for r in rows[1]]
    differences={k:sum(a[k]!=b[k]for a,b in zip(*rows))for k in rows[0][0]if k!='sample'}
    closed=[(a,b)for a,b in zip(*rows)if a['HFlagClock']==b['HFlagClock']=='1']
    first=next((dict(control=a,candidate=b)for a,b in zip(*rows)if a['Flags']!=b['Flags']),None)
    return dict(status='OBSERVED_RETAINED_CONDITION_COMPARISON',samples_per_run=6000,
        differences=differences,closed_clock_samples=len(closed),
        closed_clock_flag_differences=sum(a['Flags']!=b['Flags']for a,b in closed),
        first_flag_difference=first,parent_phase_rows_and_final_RM_map_unchanged=True,
        scope='Only bounded original Initial/map window; not Cedar loop, full CPU consumption or firmware qualification',
        inherited_private_outputs=inherited_outputs,hardware_cause_established=False,inputs=inputs)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['control','candidate','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=inspect(a.control.resolve(),a.candidate.resolve())
    a.output.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items()if k!='inputs'},indent=2))
