#!/usr/bin/env python3
"""Recompile complete phase qualifications using only archive-extracted HDL.

No working RTL or generated build objects are consumed. Original command
input paths are relocated to extracted bytes; compiler output paths are fresh.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import archive_receipts


def execute(command,log,cwd):
    with log.open('w')as f:subprocess.run(command,cwd=cwd,stdout=f,stderr=subprocess.STDOUT,check=True)


def replay(receipt,out):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False)
    readback=archive_receipts.readback(receipt);root=out/'extracted';root.mkdir()
    manifest=json.loads(gzip.decompress((receipt/'archive-manifest.json.gz').read_bytes()))
    for row in manifest:
        name=Path(row['raw_name']);assert not name.is_absolute()and '..'not in name.parts
        raw=(receipt/row['stored']).read_bytes()
        if row['stored']!=row['raw_name']:raw=gzip.decompress(raw)
        assert hashlib.sha256(raw).hexdigest()==row['raw_sha256']
        p=root/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    results=[]
    for label in ('boards-memc','boards-rest','boards-baseboard'):
        for case in sorted((root/label).iterdir()):
            if not case.is_dir()or not(case/'test.txt').is_file():continue
            dest=out/'rebuilt-boards'/case.name;dest.mkdir(parents=True)
            old=json.loads((case/'command.json').read_text())
            original_inputs=str(Path(next(v for v in old if v.endswith('/fixture.sv'))).parent)
            cmd=[v.replace(original_inputs,str(case/'inputs'))for v in old]
            cmd[cmd.index('--Mdir')+1]=str(dest/'obj')
            (dest/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
            execute(cmd,dest/'build.log',case/'inputs')
            result=subprocess.run([str(dest/'obj/Vglobal_fixture')],cwd=case/'inputs',capture_output=True,text=True,timeout=240)
            text=result.stdout+result.stderr;(dest/'test.txt').write_text(text)
            assert result.returncode==0 and text==(case/'test.txt').read_text(),(case.name,text)
            print(case.name+': '+text.strip(),flush=True);results.append(dict(board=case.name,status='PASS'))
    assert len(results)==11
    domain=root/'domain';tested=json.loads((domain/'results.json').read_text())
    for record in tested['cases']:
        label=record['case'];case=domain/('candidate'if label=='fresh_random'else label)
        dest=out/'rebuilt-domain'/label;dest.mkdir(parents=True)
        old=json.loads((case/'commands.json').read_text())[0]
        original_root=str(Path(old[old.index('-o')+1]).parent.parent)
        cmd=[v.replace(original_root,str(domain))for v in old];binary=dest/'test.vvp';cmd[cmd.index('-o')+1]=str(binary)
        execute(cmd,dest/'build.log',domain)
        run=['vvp',str(binary)]+(['+random']if label=='fresh_random'else[])
        result=subprocess.run(run,cwd=domain,capture_output=True,text=True,timeout=120)
        text=result.stdout+result.stderr;(dest/'test.txt').write_text(text)
        if record.get('expected_failure'):assert result.returncode and record['expected_failure']in text
        else:assert result.returncode==0 and'NATIVE PHASE DOMAIN PASS'in text
        print('domain '+label+': PASS',flush=True);results.append(dict(domain=label,status='PASS'))
    msa=root/'eight-msa';tested=json.loads((msa/'results.json').read_text())
    for record in tested['cases']:
        label=record['case'];case=msa/label;dest=out/'rebuilt-eight-msa'/label;dest.mkdir(parents=True)
        old=json.loads((case/'commands.json').read_text())[0]
        original_root=str(Path(old[old.index('--Mdir')+1]).parent.parent)
        cmd=[v.replace(original_root,str(msa))for v in old];cmd[cmd.index('--Mdir')+1]=str(dest/'obj')
        execute(cmd,dest/'build.log',msa)
        result=subprocess.run([str(dest/'obj/test')],cwd=msa,capture_output=True,text=True,timeout=300)
        text=result.stdout+result.stderr;(dest/'test.txt').write_text(text)
        if record.get('expected_failure'):assert result.returncode and record['expected_failure']in text
        else:
            assert result.returncode==0 and all(s in text for s in('EIGHT MSA QUARANTINE PASS',
                'host=4692793 guest=4405 reads=41760 writes=7472','checks=52931566'))
        print('eight-msa '+label+': PASS',flush=True);results.append(dict(msa=label,status='PASS'))
    # Prove the raw files used for the rebuild remained exact after every run.
    for row in manifest:
        assert hashlib.sha256((root/row['raw_name']).read_bytes()).hexdigest()==row['raw_sha256']
    (out/'results.json').write_text(json.dumps(dict(status='PASS_EXTRACTED_PHASE_REPLAY',receipt=readback,cases=results,
        compiler_inputs='Only archive-extracted HDL/headers/memory files; fresh compiler objects.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--receipt',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();replay(a.receipt.resolve(),a.output.resolve())
