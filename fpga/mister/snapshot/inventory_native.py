#!/usr/bin/env python3
"""Inventory the exact frozen native-machine hierarchy, without host ABI offsets.

Copy a frozen Quartus project's src tree and actual core QSF first. This
elaborates dorado_native_machine using its generated sources.f and real macros.
It intentionally does NOT claim coverage of emu/MiSTer framework, transport/
input/video state outside that hierarchy, external RAM bytes or ARM services.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import sys

ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources',type=Path,required=True)
    parser.add_argument('--qsf',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    source=args.sources.resolve();qsf=args.qsf.resolve();out=args.output.resolve()
    generated=source/'build/rtl-executive/native16-tx-cable'
    manifest=json.loads((generated/'manifest.json').read_text())
    if manifest.get('storage_backend')!='sdram' or not manifest.get('storage_interleave') or manifest.get('storage_defer_reads',False):
        raise ValueError('Expected selected Native49 SDRAM/interleaved/non-deferred baseline')
    definitions=[]
    for line in qsf.read_text().splitlines():
        words=shlex.split(line,comments=True)
        if words[:3]==['set_global_assignment','-name','VERILOG_MACRO']:
            if len(words)!=4:
                raise ValueError('unreviewed macro assignment: '+line)
            definitions.append(words[3])
    definitions=sorted(set(definitions))
    for name in ('DORADO_STORAGE_SDRAM','DORADO_F10415_M10K','DORADO_F10145_LOGIC','DORADO_ETHERNET_DDR','DORADO_HOST_CLOCK_HZ=50000000'):
        if name not in definitions:
            raise ValueError('missing baseline macro: '+name)
    paths=[source/name for name in (generated/'sources.f').read_text().splitlines()if name.strip()]
    # Later frozen-core builds add backend/device modules to QSF after the
    # generator emits sources.f. Include the frozen library; inventory only
    # traverses instantiated cells under the stated top hierarchy.
    paths += sorted((source/'fpga/mister').glob('*.sv'))
    paths=list(dict.fromkeys(p.resolve()for p in paths if p.name!='Dorado.sv'))
    if not all(p.is_file()for p in paths):
        raise ValueError('missing frozen source')
    out.mkdir(parents=True,exist_ok=False)
    command=['verilator','--xml-only','--timing','--top-module','dorado_native_machine',
             '-Wno-fatal',*[f'-I{source/part}'for part in ('verilog/cells','verilog/generated','verilog/vendor/6502','verilog/vendor/riot')],
             *['+define+'+d for d in definitions],'--xml-output',str(out/'native.xml'),*map(str,paths)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:
        subprocess.run(command,cwd=source,stdout=log,stderr=subprocess.STDOUT,check=True)
    result=inventory((out/'native.xml').read_bytes())
    result['scope']='Exactly frozen Native49 dorado_native_machine hierarchy/macros, not emu/wrapper/external RAM/ARM services; candidates remain UNCLASSIFIED'
    (out/'inventory.json').write_text(json.dumps(result,indent=2)+'\n')
    metadata=dict(scope=result['scope'],macros=definitions,native_manifest=manifest,
                  source_hashes={str(p.relative_to(source)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},
                  qsf_sha256=hashlib.sha256(qsf.read_bytes()).hexdigest(),
                  tool_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
                               for p in (Path(__file__),ROOT/'tools/sil_state_inventory.py')})
    (out/'provenance.json').write_text(json.dumps(metadata,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='candidates'},indent=2))
