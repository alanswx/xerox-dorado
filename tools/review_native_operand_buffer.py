#!/usr/bin/env python3
"""Retain the actual pre-event operand theorem, prepared view and native fit.

Binary source chronology, tagged publication, full X/Z fallback and complete
packet replays have separate scopes. Native startup and the one new timing
build are observations until terminal. No hardware speed or60ns pass follows.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from integrate_native_operand_buffer import control,caller,HELPER
from prepare_registered_read_native import MONITOR,memc_monitor,operand_monitor
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_native_alu_capture import retain
from review_native_registered_read import proof_check
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from stage_native_operand_buffer import backplane,CALLER
from qualify_native_ready_compare import CONTROL
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'prove_native_operand_view_epoch.py','qualify_native_operand_buffer.py',
 'derive_native_operand_support.py','integrate_native_operand_buffer.py','replay_native_operand_buffer.py',
 'test_native_operand_unknown.py','stage_native_operand_buffer.py','start_native_phase_guard_timing.py',
 'lint_r_front_control_native.py','review_native_cp_read_timing.py','prepare_registered_read_native.py','review_native_registered_read.py']


def read(p):return json.loads(p.read_text())


def startup_review(d,out):
    p=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()));verify(d/'inputs',p['inputs'])
    stage=Path(p['stage']);m=read(stage/'r-front-control-native.json');verify(stage,m['source_hashes'])
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    before=Path(p['baseline_run']);b=json.loads(gzip.decompress((before/'provenance.json.gz').read_bytes()));verify(before/'inputs',b['inputs'])
    extra=(MONITOR+memc_monitor(m)+operand_monitor(m)+'\n').encode()
    for n,h in m['source_hashes'].items():
        raw=(d/'inputs'/n).read_bytes()
        if n=='harness/tb_native_cedar.sv':
            assert raw.count(extra)==1;raw=raw.replace(extra,b'',1)
            assert raw==(before/'inputs'/n).read_bytes()
        assert hashlib.sha256(raw).hexdigest()==h,n
    assert p['host_clock_hz']==66666667 and p['normal_baseboard_boot_presses']==3
    assert int((d/'host-cycle-limit.txt').read_text())==p['host_limit']==300000000
    assert sha(d/'inputs/harness/host.cpp')==b['inputs']['harness/host.cpp']
    s,row=observed(d)
    assert all(a['returncode']==0 for a in s['actions']if a['stage']!='test')
    assert '-DDORADO_HOST_CLOCK_HZ=66666667'in next(a['command']for a in s['actions']if a['stage']=='compile')
    if s['status']=='RUNNING':assert row['verified_live']
    else:assert s['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'
    retain(d,out/d.name);return row


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    def keep(d):retain(d,out/d.name)
    epoch=BUILD/'native-operand-view-epoch-proof-v2-20261025';e=proof_check(epoch,'PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW')
    assert e['host_clock_varies_independently']and e['native_constant_inputs']=={'advance':0,'pair_fall':0}
    assert e['compared_physical_view_bits']==844;keep(epoch)
    proof=BUILD/'native-operand-buffer-proof-v3-20261025';p=proof_check(proof,'PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW')
    assert p['epoch_proof_state_sha256']==sha(epoch/'run-state.json')and p['epoch_host_clock_independence_proved']
    assert p['helper_sha256']==sha(ROOT/HELPER)and p['tag_support_bits']==909 and p['buffered_physical_bits']==88
    assert {a['label']for a in p['actions']}=={'contracts','ignore-held-owner-tag','wrong-branch-flag'};keep(proof)
    support=BUILD/'native-operand-view-support-v1-20261025';s=read(support/'run-state.json');verify(support,s['files'])
    assert s['mask_hex']==p['tag_support_hex']and s['support_bits']==909
    assert ((int(s['mask_hex'],16)>>420)&((1<<844)-1))==((1<<844)-1);keep(support)
    selection=BUILD/'native-operand-tag-mask-selection-v1-20261025';sel=read(selection/'results.json')
    assert sel['after_sha256']==sha(ROOT/HELPER)==sha(selection/'after.sv')and sha(selection/'before.sv')==sel['before_sha256'];keep(selection)
    for name in ['native-operand-view-epoch-proof-v1-20261025','native-operand-buffer-proof-v1-20261025','native-operand-buffer-proof-v2-20261025',
                 'native-operand-buffer-xz-v1-20261025','native-operand-buffer-xz-negative-v1-20261025',
                 'native-operand-buffer-full-verilator-v1-20261025','native-operand-buffer-words-verilator-v1-20261025']:
        keep(BUILD/name)
    for negative in [False,True]:
        d=BUILD/('native-operand-buffer-xz-negative-v2-20261025'if negative else'native-operand-buffer-xz-v2-20261025');s=checked(d)
        assert s['status']==('PASS_NATIVE_OPERAND_UNKNOWN_BYPASS_COUNTEREXAMPLE'if negative else'PASS_NATIVE_OPERAND_UNKNOWN_FALLBACK')
        assert s['epoch_state_sha256']==sha(epoch/'run-state.json')and s['helper_sha256']==p['helper_sha256']
        if not negative:assert 'checks=11895 unknown=11892 outputs=2969 physical=844'in(d/'test.log').read_text()
        else:assert s['actions'][0]['returncode']==0 and s['returncode']!=0 and 'OPERAND_XZ prepared view differs'in(d/'test.log').read_text()
        keep(d)
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-operand-buffer-{mode}-{engine}-v2-20261025';s,row=observed(d)
            assert s['operand_proof_state_sha256']==sha(proof/'run-state.json')and s['original_assertions_and_reference_unchanged']
            assert s['native_cells_j19_single_SRAM_and_commit_barriers_retained']and sha(d/'adapters/dorado_native_operand_buffer.sv')==p['helper_sha256']
            if s['status']=='RUNNING':assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:
                assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'
                assert len(re.findall(r'^PACKET_CLOCK channel=',(d/'test.log').read_text(),re.M))==100
                assert s['operand_prepare_edges']==(2700 if mode=='full'else 43)
            keep(d);replays.append(row)
    keep(BUILD/'native-operand-buffer-review-v1-20261025')
    timing=BUILD/'native-memc-read-buffer-full15-timing-review-v1-20261025';t=read(timing/'results.json')
    assert t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and t['registered_MemC_tag_reads']and t['worst_setup_path']['slack_ns']==-10.848
    c=read(timing/'terminal/collection.json');verify(timing/'terminal',c['collected_hashes']);assert sha(timing/'terminal.tgz')==t['bundle_sha256']
    assert len(c['source_readback'])==577 and len(c['collected_hashes'])==327 and len(t['setup_paths_by_corner'])==4
    assert len(t['terminal_state']['stages'])==9 and all(a['returncode']==0 for a in t['terminal_state']['stages']);keep(timing)
    stage=BUILD/'native-operand-buffer-full15-stage-v1-20261025';m=read(stage/'r-front-control-native.json');verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);pm=read(parent/'r-front-control-native.json');verify(parent,pm['source_hashes'])
    assert c['source_readback']==pm['source_hashes']and e['source_control_sha256']==sha(parent/CONTROL)
    assert m['operand_buffer_proof_state_sha256']==sha(proof/'run-state.json')and m['operand_view_tag_support_bits']==909
    assert len(m['source_hashes'])==578 and m['actual_native_physical_view_uses_prepared_data']
    assert m['frequency_hz']==66666667 and m['sysper']==4 and not m['assembly_permitted']
    bp=m['native_backplane_binding_source'];changed={CONTROL:control((parent/CONTROL).read_text()).encode(),CALLER:caller((parent/CALLER).read_text()).encode(),
        bp:backplane((parent/bp).read_text()).encode(),'src/'+HELPER:(ROOT/HELPER).read_bytes()}
    assert sorted(changed)==m['incremental_functional_HDL_changes']
    for n in m['source_hashes']:
        raw=changed[n]if n in changed else(parent/n).read_bytes()
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:raw=raw.replace(pm['project'].encode(),m['project'].encode())
        if n=='core/Dorado.qsf':raw+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(Path(m['project'])/('src/'+HELPER))+'\n').encode()
        assert(stage/n).read_bytes()==raw,n
    keep(stage)
    lint=BUILD/'native-operand-buffer-full15-lint-v1-20261025';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['source_files']==200 and l['stage_clock_macro_checked_against_QSF']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and '-DDORADO_HOST_CLOCK_HZ=66666667'in l['command']
    warnings=len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M));assert warnings==6;keep(lint)
    launch=BUILD/'native-operand-buffer-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['builder_pid']==290188 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')and la['verified_source_count']==578;keep(launch)
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path('/home/alans/mister/Dorado_CPRead15_PreparedOperand_20261025');s=json.loads((p/'run-state.json').read_text());m=json.loads((p/'r-front-control-native.json').read_text())
assert s['builder_pid']==290188 and not s['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']}
assert actual==m['source_hashes']and len(actual)==578
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
row=dict(observed=time.time(),state=s,source_readback=actual)
if s['status']=='RUNNING':
 q=subprocess.run(['ps','-p',str(s['stages'][-1]['pid']),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
 assert q.returncode==0 and 'quartus_'in q.stdout;row.update(verified_live=True,process=q.stdout.strip())
else:assert s['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
print(json.dumps(row))
'''
    (out/'same-prepared-operand-query.py').write_text(query)
    r=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-prepared-operand-query.stderr').write_text(r.stderr);assert r.returncode==0,r.stderr
    remote=json.loads(r.stdout);assert remote['source_readback']==m['source_hashes']
    (out/'same-prepared-operand-observation.json').write_text(json.dumps(remote,indent=2)+'\n')
    startups=[startup_review(BUILD/n,out)for n in ['native-registered-read-startup-v1-20261025','native-memc-registered-read-startup-v1-20261025','native-prepared-operand-startup-v1-20261025']]
    native=BUILD/'native-cp-read-j19-boot-v1-20261025';n=read(native/'run-state.json');assert int((native/'host-cycle-limit.txt').read_text())==5000000000
    if n['status']=='RUNNING':
        assert n['actions'][-1]['pid']==62048
        ps=subprocess.run(['ps','-p','62048','-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
        assert ps.returncode==0 and str(native/'obj/native-cedar')in ps.stdout
        native_observation=dict(status='VERIFIED_LIVE_SAME_5B_NATIVE_BOOT',pid=62048,process=ps.stdout.strip(),observed=time.time())
    else:native_observation=n
    medium=BUILD/'cedar-work-native-hints.pack'
    assert medium.stat().st_size==63578624 and sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    dest=out/'current-RTL';dest.mkdir();shutil.copy2(ROOT/HELPER,dest/Path(HELPER).name)
    result=dict(status='REVIEWED_NATIVE_PRE_EVENT_OPERAND_BUFFER',scope=__doc__,epoch_compared_physical_bits=844,
        saved_operand_bits=88,logical_tag_bits=1264,proved_tag_support_bits=909,all844_closed_owner_bits_retained_in_tag=True,
        assumptions=0,host_clock_independence_proved=True,earlier_epoch_v1_scope='Host-clock0 cofactor only; superseded by independent-host-clock v2',
        four_state_input_bits=1982,four_state_checks=11895,all_original_raw_outputs_compared=2969,
        replays=replays,pending_verified_live_Icarus=pending,completed_MemC_timing=t,
        native_stage_inputs=578,elaborated_sources=200,structural_warnings=warnings,same_prepared_operand_fit=remote,
        fixed300M_original_ROM_startups=startups,original_same5B_boot=native_observation,
        nominal_diagnostic_ordinary_ns=120,target_ordinary_ns=60,physical_timing_or_firmware_or60ns_qualified=False,
        new_hardware_speed_gain_measured=False,hardware_or_media_changed=False,full_goal_active=True,
        next_action='Follow SAME prepared-operand290188, Icarus peers and immutable native limits. Use measured paths to finish original overlapping phases and real MIR/Ready/Hold capture, then peripheral/SDRAM timing and Cedar/input/Rollback qualification.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-operand-view',extra_tools=TOOLS)
    print(r['status'],flush=True)
