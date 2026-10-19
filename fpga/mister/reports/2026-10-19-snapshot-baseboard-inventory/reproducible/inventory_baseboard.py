#!/usr/bin/env python3
"""Inventory the actual native BaseBoard, including netlist 6502 and five RIOTs.

Review evidence only: not a state classifier, adapter or machine snapshot.
The output preserves lexical scopes, including RIOT's block-local old_pa7.
"""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET

ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'tools'))
from sil_state_inventory import inventory,constant


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run(source,out):
    board=source/'build/rtl-executive/native16-tx-cable/BaseBd_nominal.v'
    backplane=source/'build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
    assert backplane.read_text().count('BaseBd_m_Rev_m_Am_nominal #(.SYSPER(SYSPER)) b_BaseBd (')==1
    paths=[board,*sorted((source/'verilog/cells').glob('*.v')),
           source/'verilog/vendor/6502/chip_6502.v',source/'verilog/vendor/riot/M6532.sv']
    out.mkdir(parents=True,exist_ok=False)
    cmd=['verilator','--xml-only','--top-module','BaseBd_m_Rev_m_Am_nominal',
         '-GSYSPER=4','-Wno-fatal','-DDORADO_F10145_LOGIC','-DDORADO_F10415_M10K',
         '--xml-output',str(out/'baseboard.xml'),*map(str,paths)]
    (out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as f:
        subprocess.run(cmd,cwd=source,stdout=f,stderr=subprocess.STDOUT,check=True)
    raw=(out/'baseboard.xml').read_bytes();report=inventory(raw)
    seq=defaultdict(lambda:dict(entries=0,bits=0,fields={},lexical_scopes=set()))
    for row in report['candidates']:
        if row['category']!='sequential':continue
        q=seq[row['module']];q['entries']+=1;q['bits']+=row['bits']
        q['fields'][row['name']]=dict(bits=row['bits'],width=row['width'],dimensions=row['unpacked_dimensions'])
        q['lexical_scopes'].add(row['path'].rsplit('.',1)[0])
    for q in seq.values():q['lexical_scopes']=sorted(q['lexical_scopes'])
    xml=ET.fromstring(raw);bindings=[]
    for mod in xml.findall('.//module'):
        if mod.get('origName')not in ('cell_CLOCKGEN','BaseBd_m_Rev_m_Am_nominal','cell_MCS6532','M6532'):continue
        params={v.get('name'):constant(v.find('const').get('name'))for v in mod.findall('var')
                if(v.get('param')=='true'or v.get('localparam')=='true')and v.find('const')is not None}
        bindings.append(dict(module=mod.get('name'),parameters=params))
    # Instance paths, not field lexical scopes, count the actual five RIOTs.
    riots={r['path'].split('.u_riot.',1)[0]+'.u_riot'for r in report['candidates']if r['module']=='M6532'}
    assert len(riots)==5 and seq['M6532']['bits']==5485
    assert seq['chip_6502']['bits']==1725 and set(seq['chip_6502']['fields'])=={'q'}
    assert sum(q['bits']for q in seq.values())==40366
    assert next(r['parameters']for r in bindings if r['module']=='BaseBd_m_Rev_m_Am_nominal')==dict(SYSPER=4)
    assert next(r['parameters']for r in bindings if r['module'].startswith('cell_CLOCKGEN'))==dict(N=4,W=2,HALF=2,ECP_LO=0,SCP_LO=1)
    text=board.read_text()
    sockets=re.findall(r'cell_i2716(?:\s*#\(\.INIT_FILE\("([^"]+)"\)\))?\s+(u_\w+)\s*\(',text)
    assert len(sockets)==8 and len([f for f,_ in sockets if f])==4
    eproms=[dict(package=pkg,init_file=f or None,sha256=digest(source/f)if f else None)for f,pkg in sockets]
    meta=dict(status='REVIEW ONLY',scope=__doc__,sequential=seq,bindings=bindings,
        sequential_bits_before_classification=40366,riot_instances=sorted(riots),eprom_sockets=eproms,
        remaining_review=['Classify every candidate including initial-only memories and diagnostics.',
            'Eight EPROM sockets: four have explicit image assets; four have no INIT_FILE. Preserve registered read outputs; explicitly review empty/uninitialized socket semantics.',
            'Implement and qualify exact state access for the 1725-node CPU, RIOT RAM/timers/edge histories and other actual primitives.',
            'No global wrapper/backend/ARM/input/disk-generation state or coherent save/load included.'])
    provenance=dict(source_hashes={str(p.relative_to(source)):digest(p)for p in paths+[backplane]+[source/f for f,_ in sockets if f]},
        tool_sha256=digest(ROOT/'tools/sil_state_inventory.py'),driver_sha256=digest(Path(__file__)))
    for name,value in [('inventory.json',report),('sequential-review.json',meta),('provenance.json',provenance)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    (out/'inventory_baseboard.py').write_bytes(Path(__file__).read_bytes())
    print('BaseBoard inventory REVIEW ONLY:40366sequential bits before classification,1725CPU nodes,5RIOTs,32RAMchips,8EPROM sockets/4explicit images')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--frozen-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.frozen_root.resolve(),a.output.resolve())
