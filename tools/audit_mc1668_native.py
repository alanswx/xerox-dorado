#!/usr/bin/env python3
"""Read terminal MC1668 native evidence against the immutable Native59 baseline.

Two qualified source deltas only; unchanged clocks, constraints and services.
Reuse the existing strict memory/geometry/OLD_DATA readers and all196 detailed
timing categories. A passing build is not a firmware/desktop qualification.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re
import sys

import stage_native_speed as baseline
from stage_mc1668_native import CELL,ADAPTER

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'fpga/mister/snapshot'))
import audit_native_dsketh as readers
from audit_native_conta import ucp


def read(path):
    if path.exists():return path.read_bytes()
    return gzip.decompress(Path(str(path)+'.gz').read_bytes())


def sha(raw):return hashlib.sha256(raw).hexdigest()


def mc_rows(text):
    start=text.index('; Analysis & Synthesis Resource Utilization by Entity')
    rows={};header=None
    for line in text[start:].split('\n\n',1)[0].splitlines():
        fields=[x.strip()for x in line.split(';')[1:-1]]
        if fields and fields[0]=='Compilation Hierarchy Node':header=fields
        elif header and len(fields)==len(header) and 'cell_MC1668_snapshot:'in fields[0]:
            r=dict(zip(header,fields));name=r['Full Hierarchy Name']
            m=re.search(r':b_(MemC|ProcH|ProcL)\|cell_MC1668_snapshot:(u_\w+)',name);assert m,name
            c=re.fullmatch(r'(\d+) \((\d+)\)',r['Dedicated Logic Registers']);assert c and c[1]==c[2]
            rows['/'.join(m.groups())]=int(c[1])
    assert len(rows)==12
    return rows


def audit(root,qualified,initial):
    r=json.loads((root/'mc1668-candidate.json').read_text());q=json.loads((qualified/'results.json').read_text())
    assert q['status']=='PASS_NATIVE_MC1668_NORMAL_OPERATION'and q['state_admin']=='INACTIVE'
    assert r['intended_deltas']==q['candidate']and r['state_admin']=='INACTIVE'
    assert r['frequency_mhz']==50 and r['sysper']==4 and r['physical_instances']==12
    initial_state=json.loads((initial/'run-state.json').read_text())
    initial_inputs=json.loads((initial/'provenance.json').read_text())
    assert initial_state['status']=='PASS_MC1668_INITIAL_COMPONENT'
    assert [a['stage']for a in initial_state['actions']]==['compile','test']
    assert all(a['returncode']==0 for a in initial_state['actions'])
    assert initial_inputs['candidate_deltas']==r['intended_deltas']
    assert initial_inputs['inputs']['verilog/cells/cell_MC1668.v']==q['inputs']['cell_MC1668.v']
    for name,digest in initial_inputs['inputs'].items():
        assert sha((initial/'executed-inputs'/name).read_bytes())==digest,('changed Initial input',name)
    initial_text=(initial/'test.log').read_text()
    assert '960 microinstructions written, 960 read back, 0 wrong'in initial_text
    match=re.search(r'INITIAL ROM PASS: original payload, CPU manifold ownership, 15 task initializations, all six error sources enabled for (\d+) samples without error',initial_text)
    assert match and int(match[1])>=100000
    gold,compiled=baseline.baseline(baseline.REPORTS);assert r['baseline_compiled_inputs']==compiled
    project=Path(r['project']);actual={}
    for name,raw in gold.items():
        value=(root/name).read_bytes();actual[name]=sha(value)
        if name in(CELL,ADAPTER):
            unit_name=Path(name).name
            assert actual[name]==q['inputs'][unit_name],('unqualified MC1668 copy',name)
        else:assert value==baseline.relocated(name,raw,project),('unintended native source/config delta',name)
    assert actual==r['source_hashes'],'recorded input coverage differs'
    state=json.loads((root/'run-state.json').read_text());assert state['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [x['stage']for x in state['stages']]==['map','fit','sta','clocks','corners','control-crossings','contb-crossings','adapter-crossings','asm']
    assert all(x['returncode']==0 for x in state['stages'])
    collected=json.loads((root/'collected-input-check.json').read_text())
    assert collected['status']=='TERMINAL_INPUT_READBACK'and len(collected['job_pids_absent'])==10
    assert collected['actual']==actual
    core=root/'core';mapping=(core/'output_files/Dorado.map.rpt').read_text()
    # Quartus17's fitter emits Latin-1 degree symbols in device/pin sections.
    # Keep raw bytes for hashing; the memory tables themselves are ASCII.
    fitted=(core/'output_files/Dorado.fit.rpt').read_text(encoding='latin-1')
    source=baseline.REPORTS/'2026-10-19-native59-dsketh'
    oldmap=read(source/'core/output_files/Dorado.map.rpt').decode()
    old=mc_rows(oldmap);new=mc_rows(mapping)
    expected={k:(1 if k in('MemC/u_b14','MemC/u_c14')else 2)for k in old}
    assert new==expected,('MC1668 normal storage differs',new,expected)
    assert sum(old.values())==38 and sum(new.values())==22
    metadata=lambda name:json.loads((root/'src/fpga/mister'/('snapshot_native_'+name)/'assets.json').read_text())
    memories={}
    memories['instruction']=readers.instruction_ram(mapping)
    m=metadata('processor');memories['processor']={title:readers.processor_ram(t,title,m,'snapshot')for t,title in((mapping,'Analysis & Synthesis RAM Summary'),(fitted,'Fitter RAM Summary'))}
    m=metadata('memc')['manifests']['MemC'];memories['memc']={title:readers.memc_ram(readers.memc_ram_sections(t),title,m,'snapshot')for t,title in((mapping,'Analysis & Synthesis RAM Summary'),(fitted,'Fitter RAM Summary'))}
    memories['ifu']=readers.ifu_ram_evidence(mapping,fitted,metadata('ifu')['manifests']['IFU'])
    m=metadata('memd')['manifests']['MemD']
    memories['memd_macros']=readers.memd_macros(readers.board_parameters(mapping,'MemD'),m)
    # Both exact cache layouts were independently qualified previously. No
    # arbitrary overlap, geometry, capacity or RDW relaxation is permitted.
    try:memories['cache']=readers.native_cache_physical(fitted,m,'independent40')
    except AssertionError:memories['cache']=readers.native_cache_physical(fitted,m,'paired38')
    m=metadata('memx')['manifests']['MemX'];memories['memx']=dict(macros=readers.memx_macros(readers.board_parameters(mapping,'MemX'),m,'snapshot'),physical=readers.memx_physical(readers.map_rows(fitted),m,'snapshot'))
    m=metadata('dispy')['manifests']['DispY'];memories['display']=readers.dispy_physical(fitted,m,'snapshot');memories['display_words']=readers.word_macros(readers.board_parameters(mapping,'DispY'))
    memories['dsketh']=readers.dsketh_evidence(mapping,fitted,metadata('dsketh')['manifests']['DskEth'],'snapshot')
    previous=json.loads(gzip.decompress((source/'qualification.json.gz').read_bytes()))
    assert memories['dsketh']['state']==previous['dsketh_memories']['snapshot']['state']
    paths=[];coverage={}
    for corner in sorted(readers.CORNERS):
        for mode in readers.MODES:
            name=f'native59-{mode}-{corner}.rpt';paths.append(readers.timed((core/name).read_text(),name))
        current=ucp((core/f'native59-ucp-{corner}.rpt').read_text());before=ucp(read(source/f'core/native59-ucp-{corner}.rpt').decode())
        assert current==before,('unconstrained coverage differs',corner,current,before)
        coverage[corner]=current
    assert len(paths)==196
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',(core/'output_files/Dorado.sta.summary').read_text())
    assert len(checks)==31 and min(float(v)for _,v in checks)>=0
    crossings=re.findall(r'ADAPTER_CROSSING_CORNER (\S+) (\w+) board=(\d+) others=(\d+)',(root/'adapter-crossings.log').read_text())
    assert len(crossings)==40 and {(c,b)for c,b,_,_ in crossings}=={(c,b)for c in readers.CORNERS for b in readers.BOARDS}
    assert all(int(a)>0 and int(b)>0 for _,_,a,b in crossings)
    warnings={}
    for phase in('map','fit'):
        now=(root/(phase+'.log')).read_text();was=read(source/(phase+'.log')).decode()
        assert 'Critical Warning'not in now
        increase=Counter(re.findall(r'Warning \((\d+)\)',now))-Counter(re.findall(r'Warning \((\d+)\)',was))
        assert not(set(increase)-{'10036'}),('new warning category/count',phase,increase)
        warnings[phase]=dict(increase)
    summary=(core/'output_files/Dorado.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
    assert 'Assembler was successful'in(root/'asm.log').read_text()
    resources={}
    for key,label in [('alms','Logic utilization (in ALMs)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks'),('dsp','Total DSP Blocks')]:
        hit=re.search(re.escape(label)+r'\s*:\s*([\d,]+)',summary);assert hit;resources[key]=int(hit[1].replace(',',''))
    assert resources['ram_blocks']==332-40+memories['cache']['unique_m10ks']and resources['dsp']==35
    rbf=core/'output_files/Dorado.rbf';assert rbf.stat().st_size>1000000
    return dict(status='PASS_NATIVE_MC1668_BUILD',scope=__doc__,rbf_sha256=sha(rbf.read_bytes()),
        frequency_mhz=50,sysper=4,physical_instances=12,source_deltas=r['intended_deltas'],
        source_hashes=actual,resources=resources,old_mc1668_registers=old,new_mc1668_registers=new,
        memory_evidence=memories,timing_checks=checks,detailed_paths=paths,coverage=coverage,warnings=warnings,
        minimum_slack_ns=min(x['slack_ns']for x in paths),state_admin='INACTIVE',
        initial_component_enabled_samples=int(match[1]),
        firmware_qualified=False,hardware_speedup_claimed=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('root',type=Path);p.add_argument('--qualified',type=Path,required=True);p.add_argument('--initial',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    r=audit(a.root.resolve(),a.qualified.resolve(),a.initial.resolve());a.output.write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],r['minimum_slack_ns'],r['resources'],r['rbf_sha256'])
