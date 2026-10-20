#!/usr/bin/env python3
"""Actual-board negative tests for independent global hold/scan/auxiliary gates."""
import argparse
import json
import os
from pathlib import Path
import resource
import shutil
import subprocess
import sys
import generate_serial_boards as gen


def run(positive,out):
    out.mkdir(parents=True,exist_ok=False)
    verified=json.loads((positive/'results.json').read_text());assert verified['status']=='PASS'
    cases=[('lost_global_hold','state_global_hold || (',"1'b0 || (",
            ('ordinary outputs differ','physical state differs','RAM probe differs','forced hold did not freeze')),
           ('lost_scan_abort','.freeze(state_freeze && !state_global_abort)', '.freeze(state_freeze)',
            ('global abort did not quarantine scan',)),
           ('lost_aux_abort',' && !state_global_abort;',';',('global abort admitted auxiliary access',))]
    results=[]
    for name in ('ContA','MemX','BaseBd'):
        original=positive/name;source=original/'inputs'
        checked=next(r for r in verified['boards'] if r['board']==name)
        assert all(gen.sha((source/p).read_bytes())==h for p,h in checked['source_hashes'].items()),'positive source changed'
        command=json.loads((original/'command.json').read_text())
        for label,before,after,why in cases:
            case=out/(name+'-'+label);case.mkdir();inputs=case/'inputs';shutil.copytree(source,inputs)
            board=inputs/(name+'__board.sv');raw=board.read_bytes();assert raw.count(before.encode())==1
            board.write_bytes(raw.replace(before.encode(),after.encode(),1))
            cmd=[s.replace(str(source),str(inputs)).replace(str(original/'obj'),str(case/'obj')) for s in command]
            (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
            with(case/'build.log').open('w')as log:subprocess.run(cmd,cwd=inputs,stdout=log,stderr=subprocess.STDOUT,check=True)
            result=subprocess.run([str(case/'obj/Vglobal_fixture')],cwd=inputs,capture_output=True,text=True,timeout=180)
            text=result.stdout+result.stderr;(case/'test.txt').write_text(text)
            print(name+'/'+label+': '+text.strip(),flush=True)
            assert result.returncode and any(reason in text for reason in why),(name,label,text)
            results.append(dict(board=name,case=label,returncode=result.returncode,expected_failure=why,
                mutated_source_sha256=gen.sha(board.read_bytes())))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results),indent=2)+'\n')


if __name__=='__main__':
    if resource.getrlimit(resource.RLIMIT_STACK)[0]<32*1024*1024:
        os.execv('/bin/zsh',['/bin/zsh','-c','ulimit -s 32768 || exit; exec "$@"','global-board-negative',sys.executable,str(Path(__file__).resolve()),*sys.argv[1:]])
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('positive',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.positive.resolve(),a.output.resolve())
