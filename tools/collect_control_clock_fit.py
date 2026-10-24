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


def collect(project, output, allow_failed=False):
    assert project not in output.parents, 'Keep the output archive outside the project'
    state=json.loads((project/'run-state.json').read_text())
    expected=['map','fit','sta','corners']
    stages=state['stages']
    if allow_failed:
        assert state['status']=='TERMINAL_FAILED'
        assert [r['stage']for r in stages]==expected[:len(stages)]
        assert all(r['returncode']==0 for r in stages[:-1]) and stages[-1]['returncode']!=0
    else:
        assert state['status']=='TERMINAL_REVIEW_REQUIRED'
        assert [r['stage']for r in stages]==expected
        assert all(r['returncode']==0 for r in stages)
    pids=[state['builder_pid']]+[r['pid']for r in state['stages']]
    assert len(set(pids))==len(stages)+1
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
    proof=dict(status='TERMINAL_FAILED_INPUT_READBACK' if allow_failed else 'TERMINAL_INPUT_READBACK',
               build_status=state['status'],job_pids_absent=pids,changed_inputs=changed,actual=actual)
    path=project/'collected-input-check.json'
    assert not path.exists() and not output.exists(), 'Inspect existing collection; do not overwrite'
    path.write_text(json.dumps(proof,indent=2)+'\n')
    with tarfile.open(output,'w:gz') as archive:
        # Multi-package fixtures freeze nested sources/ROMs as well. Include
        # every declared input, plus top-level reports, never DB directories.
        names=set(provenance['files'])
        names.update(p.name for p in project.iterdir() if p.is_file())
        for name in sorted(names):
            assert not Path(name).is_absolute() and '..' not in Path(name).parts
            p=project/name
            assert p.suffix not in ('.rbf','.sof'),'Assembly was forbidden for this fixture'
            assert p.suffix not in ('.o','.a','.pack') and not p.read_bytes().startswith(b'\x7fELF')
            archive.add(p,arcname=name)
    print(json.dumps(proof),flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('project',type=Path);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--allow-failed',action='store_true',help='Preserve a terminal failed build without accepting qualification')
    args=parser.parse_args();collect(args.project.resolve(),args.output.resolve(),args.allow_failed)
