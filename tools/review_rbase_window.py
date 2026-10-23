#!/usr/bin/env python3
"""Review legal RBase close transfers and actual-source directed integration.

The original early-phase overlap remains a counterexample in both engines.
This review does not promote the whole processor, qualify native timing or
replace missing tasking/clock/producer tests with a passing RBase component.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from archive_r_front_ready_followup import verify
from review_r_front_control import read_run as read_full
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
PROFILES=['balanced','fast_gates','later_control','parc_parity_min','parc_parity_max','later_r_memory']


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def primitive_run(d):
    r=json.loads((d/'run-state.json').read_text());assert r['status']!='RUNNING'
    assert len(r['actions'])==2 and r['actions'][0]['returncode']==0 and all('returncode'in a for a in r['actions'])
    for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
    assert sha(d/'candidate.sv')==r['candidate_sha256']and sha(d/'tb.sv')==r['testbench_sha256']
    return r,(d/'test.log').read_text()


def review(out):
    assert not out.exists();rows=[];dirs=[]
    helper=ROOT/'verilog/rtl/dorado_rbase_window_step.sv';helper_sha=sha(helper)
    for kind in ['correct','wrong','omit']:
        for engine in ['icarus','verilator']:
            d=BUILD/f'rbase-window-{kind}-{engine}-v3-20261023';r,log=primitive_run(d);dirs.append(d)
            assert r['engine']==engine
            assert r['inputs']['verilog/rtl/dorado_rbase_window_step.sv']==helper_sha
            if kind=='correct':
                assert r['status']=='PASS_RBASE_WINDOW'and r['actions'][-1]['returncode']==0
                assert r['candidate_sha256']==helper_sha
                assert r['summary']=='checks=768 closes=256 different_next_address=256 open_data_updates=256'
                assert 'RBASE_WINDOW_LEGAL_WRITES pulses=256 'in log
                result='PASS_LEGAL_RBASE_WINDOW'
            else:
                assert r['status']=='RBASE_WINDOW_FAILED'and r['actions'][-1]['returncode']!=0
                assert 'RBASE_WINDOW mismatch check=2 close=1 old_address=0 arrived_address=1'in log
                assert r['candidate_sha256']!=helper_sha
                result='REJECTED_NEXT_ADDRESS_AT_CLOSE'if kind=='wrong'else'REJECTED_OMITTED_FINAL_CAPTURE'
            rows.append(dict(run=d.name,engine=engine,result=result,run_state_sha256=sha(d/'run-state.json')))
    primary=[];overlaps=[]
    for profile in PROFILES:
        for engine in ['icarus','verilator']:
            name=f'r-front-rbase-close-directed-{engine}-v2-20261023'if profile=='balanced'else f'r-front-rbase-close-directed-{profile}-{engine}-v3-20261023'
            d=BUILD/name;r,log=read_full(d);dirs.append(d)
            assert r['engine']==engine and r['profile']==profile and r['rbase_close']and r['actual_RBase_StkP_directed']
            assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['actions'][-1]['returncode']==0
            assert r['summary']=='checks=10121 writes=242 reads=4166 epochs=391'
            assert 'R_FRONT_LOCAL_DIRECTED RBase=32 StkP=256'in log
            assert r['inputs']['verilog/rtl/dorado_rbase_window_step.sv']==helper_sha
            change=r['adapters']['rbase_window_rewrite']
            assert change['additional_state_bits']==0 and change['unchanged_logic_inverse_byte_identical']
            clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
            assert len(clocks)==100
            missing=[int(c)for c,n in clocks if not int(n)];assert missing==[43,91,92,93,95]
            primary.append(dict(run=name,engine=engine,profile=profile,summary=r['summary'],clocks=clocks,
                                unexercised_clocks=missing,run_state_sha256=sha(d/'run-state.json')))
        assert primary[-1]['clocks']==primary[-2]['clocks']
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-rbase-close-early-{engine}-v1-20261023';r,log=read_full(d);dirs.append(d)
        assert r['rbase_close']and r['status']=='R_FRONT_LOCAL_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
        assert 'R front state mismatch check=1456'in log and re.findall(r'R_FRONT_DIFF bit=(\d+) ',log)==['644','645','646','647']
        overlaps.append(dict(run=d.name,engine=engine,check=1456,bits=[644,645,646,647],run_state_sha256=sha(d/'run-state.json')))
    board=load_board(str(ROOT/'chm/sil/ProcL-Rev-Ci.dm!1_/ProcL-Rev-Ci.wl'))
    pins={(p['pkg'],p['pin']):n for n,r in board.nets.items()for p in r['pins']}
    checks={('g15',13):"RBaseWrite'",('a19',9):"PreSHCP'C",('i18',14):"RBaseWrite'",('i18',9):"RBaseWriteEn'"}
    for pin,n in checks.items():assert pins[pin]==n
    for pkg,kind in [('g15','F10145A'),('g14','MC10173'),('a19','MC10158'),('i18','SE10212')]:assert board.packages[pkg]['type']==kind
    source=(ROOT/'tools/generate_r_front_reference.py').read_text()
    assert "'{l.g15.p10,l.g15.p9,l.g15.p7,l.g15.p6}'"in source
    assert "'{l.g15.p12,l.g15.p11,l.g15.p4,l.g15.p5}'"in source
    out.mkdir(parents=True)
    record=dict(status='PASS_BOUNDED_RBASE_CLOSING_CAPTURE',scope=__doc__,helper_sha256=helper_sha,
                primitive=rows,primary=primary,preserved_overlap_failures=overlaps,
                physical_bits=844,ram_bits=9216,clock_channels=100,additional_architectural_state_bits=0,
                converted_directed=dict(RBase_fills=32,RBase_addresses=16,StkP_values=256),
                original_tasking_directed_cases_qualified=False,unexercised_clock_channels=[43,91,92,93,95],
                full_producer_arrival_contract_qualified=False,physical_timing_qualified=False,native_integrated=False,hardware_speedup_claimed=False,
                next='Wire the explicit pre-transition close into the native capture schedule with setup/hold and unchanged60ns cadence; close missing clock/tasking and IM/parity/MCR/hold-data/DMD/B-bus producers, then full native timing/ROM gates.')
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n');print(record['status'],'12 directed comparisons;2 legal primitives;4 negative captures;2 preserved overlap failures')
    return record,dirs


def archive(out,review_dir):
    assert not out.exists();r=json.loads((review_dir/'results.json').read_text())
    names={row['run']for key in ['primitive','primary','preserved_overlap_failures']for row in r[key]}
    names.update(d.name for d in BUILD.glob('rbase-window-*-20261023')if(d/'run-state.json').exists())
    names.update(f'r-front-rbase-close-{engine}-v1-20261023'for engine in ['icarus','verilator'])
    paths={}
    for name in sorted(names):
        d=BUILD/name
        if name.startswith('rbase-window-'):primitive_run(d)
        else:read_full(d)
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in {'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}:
                paths['runs/'+name+'/'+str(p.relative_to(d))]=p
    for p in review_dir.rglob('*'):
        if p.is_file():paths['review/'+str(p.relative_to(review_dir))]=p
    prior=BUILD/'rbase-window-reviewed-v1-20261023/results.json'
    if prior.exists()and prior.parent!=review_dir:paths['prior-review/results.json']=prior
    for name in ['tools/test_rbase_window.py','tools/rewrite_rbase_window.py','tools/test_r_front_control.py',
                 'verilog/rtl/dorado_rbase_window_step.sv','tools/'+Path(__file__).name]:paths['current/'+name]=ROOT/name
    out.mkdir(parents=True);receipt=dict(status=r['status'],scope=__doc__,members=[],blobs={},helper_sha256=r['helper_sha256'])
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(gzip.compress(raw,mtime=0))
            receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0));verify(out)
    (out/'summary.json').write_text(json.dumps(r,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();review(a.output.resolve())
    if a.archive:archive(a.archive.resolve(),a.output.resolve())
