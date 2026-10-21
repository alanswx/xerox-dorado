#!/usr/bin/env python3
"""Bind corrected line observers to the already archived V16 frozen inputs."""
import gzip
import hashlib
import json
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[5]
OUT=Path(__file__).resolve().parent


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


if __name__=='__main__':
    results=[]
    for arm in['control','candidate']:
        old=ROOT/'build/rtl-executive'/('r-front-ready-memory-line-'+arm+'-v16-20261021')
        new=ROOT/'build/rtl-executive'/('r-front-ready-memory-line-'+arm+'-v17-20261021')
        before=json.loads(gzip.decompress((old/'provenance.json.gz').read_bytes()))
        after=json.loads(gzip.decompress((new/'provenance.json.gz').read_bytes()))
        assert before['inputs'].keys()==after['inputs'].keys()
        changed=[n for n in after['inputs']if before['inputs'][n]!=after['inputs'][n]]
        assert set(changed)=={'test_r_front_ready_boot.py','verilog/verilator/tb_exec.sv'},changed
        assert before['external_inputs']==after['external_inputs']
        for name,digest in after['inputs'].items():assert sha(new/'executed-inputs'/name)==digest,name
        state=json.loads((new/'run-state.json').read_text())
        assert state['status']=='RUNNING'and state['actions'][0]['returncode']==0
        pid=state['actions'][-1]['pid']
        observed=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,command='],capture_output=True,text=True)
        assert observed.returncode==0 and str(new/'obj/test')in observed.stdout
        directory=OUT/arm;directory.mkdir(exist_ok=False)
        for name in changed:
            target=directory/(name+'.gz');target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress((new/'executed-inputs'/name).read_bytes(),mtime=0))
            assert hashlib.sha256(gzip.decompress(target.read_bytes())).hexdigest()==after['inputs'][name]
        for name in['provenance.json.gz','run-state.json','compile.log']:
            data=(new/name).read_bytes();target=directory/(name+'.gz')
            target.write_bytes(gzip.compress(data,mtime=0));assert gzip.decompress(target.read_bytes())==data
        results.append(dict(arm=arm,old_run=old.name,new_run=new.name,changed_inputs=changed,
            old_provenance_sha256=sha(old/'provenance.json.gz'),new_provenance_sha256=sha(new/'provenance.json.gz'),
            pid=pid,verified_live_command=observed.stdout.strip(),status='RUNNING_PREFIX_NOT_A_RESULT',
            unchanged_inputs=len(after['inputs'])-len(changed),lossless_readback=True))
    with(OUT/'corrected-inputs.json').open('x')as stream:json.dump(results,stream,indent=2);stream.write('\n')
    print(json.dumps([dict(arm=r['arm'],pid=r['pid'],unchanged_inputs=r['unchanged_inputs'])for r in results]))
