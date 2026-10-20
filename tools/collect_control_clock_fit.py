#!/usr/bin/env python3
"""Collect a terminal isolated clock fit after exact input/process readback.

Run on the Linux build host. Database directories and FPGA images are excluded;
this collects implementation-budget evidence, never native speed qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import tarfile


def collect(project, output):
    assert project not in output.parents, 'Keep the output archive outside the project'
    state=json.loads((project/'run-state.json').read_text())
    assert state['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [r['stage']for r in state['stages']]==['map','fit','sta','corners']
    assert all(r['returncode']==0 for r in state['stages'])
    pids=[state['builder_pid']]+[r['pid']for r in state['stages']]
    assert len(set(pids))==5
    assert not [p for p in pids if Path('/proc/%d/exe'%p).exists()], 'Observe same build until all processes exit'
    provenance=json.loads((project/'provenance.json').read_text())
    annotation=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    actual={};changed=[]
    for name,digest in provenance['files'].items():
        raw=(project/name).read_bytes()
        actual[name]=hashlib.sha256(raw).hexdigest()
        if actual[name]!=digest:
            assert name=='clock_fit.qsf' and raw.endswith(annotation) and raw.count(annotation)==1,(name,'changed input')
            assert hashlib.sha256(raw[:-len(annotation)]).hexdigest()==digest
            changed.append(name)
    proof=dict(status='TERMINAL_INPUT_READBACK',job_pids_absent=pids,changed_inputs=changed,actual=actual)
    path=project/'collected-input-check.json'
    assert not path.exists() and not output.exists(), 'Inspect existing collection; do not overwrite'
    path.write_text(json.dumps(proof,indent=2)+'\n')
    with tarfile.open(output,'w:gz') as archive:
        for p in sorted(project.iterdir()):
            if not p.is_file():continue
            assert p.suffix not in ('.rbf','.sof'),'Assembly was forbidden for this fixture'
            archive.add(p,arcname=p.name)
    print(json.dumps(proof),flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('project',type=Path);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();collect(args.project.resolve(),args.output.resolve())
