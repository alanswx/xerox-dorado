#!/usr/bin/env python3
"""Recover every affected original corner/crossing report on the same fit.

Copy the five original read-only Tcl reporters, changing only their corner
loop variable name so the original SDC cannot overwrite it. Preserve all
queries, path counts, operating conditions and original malformed reports.
Do not change a compiled input, timing constraint, seed or fitted database.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

REPORTS = ['device-corners.tcl', 'audit-control-crossings.tcl', 'audit-contb-crossings.tcl',
           'audit-adapter-crossings.tcl', 'audit-storage-ready.tcl']


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(project, out, quartus):
    assert not out.exists()
    manifest = project/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    assert not m['assembly_permitted']
    for name, digest in m['source_hashes'].items():
        assert sha(project/name) == digest
    for name in ['run-state.json', 'physical-report/run-state.json', 'cp-key-report/run-state.json', 'corrected-corners-report/run-state.json']:
        s = json.loads((project/name).read_text())
        assert s['status'].startswith('TERMINAL')
        for pid in [s.get('builder_pid', s.get('pid'))]+[a['pid'] for a in s.get('stages', s.get('actions', []))]:
            assert not (Path('/proc')/str(pid)).exists()
    out.mkdir(parents=True)
    bindings = []
    for name in REPORTS:
        raw = (project/'core'/name).read_text()
        assert raw.count('foreach name $corners {') == 1
        changed = raw.replace('foreach name $corners {', 'foreach dorado_report_corner $corners {', 1)
        changed = re.sub(r'\$name\b', '$dorado_report_corner', changed)
        restored = changed.replace('foreach dorado_report_corner $corners {', 'foreach name $corners {', 1).replace('$dorado_report_corner', '$name')
        assert restored == raw
        (out/name).write_text(changed)
        bindings.append(dict(name=name, original_sha256=sha(project/'core'/name), changed_sha256=sha(out/name), inverse_byte_identical=True))
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING_CORRECTED_ORIGINAL_CROSSINGS', pid=os.getpid(), started=time.time(),
                 manifest_sha256=sha(manifest), assembly_permitted=False, constraints_changed=False,
                 compiled_inputs_unchanged=True, no_refit=True, bindings=bindings, actions=[])
    path = out/'run-state.json'

    def save():
        temp = path.with_suffix('.tmp')
        temp.write_text(json.dumps(state, indent=2)+'\n')
        os.replace(temp, path)

    save()
    try:
        for name in REPORTS:
            command = [str(quartus/'quartus_sta'), '-t', str(out/name)]
            with (out/(name+'.log')).open('x') as log:
                process = subprocess.Popen(command, cwd=project/'core', stdout=log, stderr=subprocess.STDOUT)
                row = dict(name=name, pid=process.pid, command=command, started=time.time())
                state['actions'].append(row)
                save()
                code = process.wait()
                row.update(returncode=code, finished=time.time())
                save()
                assert code == 0
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest
        state.update(status='TERMINAL_CORRECTED_ORIGINAL_CROSSINGS_REVIEW_REQUIRED', finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise
    print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['project', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    run(a.project.resolve(), a.output.resolve(), a.quartus.resolve())
