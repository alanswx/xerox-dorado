#!/usr/bin/env python3
"""Review terminal native Ready/front evidence before any FPGA assembly.

Require the actual-wrapper Initial gate, exact staged sources, preserved
constraints, all unchanged memory contracts and explicit central-Q timing.
Never run Quartus, assemble, deploy or claim a measured hardware speedup.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

from audit_r_front_ready_native import BASELINE,raw,ram_table,synthesis_memory,synthesis_warnings
from fit_r_front_ready import RTL

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'fpga/mister/snapshot'))
import audit_native_dsketh as readers
from audit_native_conta import ucp


def sha(data):return hashlib.sha256(data).hexdigest()


def physical_front(text):
    rows=ram_table(text,'Fitter RAM Summary')
    selected={n:r for n,r in rows.items()if '|dorado_r_front_ready_step:r_front_evaluator|'in n}
    assert len(selected)==4,'Missing/extra native front physical RAM banks'
    seen=set();result={}
    for n,r in selected.items():
        hit=re.search(r'\|altdpram:(rm_h|rm_l|stk_h|stk_l)_rtl_0\|',n)
        assert hit and hit[1]not in seen,('Unknown/duplicate front physical bank',n)
        name=hit[1];seen.add(name)
        expected={'Type':'MLAB','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':'256','Port B Depth':'256','Port A Width':'9','Port B Width':'9',
            'Implementation Port A Depth':'256','Implementation Port B Depth':'256',
            'Implementation Port A Width':'9','Implementation Port B Width':'9',
            'Size':'2304','Implementation Bits':'2304','M10K blocks':'0','MLAB cells':'72',
            'Port A Input Registers':'yes','Port A Output Registers':'no',
            'Port B Input Registers':'no','Port B Output Registers':'no'}
        for k,v in expected.items():assert r.get(k)==v,('Native front physical RAM differs',name,k,r.get(k),v)
        result[name]=r
    assert seen=={'rm_h','rm_l','stk_h','stk_l'}
    return result


def retained_dsketh(mapping,fitted,manifest):
    evidence=readers.dsketh_evidence(mapping,fitted,manifest,'snapshot_r_front')
    prior=json.loads(raw(BASELINE/'qualification.json'))
    expected=dict(prior['memory_evidence']['dsketh']['state'])
    assert expected.pop('u_l24')==dict(kind='cell_MC10102',registers=0,block_memory_bits=0)
    assert evidence['state']==expected,'Disk/Ethernet state changed beyond the single zero-state gate'
    return evidence


def physical_instruction(text):
    rows=ram_table(text,'Fitter RAM Summary');seen=set();occupied=set();result={}
    for n,row in rows.items():
        if ':b_ContB|'not in n or 'snapshot_bank_'not in n:continue
        m=re.search(r'\|dorado_snapshot_f10415_enable_bank:snapshot_bank_(\d+)\|',n)
        assert m and int(m[1])not in seen,'Unknown/duplicate physical instruction bank'
        bank=int(m[1]);seen.add(bank)
        expected={'Type':'AUTO','Mode':'Simple Dual Port','Clock Mode':'Single Clock',
            'Port A Depth':'1024','Port B Depth':'1024','Port A Width':'18','Port B Width':'18',
            'Implementation Port A Depth':'1024','Implementation Port B Depth':'1024',
            'Implementation Port A Width':'18','Implementation Port B Width':'18',
            'Size':'18432','Implementation Bits':'18432','M10K blocks':'2','MLAB cells':'0',
            'Port A Input Registers':'yes','Port A Output Registers':'no',
            'Port B Input Registers':'yes','Port B Output Registers':'no',
            'Mixed Width RDW Mode':'Old data','ECC Mode':'Off','ECC Pipeline Registers':'No','MIF':'None'}
        for k,v in expected.items():assert row.get(k)==v,('Physical instruction RAM differs',bank,k,row.get(k),v)
        sites=[s.strip()for s in row['Location'].split(',')]
        assert len(sites)==len(set(sites))==2 and not occupied.intersection(sites),'Instruction banks overlap physically'
        assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',s)for s in sites)
        occupied.update(sites);result[str(bank)]=row
    assert seen==set(range(8))and len(occupied)==16,'Missing physical instruction RAM'
    return result


def review(root,candidate,baseline,initial,component):
    record=json.loads(raw(root/'r-front-ready-native.json'))
    expected=json.loads(raw(candidate/'r-front-ready-native.json'))
    assert record==expected,'Fitted candidate identity differs from tested candidate'
    assert record['state_admin']=='INACTIVE' and record['frequency_mhz']==50 and record['sysper']==4
    assert record['physical_bits']==752 and record['ram_bits']==9216
    for n,h in record['source_hashes'].items():
        assert sha(raw(root/n))==sha(raw(candidate/n))==h,('Compiled native input differs',n)
    old=json.loads(raw(baseline/'mc1668-candidate.json'))
    changed=[]
    for n,h in old['source_hashes'].items():
        original=raw(baseline/n);assert sha(original)==h,('Preserved MC1668 input changed',n)
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:
            original=original.replace(old['project'].encode(),record['project'].encode())
        if original!=raw(root/n):changed.append(n)
    assert set(changed)=={'core/Dorado.qsf',
        'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',
        *['src/fpga/mister/'+('snapshot_native_conta'if b=='ContA'else'snapshot_native_processor')+'/'+b+'_full_snapshot.sv'for b in ['ContA','ProcH','ProcL']]}
    additions={'src/verilog/rtl/'+n for n in [*RTL,'dorado_r_front_cone.sv']}
    assert set(record['source_hashes'])-set(old['source_hashes'])==additions
    # Preserve every original QSF assignment; only append evaluator sources.
    staged_qsf=raw(candidate/'core/Dorado.qsf')
    old_qsf=raw(baseline/'core/Dorado.qsf').replace(old['project'].encode(),record['project'].encode())
    tail=staged_qsf[len(old_qsf):]
    assert staged_qsf.startswith(old_qsf)
    assignments=[l for l in tail.decode().splitlines()if l and not l.startswith('#')]
    assert assignments==['set_global_assignment -name SYSTEMVERILOG_FILE '+record['project']+'/src/verilog/rtl/'+n for n in [*RTL,'dorado_r_front_cone.sv']]
    ip=json.loads(raw(initial/'provenance.json'));ist=json.loads(raw(initial/'run-state.json'))
    assert ist['status']=='PASS_ACTUAL_NATIVE_BOARDS_INITIAL_COMPONENT'and all(a['returncode']==0 for a in ist['actions'])
    assert ip['candidate_sha256']==sha(raw(root/'r-front-ready-native.json'))
    for n,h in ip['inputs'].items():assert sha(raw(initial/'executed-inputs'/n))==h,('Initial input differs',n)
    for n,h in ip['native_board_hashes'].items():assert record['source_hashes']['src/'+n]==h
    for n in additions:assert record['source_hashes'][n]==ip['inputs'][n[4:]]
    ib=raw(initial/'executed-inputs/verilog/verilator/tb_exec.sv').decode()
    assert 'Actual native Initial live producer aliases disagree'in ib
    assert 'Actual native Initial slice has unsettled/repeated event'in ib
    itext=raw(initial/'test.log').decode()
    m=re.search(r'INITIAL ROM PASS: original payload, CPU manifold ownership, 15 task initializations, all six error sources enabled for (\d+) samples without error',itext)
    assert m and int(m[1])>=100000
    assert '960 microinstructions written, 960 read back, 0 wrong'in itext
    state=json.loads(raw(root/'run-state.json'));central=json.loads(raw(root/'central-review-state.json'))
    assert state['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [s['stage']for s in state['stages']]==['map','fit','sta','clocks','corners','control-crossings','contb-crossings','adapter-crossings']
    assert all(s['returncode']==0 for s in state['stages'])
    assert central['status']=='TERMINAL_CENTRAL_REPORTS_REVIEW_REQUIRED' and len(central['actions'])==1
    assert central['actions'][0]['returncode']==0
    assert central['tcl_sha256']==sha(raw(root/'audit-r-front-ready.tcl'))
    assert raw(root/'audit-r-front-ready.tcl')==(ROOT/'tools/audit_r_front_ready_native.tcl').read_bytes()
    collected=json.loads(raw(root/'collected-input-check.json'))
    assert collected['status']=='TERMINAL_NATIVE_INPUT_READBACK'and collected['actual']==record['source_hashes']
    assert set(collected['absent_pids'])=={state['builder_pid'],*[s['pid']for s in state['stages']]}
    assert set(collected['absent_central_pids'])=={central['review_pid'],central['actions'][0]['pid']}
    core=root/'core';mapping=raw(core/'output_files/Dorado.map.rpt').decode()
    fitted=raw(core/'output_files/Dorado.fit.rpt').decode('latin-1')
    meta=lambda b:json.loads(raw(root/'src/fpga/mister'/('snapshot_native_'+b)/'assets.json'))
    memories={'synthesis':synthesis_memory(mapping,raw(BASELINE/'core/output_files/Dorado.map.rpt').decode()),
        'front_physical':physical_front(fitted),'instruction':readers.instruction_ram(mapping),
        'instruction_physical':physical_instruction(fitted)}
    memc=meta('memc')['manifests']['MemC']
    memories['memc']={title:readers.memc_ram(readers.memc_ram_sections(t),title,memc,'snapshot')for t,title in [(mapping,'Analysis & Synthesis RAM Summary'),(fitted,'Fitter RAM Summary')]}
    memories['ifu']=readers.ifu_ram_evidence(mapping,fitted,meta('ifu')['manifests']['IFU'])
    memd=meta('memd')['manifests']['MemD']
    memories['memd_macros']=readers.memd_macros(readers.board_parameters(mapping,'MemD'),memd)
    memories['cache']=readers.native_cache_physical(fitted,memd,'paired3_5_39')
    memx=meta('memx')['manifests']['MemX']
    memories['memx']=dict(macros=readers.memx_macros(readers.board_parameters(mapping,'MemX'),memx,'snapshot'),physical=readers.memx_physical(readers.map_rows(fitted),memx,'snapshot'))
    memories['display']=readers.dispy_physical(fitted,meta('dispy')['manifests']['DispY'],'snapshot')
    memories['display_words']=readers.word_macros(readers.board_parameters(mapping,'DispY'))
    memories['dsketh']=retained_dsketh(mapping,fitted,meta('dsketh')['manifests']['DskEth'])
    paths=[];coverage={}
    for c in sorted(readers.CORNERS):
        for mode in readers.MODES:
            n=f'native59-{mode}-{c}.rpt';paths.append(readers.timed(raw(core/n).decode(),n))
        values=ucp(raw(core/f'native59-ucp-{c}.rpt').decode())
        assert values==ucp(raw(BASELINE/f'core/native59-ucp-{c}.rpt').decode()),('Unconstrained native coverage differs',c)
        coverage[c]=values
    assert len(paths)==196
    modes=['internal-setup','internal-hold','in-setup','in-hold','out-setup','out-hold']
    modes += [b+'-'+d+'-'+m for b in ['conta','contb','proch','procl','ifu','memc','memx']for d in ['in','out']for m in ['setup','hold']]
    extra=[]
    for c in sorted(readers.CORNERS):
        for mode in modes:
            n=f'r-front-ready-{mode}-{c}.rpt';extra.append(readers.timed(raw(core/n).decode(),n))
    assert len(extra)==136
    log=raw(root/'central-crossings.log').decode()
    qrows=re.findall(r'R_FRONT_READY_CORNER (\S+) central_Q=(\d+) rest=(\d+)',log)
    assert len(qrows)==4 and {c for c,_,_ in qrows}==readers.CORNERS
    assert all(0<int(q)<=752 and int(rest)>0 for _,q,rest in qrows)
    assert len({int(q)for _,q,_ in qrows})==1
    b=re.findall(r'R_FRONT_READY_BOUNDARY (\S+) (\w+) registers=(\d+)',log)
    assert len(b)==28 and all(int(n)>0 for _,_,n in b)
    assert {(c,n)for c,n,_ in b}=={(c,n)for c in readers.CORNERS for n in ['conta','contb','proch','procl','ifu','memc','memx']}
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',raw(core/'output_files/Dorado.sta.summary').decode())
    assert len(checks)==31 and all(float(v)>=0 for _,v in checks),('Negative/missing all-device timing',checks)
    warnings={'map':synthesis_warnings(raw(root/'map.log').decode(),raw(BASELINE/'map.log').decode(),candidate,component)}
    current=raw(root/'fit.log').decode();before=raw(BASELINE/'fit.log').decode()
    from collections import Counter
    assert 'Critical Warning'not in current
    increase=Counter(re.findall(r'Warning \((\d+)\)',current))-Counter(re.findall(r'Warning \((\d+)\)',before))
    assert not(set(increase)-{'10036'}),('Unreviewed fitter warning increase',increase)
    warnings['fit']=dict(increase)
    summary=raw(core/'output_files/Dorado.fit.summary').decode();assert 'Fitter Status : Successful'in summary
    resources={}
    for key,label in [('alms','Logic utilization (in ALMs)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks'),('dsp','Total DSP Blocks')]:
        hit=re.search(re.escape(label)+r'\s*:\s*([\d,]+)',summary);assert hit,label
        resources[key]=int(hit[1].replace(',',''))
    assert resources['ram_blocks']==332-40+memories['cache']['unique_m10ks'] and resources['dsp']==35
    return dict(status='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW',scope=__doc__,
        source_hashes=record['source_hashes'],memory_evidence=memories,timing_checks=checks,
        detailed_paths=paths,central_paths=extra,unconstrained_coverage=coverage,warnings=warnings,
        central_Q_registers=int(qrows[0][1]),resources=resources,initial_component_enabled_samples=int(m[1]),
        minimum_slack_ns=min(p['slack_ns']for p in paths+extra),frequency_mhz=50,sysper=4,
        state_admin='INACTIVE',assembly_review_only=True,firmware_qualified=False,hardware_speedup_claimed=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('root',type=Path)
    for n in ['candidate','baseline','initial','component','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.root,a.candidate,a.baseline,a.initial,a.component)
    a.output.write_text(json.dumps(r,indent=2)+'\n');print(r['status'],r['minimum_slack_ns'],r['resources'])
