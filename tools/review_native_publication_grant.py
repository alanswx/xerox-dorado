#!/usr/bin/env python3
"""Retain the terminal compact fit and source-proved shared-grant milestone.

Running jobs are observed by their real process handles, never credited as
passes. No assembly, reload, reset, firmware or protected-media change occurs.
"""
import argparse
import json
from pathlib import Path
import subprocess
import time

from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'stage_native_publication_grant.py','prove_native_publication_grant.py',
       'replay_native_publication_grant.py','audit_native_publication_grant.py',
       'start_native_phase_guard_timing.py','review_native_cp_read_timing.py','collect_native_clock_enable_fit.py']


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'native-publication-grant-stage-v2-20261025'
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    dirs=[stage,BUILD/'native-publication-grant-stage-v1-20261025']
    rejection=BUILD/'native-compact-operand-routing-review-v1-20261025'
    r=json.loads((rejection/'results.json').read_text());dirs.append(rejection)
    assert r['status']=='COLLECTED_TERMINAL_NATIVE_CLOCK_ENABLE_ROUTING_REJECTION'
    assert r['terminal_state']['builder_pid']==743068 and r['source_files_verified']==578
    proofs=[]
    for version in [1,2]:
        d=BUILD/f'native-publication-grant-proof-v{version}-20261025'
        p=json.loads((d/'run-state.json').read_text());verify(d,p['files'])
        assert p['status']=='PASS_NATIVE_SHARED_PUBLICATION_GRANT'
        assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and p['assumptions']==0
        proofs.append(dict(version=version,state_sha256=sha(d/'run-state.json'),native_four_state=p['four_state_native_cases'],generic_four_state=p['four_state_generic_cases']))
        dirs.append(d)
    lint=BUILD/'native-publication-grant-lint-v1-20261025';dirs.append(lint)
    l=json.loads((lint/'run-state.json').read_text());verify(lint,l['files'])
    assert l['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'
    assert all(a['returncode']==0 for a in l['actions'])and len(l['actions'])==2
    audit=BUILD/'native-publication-grant-audit-v1-20261025';dirs.append(audit)
    a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_SHARED_GRANT_STRUCTURAL_AUDIT'
    assert a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and a['instances']==12782 and a['sensitive_processes']==3798
    replays=[]
    for engine in ['icarus','verilator']:
        for mode in ['full','words']:
            d=BUILD/f'native-publication-grant-{mode}-{engine}-v1-20261025';dirs.append(d)
            s=json.loads((d/'run-state.json').read_text());verify(d,s['files'])
            assert s['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
            obs=dict(engine=engine,mode=mode,status=s['status'],state_sha256=sha(d/'run-state.json'))
            if s['status']=='RUNNING':
                action=s['actions'][-1]
                ps=subprocess.run(['ps','-p',str(action['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True,capture_output=True)
                assert ps.returncode==0 and str(d)in ps.stdout,(d,ps.stdout,ps.stderr)
                obs.update(pid=action['pid'],process=ps.stdout.strip(),verified_live_at=time.time())
            else:
                assert s['status']=='PASS_SHARED_GRANT_COMPLETE_NATIVE_CALLER'
                assert s['original_results_match']and s['preparation_coverage_match']
                obs.update(outcomes=s['outcomes'])
            replays.append(obs)
    launch=BUILD/'native-publication-grant-launch-v1-20261025';dirs.append(launch)
    launch_state=json.loads((launch/'launch-observation.json').read_text())
    assert launch_state['builder_pid']==860426 and launch_state['manifest_sha256']==sha(stage/'r-front-control-native.json')
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert len(m['source_hashes'])==578 and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==860426 and not s['assembly_permitted']
processes=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
  processes.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(v['returncode']==0 for v in processes)
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
print(json.dumps({'observed':time.time(),'run_state':s,'processes':processes,'source_files_verified':578}))
'''.replace('PROJECT',repr(m['project']))
    (out/'same-shared-grant-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-shared-grant-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    fit=json.loads(remote.stdout)
    (out/'same-shared-grant-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    # Preserve the older fixed-limit run as an early startup observation only.
    early=BUILD/'native-prepared-operand-startup-v1-20261025'
    e=json.loads((early/'run-state.json').read_text())
    assert e['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC'and e['reason']=='HOST_LIMIT'and not e['firmware_qualified']
    for d in dirs:retain(d,out/'retained'/d.name)
    for name in ['run-state.json','test.log']:
        q=out/'early-startup-observation'/name;q.parent.mkdir(exist_ok=True);q.write_bytes((early/name).read_bytes())
    result=dict(status='REVIEWED_NATIVE_SHARED_PUBLICATION_GRANT_MILESTONE',
                previous_goal_turn='progress: observed compact fit terminal routing failure; next structural action now selected',
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),compact_fit_terminal=r,
                proof_receipts=proofs,actual_native_instances=a['instances'],sequential_processes=a['sensitive_processes'],
                one_proved_owner_guard_change=True,standalone_independent_ready_barrier_retained=True,
                full_program_replays=replays,same_new_timing_fit=fit,
                early_startup_terminal={'pid':30666,'bound_host_edges':300000000,'reason':'HOST_LIMIT','guest_edges':52022494,'firmware_qualified':False},
                nominal_ordinary_ns=120,target_ordinary_ns=60,
                hardware_speed_gain_measured=False,FPGA_or_protected_media_changed=False,full_goal_active=True,
                next_action='Observe SAME860426 and Icarus48406/48963; collect terminal reports without restart. Review physical routing and all timing before any assembly. Finish actual60ns phases/SDRAM/timers and pinned hardware/Cedar/Rollback/persistence/three-boot/stress gates; complete snapshots deferred.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-shared-publication-grant',extra_tools=TOOLS)
    print(r['status'],flush=True)
