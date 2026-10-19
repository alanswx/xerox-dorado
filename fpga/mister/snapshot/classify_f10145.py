#!/usr/bin/env python3
"""Account for original physical file state and excluded simulation diagnostics."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys

import generate_f10145 as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest):
    expected={'mem':dict(bits=64,width=4,dimensions=[[0,15]],access=manifest['ram'])}
    occupied=set((word,bit)for word in range(16)for bit in range(4))
    for field in manifest['write_window']['fields']:
        name=field['name']
        if name in expected:raise ValueError('duplicate physical field')
        positions={(manifest['write_window']['index'],bit)for bit in range(field['lsb'],field['lsb']+field['width'])}
        if positions&occupied:raise ValueError('aliased physical state bits')
        occupied|=positions
        expected[name]=dict(bits=field['width'],width=field['width'],dimensions=[],
                            access=dict(word=manifest['write_window']['index'],**field))
    if occupied!={(word,bit)for word in range(16)for bit in range(4)}|{(16,bit)for bit in range(10)}:
        raise ValueError('incomplete physical state layout')
    if manifest['ram']!=dict(words=16,width=4,indices=[0,15]):raise ValueError('changed RAM access geometry')
    packed=[(f['name'],f['width'],f['lsb'])for f in manifest['write_window']['fields']]
    if packed!=[('we_d',1,0),('a_lat',4,1),('d_lat',4,5),('ce_lat',1,9)]or manifest['write_window']['width']!=10:
        raise ValueError('write-window layout differs from generated packing')
    if manifest['logical_state_bits']!=74:raise ValueError('incorrect manifest state total')
    diagnostics={'a_first':4,'d_first':4,'in_we':1,'n_amoved':32,'n_dmoved':32,'n_writes':32,'ramwe_dbg':1}
    if set(manifest['excluded_simulation_only'])!=set(diagnostics):raise ValueError('changed diagnostic exclusions')
    decisions=[];seen=set()
    for row in report['candidates']:
        name=row['name']
        if row['path']!='cell_F10145A.'+name or name in seen:raise ValueError('duplicate/unexpected instance')
        seen.add(name)
        if name in expected:
            item=expected.pop(name)
            if row['category']!='sequential' or (row['width'],row['bits'],row['unpacked_dimensions'])!=(item['width'],item['bits'],item['dimensions']):
                raise ValueError('physical state geometry differs: '+name)
            decisions.append(dict(path=row['path'],decision='SAVE',bits=row['bits'],access=item['access']))
        elif name in diagnostics:
            width=diagnostics.pop(name)
            category='initial_only_candidate'if name=='ramwe_dbg'else'sequential'
            if (row['category'],row['bits'],row['unpacked_dimensions'])!=(category,width,[]):raise ValueError('diagnostic geometry differs')
            decisions.append(dict(path=row['path'],decision='EXCLUDE_SIMULATION_ONLY',bits=width,
                                  reason='Inside original synthesis translate_off block; generator verifies no reference outside it'))
        elif (name=='_unused_pins'and row['category']=='initial_only_candidate')or(name in ('a','q','p1','p14','p15','p2')and row['category']=='combinational_review'):
            decisions.append(dict(path=row['path'],decision='RECOMPUTE',bits=row['bits'],reason='Original continuous combinational/constant wire'))
        else:raise ValueError('unclassified state: '+name)
    if expected or diagnostics:raise ValueError('missing state/exclusion from original inventory')
    saved=sum(x['bits']for x in decisions if x['decision']=='SAVE')
    if saved!=74:raise ValueError('incorrect physical state total')
    return dict(status='PASS',physical_bits=saved,ram_bits=64,write_window_bits=10,
        excluded_simulator_bits=106,xml_sha256=report['xml_sha256'],decisions=decisions,
        scope='One original F10145 file, exact current RTL/macros; not board/machine completeness or physical restore')


def controls(report,manifest):
    rows=[]
    for case in ('missing_we','width','ram','new_state','alias','swapped_window','diagnostic_use'):
        r,m=copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_we':m['write_window']['fields']=m['write_window']['fields'][1:]
        elif case=='width':next(x for x in r['candidates']if x['name']=='ce_lat')['bits']=2
        elif case=='ram':next(x for x in r['candidates']if x['name']=='mem')['unpacked_dimensions']=[[0,31]]
        elif case=='new_state':
            r['candidates'].append(dict(r['candidates'][-1],name='new_history',path='cell_F10145A.new_history'))
        elif case=='alias':m['write_window']['index']=15
        elif case=='swapped_window':
            m['write_window']['fields'][0]['lsb']=9;m['write_window']['fields'][3]['lsb']=0
        else:m['excluded_simulation_only'].append('we_d')
        try:classify(r,m)
        except ValueError as error:rows.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise ValueError('damaged physical state coverage accepted: '+case)
    original=gen.ORIGINAL.read_text()
    for case,before,after in [
        ('source_extra_history','  reg we_d;','  reg we_d; reg extra_history;'),
        ('source_extra_process','endmodule',"always @(posedge sys_clk) we_d <= 1'b0;\nendmodule"),
        ('source_ram_geometry','reg [3:0] mem [0:15];','reg [3:0] mem [0:31];'),
        ('source_diagnostic_escape','wire _unused_pins =', 'wire escaped_debug = ramwe_dbg;\n  wire _unused_pins =')]:
        changed=original.replace(before,after)
        if changed==original:raise ValueError('missing source control anchor')
        try:gen.generate(changed)
        except ValueError as error:rows.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise ValueError('changed source silently accepted: '+case)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--negative-controls',action='store_true')
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    _,manifest=gen.generate()
    command=['verilator','--xml-only','--top-module','cell_F10145A','-DDORADO_F10145_LOGIC',
             '-Wno-fatal','--xml-output',str(out/'cell.xml'),str(gen.ORIGINAL)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w')as log:subprocess.run(command,check=True,stdout=log,stderr=subprocess.STDOUT)
    report=inventory((out/'cell.xml').read_bytes());result=classify(report,manifest)
    if args.negative_controls:result['failure_controls']=controls(report,manifest)
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/'inventory.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    paths=[Path(__file__).resolve(),gen.HERE/'generate_f10145.py',gen.ORIGINAL,gen.ROOT/'tools/sil_state_inventory.py']
    (out/'provenance.json').write_text(json.dumps({str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths},indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='decisions'},indent=2))
