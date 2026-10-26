#!/usr/bin/env python3
"""Retain the RIOT RAM integration milestone and the preceding routing failure.

Component induction, all chip states, original-ROM whole-board checks and
actual native ancestry are qualified separately. The new native fit is still
diagnostic. Never infer Cedar boot,60ns cadence or speed from component gates.
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

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'test_riot_block_ram.py','map_riot_block_ram.py','prepare_riot_blockram.py',
       'test_riot_blockram_component.py','test_riot_blockram_baseboard.py','prove_riot_block_ram.py',
       'stage_native_riot_blockram.py','audit_native_riot_blockram.py','lint_native_clock_enables.py',
       'start_native_phase_guard_timing.py','review_native_cp_read_timing.py','collect_native_clock_enable_fit.py']


def review(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[]
    stage=BUILD/'native-riot-blockram-stage-v1-20261025'
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert len(m['source_hashes'])==580 and m['native_RIOT_block_ram_staged']and not m['assembly_permitted']
    dirs.append(stage)
    terminal=BUILD/'native-publication-grant-routing-review-v1-20261025'
    rejection=json.loads((terminal/'results.json').read_text());dirs.append(terminal)
    assert rejection['status']=='COLLECTED_TERMINAL_NATIVE_CLOCK_ENABLE_ROUTING_REJECTION'
    assert rejection['terminal_state']['builder_pid']==860426 and rejection['source_files_verified']==578
    replays=[]
    for engine in ['icarus','verilator']:
        for mode in ['full','words']:
            d=BUILD/f'native-publication-grant-{mode}-{engine}-v1-20261025'
            r=json.loads((d/'run-state.json').read_text());verify(d,r['files'])
            assert r['status']=='PASS_SHARED_GRANT_COMPLETE_NATIVE_CALLER'and r['original_results_match']and r['preparation_coverage_match']
            for action in r['actions']:assert action['returncode']==0 and sha(d/(action['label']+'.log'))==action['log_sha256']
            replays.append(dict(engine=engine,mode=mode,state_sha256=sha(d/'run-state.json')));dirs.append(d)
    gates={}
    for name,status in [('riot-blockram-tests-v1-20261025','PASS_RESET_MASKED_RIOT_BLOCK_RAM_COMPONENT'),
                        ('riot-blockram-proof-v2-20261025','PASS_RESET_HISTORY_RIOT_RAM_INDUCTION'),
                        ('riot-blockram-component-tests-v1-20261025','PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT'),
                        ('riot-blockram-baseboard-v2-20261025','PASS_ORIGINAL_ROM_BASEBOARD_RIOT_BLOCK_RAM')]:
        d=BUILD/name;r=json.loads((d/'run-state.json').read_text());verify(d,r['files']);assert r['status']==status
        gates[name]=dict(state_sha256=sha(d/'run-state.json'),status=status);dirs.append(d)
    base=BUILD/'riot-blockram-baseboard-v2-20261025';b=json.loads((base/'run-state.json').read_text())
    assert b['metrics']==['CLOCK_ENABLE_BASEBOARD_PASS host=465811 active=349348 paused=116463 state_vectors=208 outputs=69 RAM_arrays=37']
    for action in b['actions']:assert action['returncode']==0 and sha(base/(action['label']+'.log'))==action['log_sha256']
    mapping=BUILD/'riot-blockram-map-review-v1-20261025';ma=json.loads((mapping/'results.json').read_text());verify(mapping,ma['files'])
    assert ma['status']=='COLLECTED_RIOT_RAM_MAPPING'and ma['state']['returncode']==0
    log=(mapping/'map.log').read_text()
    assert 'Parameter RAM_BLOCK_TYPE set to M10K'in log and 'Parameter READ_DURING_WRITE_MODE_MIXED_PORTS set to OLD_DATA'in log
    assert 'Total registers : 129'in ma['map_summary']and'Total block memory bits : 1,024'in ma['map_summary']
    lint=BUILD/'native-riot-blockram-lint-v1-20261025';l=json.loads((lint/'run-state.json').read_text());verify(lint,l['files'])
    assert l['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'and l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    audit=BUILD/'native-riot-blockram-audit-v1-20261025';a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_RIOT_BLOCK_RAM_STRUCTURAL_AUDIT'and a['instances_after']==12787 and a['process_counts']['new_processes']==3813
    assert a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and a['all_new_processes_same_clock_and_advance']
    dirs.extend([mapping,lint,audit,BUILD/'riot-blockram-map-launch-v1-20261025',BUILD/'riot-blockram-component-v1-20261025',
                 BUILD/'riot-blockram-proof-v1-20261025',BUILD/'riot-blockram-baseboard-v1-20261025'])
    launch=BUILD/'native-riot-blockram-launch-v1-20261025';la=json.loads((launch/'launch-observation.json').read_text());dirs.append(launch)
    assert la['builder_pid']==1029546 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert la['verified_source_count']==580 and la['clock_enable_diagnostic_preflight']['RIOT_block_ram_receipts']==m['RIOT_block_ram_receipts']
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert len(m['source_hashes'])==580 and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==1029546 and not s['assembly_permitted']
ps=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
  ps.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(v['returncode']==0 for v in ps)
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps,'source_files_verified':580}))
'''.replace('PROJECT',repr(m['project']))
    (out/'same-RIOT-fit-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-RIOT-fit-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    fit=json.loads(remote.stdout);(out/'same-RIOT-fit-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    boot=BUILD/'native-j19-receive-analysis-v1-20261025';br=json.loads((boot/'results.json').read_text());verify(boot,br['files'])
    assert br['pid']==62048 and br['whole_packet_CRC_and_zero_EOP_match']and not br['first_packet_rejection_reason_established']
    dirs.append(boot)
    medium=BUILD/'cedar-work-native-hints.pack'
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    for d in dirs:retain(d,out/'retained'/d.name)
    result=dict(status='REVIEWED_NATIVE_RIOT_BLOCK_RAM_INTEGRATION',previous_goal_turn='progress: proved/staged shared grant, launched fit and pushed milestone',
                preceding_grant_fit=rejection,all_four_grant_replays=replays,RIOT_gates=gates,
                isolated_RAM_mapping=dict(registers=129,memory_bits=1024,type='M10K',read_during_write='OLD_DATA'),
                whole_BaseBoard_metrics=b['metrics'],native_stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                actual_native_instances=12787,actual_native_sequential_processes=3813,same_new_native_fit=fit,
                newer_boot_observation=dict(pid=62048,host_ticks=br['host_ticks'],guest_ticks=br['guest_ticks'],
                                           last_reply_CRC_and_EOP_match=True,sequence_number=br['registers']['SEQNO']['value'],
                                           first_packet_rejection_reason_established=False,firmware_qualified=False),
                proof_scope='All1024 logical RAM bits after an accepted reset and registered reads with reset history; inputs/backing arbitrary, no assumptions. Four-state tests cover pre-reset X/unknown controls.',
                nominal_ordinary_ns=120,target_ordinary_ns=60,physical_timing_or_hardware_speed_or_Cedar_qualified=False,
                FPGA_or_protected_media_changed=False,full_goal_active=True,
                next_action='Observe SAME1029546 through terminal reports; verify all five actual RAM mappings, area and all-corner/crossing/SDRAM timing. Continue original frozen native38212/62048 without changes. Then original60ns phases, calibrated timers/SDRAM, pinned hardware gains and original Cedar/Rollback/persistence/three-boot/stress gates; snapshots deferred.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-riot-blockram-integration',extra_tools=TOOLS)
    print(r['status'],flush=True)
