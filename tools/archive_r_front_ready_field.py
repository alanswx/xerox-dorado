#!/usr/bin/env python3
"""Archive the failed Ready/front hardware boots and component diagnostics.

Lossless evidence only. Running test logs are bounded prefixes; their capture
does not imply termination or qualification. Excludes FPGA images, executables
and Quartus databases. Original firmware inputs remain part of test provenance.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
RUNS=[
 'r-front-ready-ether-handoff-v1-20261021',
 'r-front-ready-ether-handoff-control-v1-20261021',
 'r-front-ready-pilot-v1-20261021',
 'r-front-ready-pilot-control-v1-20261021',
 'r-front-ready-pilot-control-v2-20261021',
 'r-front-ready-pilot-v3-20261021',
 'r-front-ready-pilot-control-v3-20261021',
]


def sha(raw):return hashlib.sha256(raw).hexdigest()


def archive(out):
    out.mkdir(parents=True,exist_ok=False)
    files={}
    field=ROOT/'build/rtl-executive/r-front-ready-native-field-20261021'
    for path in field.rglob('*'):
        if path.is_file():files['field/'+str(path.relative_to(field))]=path
    processes=[]
    for name in RUNS:
        directory=ROOT/'build/rtl-executive'/name
        for path in directory.iterdir():
            if path.is_file() and path.suffix in ('.json','.gz','.log','.txt'):
                files['simulation/'+name+'/'+path.name]=path
        # Preserve all frozen inputs for the corrected paired test. Earlier
        # diagnostics retain their changed bench/script plus parent hashes.
        frozen=directory/'executed-inputs'
        if frozen.exists():
            paths=frozen.rglob('*') if '-v3-' in name else [
                frozen/'verilog/verilator/tb_exec.sv',frozen/'test_r_front_ready_boot.py']
            for path in paths:
                if path.is_file():
                    files['simulation/'+name+'/executed-inputs/'+str(path.relative_to(frozen))]=path
        state_path=directory/('run-state.json' if (directory/'run-state.json').exists() else 'execution.json')
        state=json.loads(state_path.read_text())
        pids=[a['pid'] for a in state.get('actions',[])] or [state['pid']]
        for pid in pids:
            result=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,command='],capture_output=True,text=True)
            processes.append(dict(run=name,pid=pid,ps_returncode=result.returncode,
                                  authoritative_observation=result.stdout.strip()))
    for name in ['deploy_r_front_ready_native.py','deploy_mc1668_native.py',
                 'collect_r_front_ready_field.py','test_r_front_ready_boot.py',
                 Path(__file__).name]:
        files['tools/'+name]=ROOT/'tools'/name
    rows=[]
    for name,path in sorted(files.items()):
        assert path.suffix not in ('.rbf','.sof','.qdb','.o','.a'),path
        with path.open('rb') as stream:
            data=stream.read(path.stat().st_size)
        if path.suffix=='.log' and data and not data.endswith(b'\n'):
            data=data[:data.rfind(b'\n')+1]
        assert not data.startswith(b'\x7fELF'),path
        packed=not(name.endswith(('.gz','.png')) or (name.endswith('.json') and len(data)<=102400))
        stored=name+'.gz' if packed else name
        target=out/'raw'/stored;target.parent.mkdir(parents=True,exist_ok=True)
        target.write_bytes(gzip.compress(data,mtime=0) if packed else data)
        readback=gzip.decompress(target.read_bytes()) if packed else target.read_bytes()
        assert readback==data
        rows.append(dict(raw_name=name,stored=stored,raw_bytes=len(data),raw_sha256=sha(data),
                         stored_sha256=sha(target.read_bytes()),gzip=packed))
    record=dict(scope=__doc__,captured=time.time(),members=rows,processes=processes,
                status='COLLECTED_STARTUP_FAILURE_DIAGNOSTICS',desktop_qualified=False,
                speedup_qualified=False,whole_machine_snapshot=False)
    (out/'archive-manifest.json.gz').write_bytes(gzip.compress(
        (json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    for row in rows:
        raw=(out/'raw'/row['stored']).read_bytes()
        assert sha(raw)==row['stored_sha256']
        if row['gzip']:raw=gzip.decompress(raw)
        assert len(raw)==row['raw_bytes'] and sha(raw)==row['raw_sha256']
    print(json.dumps(dict(status=record['status'],members=len(rows),
                         raw_bytes=sum(r['raw_bytes'] for r in rows),
                         lossless_readback=True)))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True)
    archive(p.parse_args().output.resolve())
