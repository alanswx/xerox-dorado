#!/usr/bin/env python3
"""Retain the source-bound native clock-enable conversion and its current gates.

The whole-core fit remains diagnostic until all terminal timing and functional
checks pass. Preserve earlier failed tooling attempts and the older Icarus
coverage rejection; do not promote a successful compiler/test exit alone.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['elaborate_native_clock_enable_audit.py','audit_native_clock_enable_sinks.py',
 'stage_native_clock_enables.py','lint_native_clock_enables.py','audit_native_clock_enable_conversion.py',
 'test_native_clock_enable_baseboard.py','test_native_clock_enable_storage.py',
 'start_native_phase_guard_timing.py','replay_native_operand_buffer.py',Path(__file__).name]


def read(p):return json.loads(p.read_text())


def checked(directory,status):
    s=read(directory/'run-state.json');assert s['status']==status,(directory,s['status'])
    verify(directory,s['files']);return s


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'native-clock-enable-stage-v4-20261025';m=read(stage/'r-front-control-native.json');verify(stage,m['source_hashes'])
    assert m['clock_enable_conversion_staged']and not m['assembly_permitted']and len(m['source_hashes'])==578
    inventory=BUILD/'native-clock-enable-sinks-v2-20261025';i=read(inventory/'results.json')
    assert i['process_class_counts']=={'VIRTUAL_CLOCK_ENABLE_REQUIRED':1823,'HOST_CLOCK_RETAINS_EXISTING_POLICY':1975}
    assert not i['ambiguous_clock_port_bindings']and all(u['domain']=='HOST'for u in i['clock_as_data_uses'])
    lint=BUILD/'native-clock-enable-lint-v5-20261025';l=checked(lint,'ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT')
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and len(l['actions'])==2
    assert all(a['returncode']==0 for a in l['actions'])
    audit=BUILD/'native-clock-enable-conversion-audit-v4-20261025';a=read(audit/'results.json')
    assert a['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['process_counts']['removed_guards']==1823 and a['combinational_loop_warnings']==6
    bb=BUILD/'native-clock-enable-baseboard-v2-20261025';b=checked(bb,'PASS_ORIGINAL_ROM_BASEBOARD_COMMON_CLOCK_EQUIVALENCE')
    negative=BUILD/'native-clock-enable-baseboard-negative-v1-20261025';n=checked(negative,'PASS_MISSING_CLOCK_ENABLE_COUNTEREXAMPLE')
    for s in [b,n]:
        tested=Path(s['stage']);tm=read(tested/'r-front-control-native.json');verify(tested,tm['source_hashes'])
        assert s['stage_manifest_sha256']==sha(tested/'r-front-control-native.json')
        assert {p:h for p,h in tm['source_hashes'].items()if Path(p).suffix in ['.v','.sv']}=={p:h for p,h in m['source_hashes'].items()if Path(p).suffix in ['.v','.sv']}
    bb_line=re.findall(r'^CLOCK_ENABLE_BASEBOARD_PASS .*$',(bb/'test.log').read_text(),re.M);assert len(bb_line)==1
    assert 'host=465811 active=349348 paused=116463 state_vectors=208 outputs=69 RAM_arrays=37'in bb_line[0]
    assert 'BB_RESET_MIDAS_RETURN'in(bb/'test.log').read_text()and 'dex=256'in(bb/'test.log').read_text()
    storage=BUILD/'native-clock-enable-storage-v2-20261025';s=checked(storage,'PASS_ACTUAL_NATIVE_COMMON_CLOCK_STORAGE_AND_ADDRESS')
    assert s['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and len(s['cases'])==8 and all(c['passed']for c in s['cases'])
    assert all('checks=40000 steps=3531 reads=1591 writes=359 paused_responses=1948 reset_cycles=14'in c['metrics'][0]for c in s['cases'])
    timing=BUILD/'native-operand-buffer-full15-timing-review-v1-20261025';t=read(timing/'results.json')
    assert t['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and t['source_files_verified']==578
    assert t['worst_setup_path']['slack_ns']==-10.242 and len(t['terminal_state']['stages'])==9
    assert all(v['returncode']==0 for v in t['terminal_state']['stages'])and not t['assembly_permitted']
    # Observe the completed old test without restarting it or rewriting its
    # run-state. Its driver stopped on a post-run coverage assertion.
    replay=BUILD/'native-operand-buffer-full-icarus-v2-20261025';r=read(replay/'run-state.json')
    last=r['actions'][-1];assert last['label']=='test'and last['returncode']==0
    log=(replay/'test.log').read_text();parent=Path(r['source'])
    assert outcomes(log)==outcomes((parent/'test.log').read_text())
    assert 'OPERAND_BUFFER_PREPARATION edges=0'in log
    gap=dict(status='TERMINAL_ORIGINAL_RESULTS_MATCH_KNOWN_PREPARATION_COVERAGE_MISSING',
        saved_driver_status=r['status'],test_pid=last['pid'],test_returncode=0,
        test_log_sha256=sha(replay/'test.log'),known_preparation_edges=0,
        passed=False,original_assertions_or_coverage_requirements_waived=False)
    (out/'older-full-Icarus-observation.json').write_text(json.dumps(gap,indent=2)+'\n')
    words=checked(BUILD/'native-operand-buffer-words-icarus-v2-20261025','PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER')
    assert words['operand_prepare_edges']>0
    launch=BUILD/'native-clock-enable-full15-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['manifest_sha256']==sha(stage/'r-front-control-native.json')and la['builder_pid']==458549
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT)
m=json.loads((p/'r-front-control-native.json').read_text())
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in m['source_hashes']}
assert actual==m['source_hashes']and len(actual)==578
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==458549 and not s['assembly_permitted']
ps=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
  ps.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(r['returncode']==0 for r in ps)
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps,'source_files_verified':len(actual)}))
'''.replace('PROJECT',repr(m['project']))
    (out/'same-clock-enable-fit-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-clock-enable-fit-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    fit=json.loads(remote.stdout);(out/'same-clock-enable-fit-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    observations=[]
    for name in ['native-registered-read-startup-v1-20261025','native-memc-registered-read-startup-v1-20261025','native-prepared-operand-startup-v1-20261025','native-cp-read-j19-boot-v1-20261025']:
        d=BUILD/name;state=read(d/'run-state.json');last=state['actions'][-1]
        row=dict(run=name,status=state['status'],last_pid=last['pid'],last_returncode=last.get('returncode'))
        if state['status']=='RUNNING':
            ps=subprocess.run(['ps','-p',str(last['pid']),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
            assert ps.returncode==0 and str(d/'obj/native-cedar')in ps.stdout
            row.update(process=ps.stdout.strip(),verified_live=True,observed=time.time())
        for f in ['run-state.json','host-cycle-limit.txt','test.log']:
            if(d/f).is_file():
                target=out/'native-observations'/name/f;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(d/f,target)
        observations.append(row)
    medium=BUILD/'cedar-work-native-hints.pack';assert medium.stat().st_size==63578624
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    dirs=[BUILD/name for name in [
      'native-clock-enable-audit-xml-v1-20261025','native-clock-enable-sinks-v1-20261025','native-clock-enable-sinks-v2-20261025',
      *['native-clock-enable-stage-v'+str(v)+'-20261025'for v in range(1,5)],
      *['native-clock-enable-lint-v'+str(v)+'-20261025'for v in range(1,6)],
      *['native-clock-enable-conversion-audit-v'+str(v)+'-20261025'for v in range(1,5)],
      'native-clock-enable-baseboard-v1-20261025','native-clock-enable-baseboard-v2-20261025',
      'native-clock-enable-baseboard-negative-v1-20261025','native-clock-enable-storage-v1-20261025',
      'native-clock-enable-storage-v2-20261025','native-clock-enable-full15-launch-v1-20261025',
      'native-operand-buffer-full15-timing-review-v1-20261025']]
    for d in dirs:assert d.exists();retain(d,out/'retained'/d.name)
    result=dict(status='REVIEWED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT',scope=__doc__,
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),converted_module_types=33,
        actual_native_instances=a['actual_instances'],guest_processes_guarded=1823,host_processes_unchanged=1975,
        original_transfer_ASTs_recovered=True,source_processes=58,BaseBoard_original_ROM_result=bb_line[0],
        missing_enable_negative_passed=True,storage_cases=s['cases'],structural_warning_count=6,
        completed_prepared_operand_timing_corners={k:min(p['slack_ns']for p in v)for k,v in t['setup_paths_by_corner'].items()},
        completed_prepared_operand_worst=t['worst_setup_path'],same_clock_enable_fit=fit,
        older_full_Icarus_coverage_gap=gap,original_native_observations=observations,
        retained_tooling_failures=[
          'Initial inventory used positional pin labels instead of formal names; corrected inventory resolves all six host clock processes.',
          'First variant stage removed original modules referenced by unused wrappers; keep originals and append variants.',
          'Second variant stage appended after vendor macro undef; insert before cleanup.',
          'First BaseBoard test omitted generated scope in cross-references; exact scaled_crystal path restored.',
          'AST reader compared synthetic combinational alias hashes and baked diagnostic line numbers; scope edge-sensitive processes and normalize diagnostic location only.',
          'First storage fixture selected an older DEFER_READS API; corrected fixture uses actual native machine_step/after-address interface.'
        ],nominal_ordinary_ns=120,target_ordinary_ns=60,
        full_machine_equivalence_or_firmware_or_physical_timing_qualified=False,
        measured_hardware_speed_gain=False,FPGA_or_protected_media_changed=False,full_goal_active=True,
        next_action='Observe SAME clock-enable458549 through all terminal timing reports. Finish actual peripheral/full-native equivalence and old Icarus known-preparation coverage. Then chronological producer splitting, original60ns phases and full hardware qualification.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-clock-enable-conversion',extra_tools=TOOLS)
    print(r['status'],flush=True)
