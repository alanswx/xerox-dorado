#!/usr/bin/env python3
"""Review conservative native844 integration, preserving rejected diagnostics.

This receipt proves inventory, bounded component comparisons and native
elaboration only. Live original-ROM prefixes are observations, never passes.
No full boot, future60ns phase contract, FPGA timing or speed qualification.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from archive_r_front_ready_followup import verify
from inventory_r_front_control_native import PATHS
from review_r_front_control import read_run

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();dirs=[]
    invdir=BUILD/'r-front-control-native-inventory-v1-20261023';inv=read(invdir/'results.json');dirs.append(invdir)
    assert inv['status']=='PASS_PROCESSOR_CONTROL_NATIVE_INVENTORY'and inv['verified_native_inputs']==530
    for n,h in inv['inputs'].items():assert sha(invdir/'inputs'/n)==h,n
    assert sorted(i for p in inv['packages']for i in p['global_Q_bits_lsb_first'])==list(range(844))
    assert inv['evaluator_owned_Q_bits']==828 and inv['retained_visible_R_Qs']==list(range(572,588))
    assert sum(p['physical_bits']for p in inv['RAM_packages'])==9216
    proof_rows=[]
    for kind in ['late','wrong']:
        d=BUILD/f'native-refresh-order-{kind}-v2-20261023';p=read(d/'results.json');dirs.append(d)
        for n,h in p['inputs'].items():assert sha(d/'inputs'/n)==h,n
        assert sha(d/'tb.sv')==p['testbench_sha256']
        assert all('returncode'in a for a in p['actions'])and p['actions'][0]['returncode']==0
        log=(d/'test.log').read_text()
        if kind=='late':
            assert p['status']=='PASS_FROZEN_NATIVE_REFRESH_ORDER'and not p['assume_early']
            assert 'PASS checks=16 release_cases=1 old_Set_at_capture=1'in log and p['actions'][-1]['returncode']==0
        else:
            assert p['status']=='NATIVE_REFRESH_ORDER_FAILED'and p['assume_early']and p['actions'][-1]['returncode']!=0
            assert 'mismatch check=4 old_refresh=1 want=0 cpu=0 stop=0 expected=0 actual=11'in log
        proof_rows.append(dict(run=d.name,status=p['status'],result_sha256=sha(d/'results.json')))
    comparisons=[]
    for kind in ['owned-inputs','input-cones']:
        pair=[]
        for engine in ['icarus','verilator']:
            d=BUILD/f'r-front-control-{kind}-{engine}-v1-20261023';r,log=read_run(d);dirs.append(d)
            assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['actions'][-1]['returncode']==0
            assert r['summary']=='checks=10121 writes=242 reads=4166 epochs=391'
            assert r['rbase_close']and r['actual_RBase_StkP_directed']
            assert bool(r.get('owned_r_inputs_zeroed'))==(kind=='owned-inputs')
            assert bool(r.get('partitioned_input_cones'))==(kind=='input-cones')
            clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
            assert len(clocks)==100 and [int(c)for c,n in clocks if int(n)==0]==[43,91,92,93,95]
            pair.append(clocks)
            comparisons.append(dict(run=d.name,engine=engine,summary=r['summary'],run_state_sha256=sha(d/'run-state.json')))
        assert pair[0]==pair[1]
    stages=[]
    for version in range(1,8):
        d=BUILD/f'r-front-control-native-stage-v{version}-20261023';s=read(d/'r-front-control-native.json');dirs.append(d)
        for n,h in s['source_hashes'].items():assert sha(d/n)==h,(d,n)
        assert s['physical_Q_bits']==844 and s['evaluator_owned_Q_bits']==828 and s['native_visible_R_bits']==16
        assert s['RAM_bits']==9216 and s['replaced_storage_packages']==119 and s['replaced_RAM_packages']==12
        assert s['frequency_hz']==57142857 and s['sysper']==4
        assert not any(s[k]for k in ['assembly_permitted','firmware_qualified','physical_timing_qualified','future60ns_partition_qualified','initialization_or_guest_state_added'])
        if version>1:assert not s['physical_RAM_read_signals_tied_off']
        stages.append(dict(directory=d.name,manifest_sha256=sha(d/'r-front-control-native.json'),source_files=len(s['source_hashes'])))
    # v7 differs from v2 only by zeroing unused18 input slots at their export
    # origins; all real physical data/parity consumers remain byte-identical.
    a=BUILD/'r-front-control-native-stage-v2-20261023';b=BUILD/'r-front-control-native-stage-v7-20261023'
    changed=[]
    for board,n in PATHS.items():
        before=(a/n).read_text();after=(b/n).read_text();expected=before
        for pin in range(127,145):expected=re.sub(rf'(assign r_front_pins\[{pin}\]=)[^;]+;',rf"\g<1>1'b0;",expected)
        assert expected==after,(board,'Unexpected physical wire or consumer change')
        if before!=after:changed.append(board)
    assert changed==['ProcH','ProcL']
    lints=[]
    names=['r-front-control-native-structural-baseline-v1-20261023']
    names += [f'r-front-control-native-structural-candidate-v{v}-20261023'for v in range(1,8)]
    names += ['r-front-control-native-unoptflat-v1-20261023','r-front-control-native-unoptimized-v1-20261023']
    for name in names:
        d=BUILD/name;r=read(d/'run-state.json');dirs.append(d)
        assert r['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and r['returncode']==0
        assert r['structural_warnings_visible']and '-Wno-UNOPTFLAT'not in r['command']and '-Wno-MULTIDRIVEN'not in r['command']
        assert sha(Path(r['stage'])/'r-front-control-native.json')==r['stage_manifest_sha256']
        log=(d/'lint.log').read_text();counts=dict(Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',log,re.M)))
        lints.append(dict(directory=name,run_state_sha256=sha(d/'run-state.json'),warnings=counts))
    assert lints[0]['warnings']['UNOPTFLAT']==1 and lints[0]['warnings']['MULTIDRIVEN']==1
    assert all(x['warnings']['UNOPTFLAT']==3 for x in lints[1:7])
    assert lints[7]['warnings']['UNOPTFLAT']==1 and lints[7]['warnings']['MULTIDRIVEN']==1
    native=[]
    for version in [1,2,3]:
        d=BUILD/f'r-front-control-native-cold-v{version}-20261023';r=read(d/'run-state.json');dirs.append(d)
        p=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()))
        for n,h in p['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
        assert not p['baseline_inputs_or_limits_changed']and not p['firmware_qualified']
        if r['status']=='RUNNING':
            action=r['actions'][-1];pid=action['pid']
            observed=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,command='],capture_output=True,text=True)
            assert observed.returncode==0 and str(d)in observed.stdout,(d,'Recheck authoritative job state')
            row=dict(directory=d.name,status='VERIFIED_LIVE_PREFIX_ONLY',pid=pid,ps=observed.stdout.strip(),observed=time.time())
        else:
            assert all('returncode'in x for x in r['actions'])
            row=dict(directory=d.name,status=r['status'],host_limit=p['host_limit'])
            if version==1:
                assert p['host_limit']==12000000 and 'NATIVE844_PREFIX samples='not in(d/'test.log').read_text()
                row['qualification']='INSUFFICIENT_BEFORE_PHYSICAL_INITIALIZATION'
        row.update(run_state_sha256=sha(d/'run-state.json'),provenance_sha256=sha(d/'provenance.json.gz'),firmware_qualified=False)
        native.append(row)
    out.mkdir(parents=True)
    result=dict(status='PASS_CONSERVATIVE_NATIVE844_STAGING_AND_ELABORATION',scope=__doc__,inventory_sha256=sha(invdir/'results.json'),refresh_contract=proof_rows,
        comparisons=comparisons,stages=stages,structural_elaborations=lints,native_prefixes=native,
        export_change_only=['ProcH/ProcL unused evaluator slots127..144'],physical_RAM_consumers_changed_by_export_fix=False,
        canonical_R_owners=16,evaluator_owned_Qs=828,physical_Qs=844,RAM_bits=9216,
        ordinary_cadence_changed=False,frequency_hz=57142857,sysper=4,
        full_boot_qualified=False,future60ns_qualified=False,FPGA_timing_qualified=False,speedup_qualified=False,
        unexercised_clocks=[43,91,92,93,95],preserved_early_RBase_overlap_failure=True,
        next='Original-ROM native startup/producer correctness, accurate60ns capture phases/full15ns timing, remaining clock/tasking/timer/SDRAM and full A-F acceptance.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result,dirs


def archive(out,review_dir,dirs):
    assert not out.exists();paths={}
    for d in dirs:
        for p in d.rglob('*'):
            rel=p.relative_to(d)
            if not p.is_file():continue
            if 'obj'in rel.parts and p.suffix!='.dot':continue
            if p.suffix in {'.vvp','.o','.a','.rbf','.sof','.pack'}:continue
            paths['runs/'+d.name+'/'+str(rel)]=p
    for p in review_dir.iterdir():
        if p.is_file():paths['review/'+p.name]=p
    for n in ['inventory_r_front_control_native.py','test_native_refresh_order.py','stage_r_front_control_native.py','lint_r_front_control_native.py','run_r_front_control_native.py','test_r_front_control.py','rewrite_r_front_input_cones.py',Path(__file__).name]:paths['tools/'+n]=ROOT/'tools'/n
    out.mkdir(parents=True);r=dict(scope=__doc__,members=[],blobs={})
    for name,p in sorted(paths.items()):
        with p.open('rb')as f:raw=f.read(p.stat().st_size)
        if p.suffix=='.log'and raw and not raw.endswith(b'\n'):raw=raw[:raw.rfind(b'\n')+1]
        assert not raw.startswith(b'\x7fELF'),p
        h=hashlib.sha256(raw).hexdigest()
        if h not in r['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(exist_ok=True);target.write_bytes(gzip.compress(raw,mtime=0))
            r['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        r['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    (out/'summary.json').write_bytes((review_dir/'results.json').read_bytes());verify(out)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();result,dirs=review(a.output.resolve());print(result['status'])
    if a.archive:archive(a.archive.resolve(),a.output.resolve(),dirs)
