#!/usr/bin/env python3
"""Re-run the serial disk/HPS payload gate for original and staged cache tags.

The period file-backed serial medium is the independent oracle. Loop back a
sector, seek, and require byte-identical whole-image writeback. Both builds use
the same falling-before-rising host phases and deterministic extra guest stalls.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_disk_cache_phase as gen
from generate_serial_boards import ROOT,once


def run(out,image):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    for name in ('tb_hps_pack.sv','hps_pack_test.cpp','hps_disk_test.h','dorado_pack.sv'):
        (inputs/name).write_bytes((ROOT/'verilog/verilator'/name).read_bytes())
    cpp=(inputs/'hps_pack_test.cpp').read_text()
    cpp=once(cpp,'i<1000000','i<3000000')
    cpp=once(cpp,'bool run=t.pack_ready;','bool run=t.pack_ready && (i%7!=0);')
    (inputs/'hps_pack_test.cpp').write_text(cpp)
    source=gen.reference();candidate=gen.transform(source);assert gen.inverse(candidate)==source
    results=[]
    for label,body in [('reference',source),('candidate',candidate)]:
        case=out/label;case.mkdir();(case/'medium.sv').write_text(body)
        command=['verilator','--cc','--exe','--build','--timing','--assert','-j','4','--top-module','tb_hps_pack','-Wno-fatal',
                 '--Mdir',str(case/'obj'),'-CFLAGS','-O2 -std=c++20 -I'+str(inputs),'-o','test-payload',str(inputs/'tb_hps_pack.sv'),
                 str(case/'medium.sv'),str(inputs/'dorado_pack.sv'),str(inputs/'hps_pack_test.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/test-payload'),str(image),'+pack='+str(image)],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(label+': '+text.strip(),flush=True)
        assert not result.returncode and 'HPS-PACK PASS'in text,text
        results.append(dict(case=label,returncode=result.returncode,output=text))
    assert results[0]['output']==results[1]['output'],'reference/candidate disk result differs'
    for p in (Path(__file__),Path(gen.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,cases=results,image=dict(path=str(image),bytes=image.stat().st_size,sha256=hashlib.sha256(image.read_bytes()).hexdigest()),
        executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},
        limits='Component payload regression only; no native integration, wrapper snapshot access or timing qualification.'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--image',type=Path,default=ROOT/'chm/diskpacks/games-trident.pack');a=p.parse_args();run(a.output.resolve(),a.image.resolve())
