#!/usr/bin/env python3
"""Elaborate actual QSF-selected nineteen-board native sources and account state.

Board geometry is compared with qualified original inventories. Unadapted
wrapper fields are preserved as explicit REQUIRED/ACCESS PENDING state.
The top emu/UIO, external SDRAM bytes and ARM services are outside this XML.
This is neither a native pause proof nor a complete machine snapshot.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import xml.etree.ElementTree as ET
import generate_serial_boards as gen
from classify_serial_boards import read_inventory,alias_controls
from classify_msa_board import PRIVATE as MSA_PRIVATE
from classify_msa_transport import FABRIC,GROUP
from classify_native_msa import INSTANCES
from test_msa_board import physical_fields
from sil_state_inventory import constant

ROOT=gen.ROOT
PREFIX='dorado_native_machine.'


def expected_fields(reports):
    expected={}
    for name,*_ in gen.fmt.SPECS:
        original=gen.qualified(reports,name);prefix=original['manifest']['module']+'.'
        physical={r['path'] for r in original['classification']['decisions'] if r['scope']=='PHYSICAL'}
        for row in original['inventory']['candidates']:
            if row['category']!='sequential':continue
            assert row['path'].startswith(prefix)
            path=PREFIX+'m.b_'+name+'.'+row['path'][len(prefix):]
            expected[path]=(row['width'],row['bits'],row['unpacked_dimensions'],
                            'PHYSICAL BOARD' if row['path'] in physical else 'PRIVATE BOARD')
    for name in INSTANCES:
        for row in physical_fields():expected[PREFIX+'m.'+name+'.'+row['path']]=(row['width'],row['width'],[],'PHYSICAL BOARD')
        for path,width in MSA_PRIVATE.items():expected[PREFIX+'m.'+name+'.'+path]=(width,width,[],'PRIVATE BOARD')
    for instance,fields in [('u_snapshot_fabric',FABRIC),('u_snapshot_msa',GROUP)]:
        for path,width in fields.items():expected[PREFIX+'m.'+instance+'.'+path]=(width,width,[],'PRIVATE TRANSPORT')
    root=reports/'2026-10-19-snapshot-serial-board-global'
    support=json.loads(gen.raw_receipt(root,'native-support-review/machine-support-review.json'))
    for row in support['candidates']:
        if row['category']!='sequential':continue
        if row['path'].startswith(tuple(PREFIX+'m.'+n+'.' for n in INSTANCES)+
                                  (PREFIX+'m.u_snapshot_fabric.',)):continue
        assert row['path'] not in expected
        expected[row['path']]=(row['width'],row['bits'],row['unpacked_dimensions'],'WRAPPER ACCESS PENDING')
    return expected


def bindings(normalized):
    tree=ET.fromstring(normalized);modules={m.get('name'):m for m in tree.findall('.//module')}
    values={}
    for cell in tree.findall('.//cells//cell'):
        path=cell.get('hier');module=modules[cell.get('submodname')]
        if path in [PREFIX+'m.b_'+name for name,*_ in gen.fmt.SPECS]+[PREFIX+'m.'+n for n in INSTANCES]+[PREFIX+'m.u_snapshot_fabric']:
            values[path]={v.get('name'):constant(v.find('const').get('name'))for v in module.findall('./var')
                          if(v.get('param')=='true'or v.get('localparam')=='true')and v.find('const')is not None}
    return values


def classify(data,expected):
    remaining=dict(expected);decisions=[]
    for row in data['candidates']:
        if row['category']!='sequential':continue
        wanted=remaining.pop(row['path'],None)
        assert wanted is not None,'extra/duplicate native state: '+row['path']
        width,bits,dimensions,scope=wanted
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(width,bits,dimensions),'native geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=bits,scope=scope))
    assert not remaining,'missing native state: '+str(list(remaining)[:3])
    totals=Counter()
    for row in decisions:totals[row['scope']]+=row['bits']
    assert dict(totals)=={'PHYSICAL BOARD':1966264,'PRIVATE BOARD':4594,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132484},dict(totals)
    wanted={PREFIX+'m.b_'+name:dict(SYSPER=4 if name in ('DskEth','BaseBd')else 16)for name,*_ in gen.fmt.SPECS}
    wanted[PREFIX+'m.b_MemD']['STORAGE_INPUT_DELAY']=2
    wanted.update({PREFIX+'m.'+name:dict(SYSPER=4)for name in INSTANCES})
    wanted[PREFIX+'m.u_snapshot_fabric']=dict(RESPONSE_LIMIT=4096,SERVICE_DOMAINS=5,AGE_BITS=12)
    assert data['bindings']==wanted,'actual board/deadline parameter bindings differ'
    return dict(status='PASS_BOARD_GEOMETRY_WRAPPER_ACCESS_PENDING',scope=__doc__,totals=dict(totals),
                sequential_exclusions=[],decisions=decisions,bindings=data['bindings'],
                xml_sha256=data['xml_sha256'],normalized_xml_sha256=data['normalized_xml_sha256'],
                resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,expected):
    results=[]
    for case in ('missing_board','missing_msa','missing_wrapper','missing_owner','duplicate','extra','width','dimensions','nonsequential','clock_binding','input_delay','deadline'):
        broken=copy.deepcopy(data);rows=broken['candidates']
        target=next(r for r in rows if r['path']==PREFIX+'m.b_msa.u_storage_0.old_data')
        if case=='missing_board':broken['candidates']=[r for r in rows if not r['path'].startswith(PREFIX+'m.b_BaseBd.')]
        elif case=='missing_msa':broken['candidates']=[r for r in rows if not r['path'].startswith(PREFIX+'m.b_msa3Odd.')]
        elif case=='missing_wrapper':rows.remove(next(r for r in rows if r['path']==PREFIX+'u_media.buffer'))
        elif case=='missing_owner':rows.remove(next(r for r in rows if r['path']==PREFIX+'m.u_snapshot_msa.owner'))
        elif case in ('duplicate','extra'):
            row=copy.deepcopy(target)
            if case=='extra':row['path']+='_hidden'
            rows.append(row)
        elif case=='width':target['width']+=1
        elif case=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif case=='nonsequential':target['category']='initial_only_candidate'
        elif case=='clock_binding':broken['bindings'][PREFIX+'m.b_DispY']['SYSPER']=4
        elif case=='input_delay':broken['bindings'][PREFIX+'m.b_MemD']['STORAGE_INPUT_DELAY']=0
        else:broken['bindings'][PREFIX+'m.u_snapshot_fabric']['RESPONSE_LIMIT']=2048
        try:classify(broken,expected)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged combined native state accepted: '+case)
    return results


def run(project,reports,out):
    qsf=project/'core/Dorado.qsf';sources=[];defines=[]
    for line in qsf.read_text().splitlines():
        words=shlex.split(line,comments=True)
        if words[:3]==['set_global_assignment','-name','VERILOG_MACRO']:
            assert len(words)==4;defines.append(words[3])
        if words[:3] in (['set_global_assignment','-name','VERILOG_FILE'],['set_global_assignment','-name','SYSTEMVERILOG_FILE']):
            assert len(words)==4
            path=(project/'core'/words[3]).resolve()
            if path.is_relative_to(project/'src'):sources.append(path)
    sources=list(dict.fromkeys(sources));defines=sorted(set(defines))
    assert all(p.is_file()for p in sources)
    assert 'DORADO_STORAGE_INPUT_DELAY=2'in defines and 'DORADO_HOST_CLOCK_HZ=50000000'in defines
    out.mkdir(parents=True,exist_ok=False)
    command=['verilator','--xml-only','--timing','--assert','--top-module','dorado_native_machine','-Wno-fatal',
             *[f'-I{project/"src"/part}'for part in ('verilog/cells','verilog/generated','verilog/vendor/6502','verilog/vendor/riot','fpga/mister/snapshot_serial')],
             *['+define+'+d for d in defines],'--xml-output',str(out/'native.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    provenance=dict(scope=__doc__,macros=defines,qsf_sha256=gen.sha(qsf.read_bytes()),
                    source_hashes={str(p.relative_to(project)):gen.sha(p.read_bytes())for p in sources},tool_sha256=gen.sha(Path(__file__).read_bytes()))
    (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,cwd=project/'src',stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'native.xml').read_bytes();data,normalized=read_inventory(raw);data['bindings']=bindings(normalized)
    (out/'native-normalized.xml').write_bytes(normalized)
    (out/'inventory.json').write_text(json.dumps(data,indent=2)+'\n')
    expected=expected_fields(reports)
    (out/'expected-fields.json').write_text(json.dumps(expected,indent=2)+'\n')
    result=classify(data,expected)
    result['failure_controls']=controls(data,expected)+alias_controls(raw)
    (out/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],failure_controls=len(result['failure_controls']))),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.project.resolve(),a.reports.resolve(),a.output.resolve())
