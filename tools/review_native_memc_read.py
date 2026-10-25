#!/usr/bin/env python3
"""Retain measured Ready/R-read timing and the next actual MemC read boundary.

Formal and cell replays establish the tagged SRAM contract. The native stage,
startup process and timing build remain separate, explicitly scoped evidence.
Never turn successful fitter exit status or a running boot into qualification.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from qualify_memc_read_buffer import CELL,RTL,transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_native_registered_read import review as review_previous,proof_check
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from stage_native_memc_read_buffer import BOARD,board,backplane
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'qualify_memc_read_buffer.py','test_memc_read_buffer.py','stage_native_memc_read_buffer.py',
       'prepare_registered_read_native.py','review_native_registered_read.py','review_native_cp_read_timing.py',
       'lint_r_front_control_native.py','start_native_phase_guard_timing.py']


def read(p):return json.loads(p.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    timing=[BUILD/'native-ready-compare-full15-timing-review-v2-20261025',BUILD/'native-r-read-buffer-full15-timing-review-v1-20261025']
    previous=review_previous(out/'ready-and-R-read',BUILD/'native-memc-registered-read-startup-v1-20261025',timing)
    assert not previous['pending_verified_live_Icarus']
    rejection=BUILD/'native-memc-read-review-v1-20261025'
    assert(rejection/'failure.json').is_file()
    rejected=out/'review-rejection';rejected.mkdir()
    for n in ['failure.json','review_native_memc_read.py','review_native_registered_read.py']:shutil.copy2(rejection/n,rejected/n)
    proof=BUILD/'memc-read-buffer-proof-v2-20261025';p=proof_check(proof,'PASS_ACTUAL_MEMC_TAGGED_SRAM_READ')
    assert p['initial_RAM_bits_unconstrained']==1024 and p['candidate_sha256']==sha(ROOT/RTL)
    assert(ROOT/RTL).read_text()==transform((proof/'original.sv').read_text())
    assert {a['label']for a in p['actions']}=={'contracts','missing-write-forward','ignore-address-tag','wrong-guest-data'}
    retain(proof,out/proof.name)
    cell_checks=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'memc-read-buffer-cell-{engine}-v2-20261025';s=checked(d)
        assert s['status']=='PASS_ACTUAL_MEMC_READ_CELL_REPLAY'and s['proof_state_sha256']==sha(proof/'run-state.json')
        assert 'checks=8708 matches=6155 misses=2553 admin=1024 guest_writes=1024 RAM=1024'in(d/'test.log').read_text()
        assert sha(d/'buffered.sv')==p['candidate_sha256'];retain(d,out/d.name)
        cell_checks.append(dict(engine=engine,state_sha256=sha(d/'run-state.json')))
    for name in ['memc-read-buffer-proof-v1-20261025','memc-read-buffer-cell-icarus-v1-20261025','memc-read-buffer-cell-verilator-v1-20261025']:
        d=BUILD/name;s=read(d/'run-state.json');assert s['status'].startswith('TERMINAL_')
        assert s['actions'][0]['returncode']!=0;retain(d,out/name)
    stage=BUILD/'native-memc-read-buffer-full15-stage-v1-20261025';m=read(stage/'r-front-control-native.json');verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);pm=read(parent/'r-front-control-native.json')
    assert len(m['source_hashes'])==577 and m['MemC_tag_read_proof_state_sha256']==sha(proof/'run-state.json')
    assert sha(stage/CELL)==p['source_cell_sha256']
    actual,packages=board((parent/BOARD).read_text());bp=m['native_backplane_binding_source']
    assert packages==m['MemC_tag_read_packages']and len(packages)==21
    changed={BOARD:actual.encode(),bp:backplane((parent/bp).read_text()).encode(),'src/'+RTL:(ROOT/RTL).read_bytes()}
    assert sorted(changed)==m['incremental_functional_HDL_changes']
    assert m['all21_address_matches_gate_whole_machine_publication']and m['original_MemC_RAM_write_and_admin_updates_unchanged']
    for n in m['source_hashes']:
        raw=changed[n]if n in changed else(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(pm['project'].encode(),m['project'].encode())
        if n=='core/Dorado.qsf':raw+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(Path(m['project'])/('src/'+RTL))+'\n').encode()
        assert(stage/n).read_bytes()==raw,n
    lint=BUILD/'native-memc-read-buffer-full15-lint-v1-20261025';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['source_files']==199 and l['stage_clock_macro_checked_against_QSF']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert '-DDORADO_HOST_CLOCK_HZ=66666667'in l['command']
    assert len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M))==6
    launch=BUILD/'native-memc-read-buffer-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['builder_pid']==122249 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')and la['verified_source_count']==577
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path('/home/alans/mister/Dorado_CPRead15_MemCRegisteredRead_20261025')
s=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert s['builder_pid']==122249 and not s['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']}
assert actual==m['source_hashes']and len(actual)==577
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
row=dict(observed=time.time(),state=s,source_readback=actual)
if s['status']=='RUNNING':
 q=subprocess.run(['ps','-p',str(s['stages'][-1]['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert q.returncode==0 and 'quartus_'in q.stdout;row.update(verified_live=True,process=q.stdout.strip())
else:assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''
    (out/'same-memc-build-query.py').write_text(query)
    response=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-memc-build-query.stderr').write_text(response.stderr);assert response.returncode==0,response.stderr
    remote=json.loads(response.stdout);assert remote['source_readback']==m['source_hashes']
    (out/'same-memc-build-observation.json').write_text(json.dumps(remote,indent=2)+'\n')
    for d in [stage,lint,launch]:retain(d,out/d.name)
    startup=BUILD/'native-registered-read-startup-v1-20261025';s,row=observed(startup)
    if s['status']=='RUNNING':assert row['verified_live']
    retain(startup,out/startup.name)
    result=dict(status='REVIEWED_TIMING_REJECTIONS_AND_ACTUAL_MEMC_READ_BOUNDARY',scope=__doc__,
        ready_and_R_read=previous,MemC_initial_RAM_bits_arbitrary_per_cell=1024,MemC_assumptions=0,
        MemC_cell_checks=cell_checks,MemC_package_count=21,original_memory_and_write_admin_updates_retained=True,
        MemC_native_source_count=577,MemC_native_elaborated_sources=199,MemC_native_structural_warnings=6,
        same_MemC_timing_job=remote,original_ROM_R_read_startup=row,
        original60ns_schedule_qualified=False,hardware_speed_gain_measured=False,hardware_or_media_changed=False,full_goal_active=True,
        next_action='Follow SAME MemC122249 and fixed300M startup processes. Use all-corner paths to place control/Ready/Hold captures into the original overlapping60ns schedule; keep all storage/tag and settling barriers. Finish peripheral/SDRAM/firmware/input/Rollback qualification.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-memc-tag-read',extra_tools=TOOLS)
    print(r['status'],flush=True)
