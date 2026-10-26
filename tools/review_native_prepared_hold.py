#!/usr/bin/env python3
"""Retain qualified prepared-Hold candidate and observe its exact live jobs.

Source function/buffer induction, complete publication and host-prefix checks
remain separate from live timing/firmware qualification and measured speed.
"""
import argparse
import gzip
import json
from pathlib import Path
import subprocess
import time

from prepare_common_clock_native_startup import observer,retime,hold_monitor
from prepare_registered_read_native import MONITOR,memc_monitor,operand_monitor
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['resolve_native_hold_owners.py','prepare_native_hold_function.py','qualify_native_hold_buffer.py',
       'test_native_hold_buffer.py','stage_native_hold_buffer.py','qualify_native_hold_publication.py',
       'prove_native_hold_prefix_independence.py','audit_native_hold_buffer.py','start_native_prepared_hold_timing.py',
       'start_native_phase_guard_timing.py','prepare_common_clock_native_startup.py',Path(__file__).name]


def review(out):
    assert not out.exists()
    stage=BUILD/'native-prepared-hold-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert len(m['source_hashes'])==581 and m['prepared_MemC_Hold_staged']and not m['assembly_permitted']
    function=BUILD/'native-hold-function-v1-20261025';f=json.loads((function/'results.json').read_text());verify(function,f['files'])
    assert f['context_bits']==132 and f['buffer_sha256']==sha(stage/'src/verilog/rtl/dorado_memc_hold_buffer.sv')
    runs={name:BUILD/d for name,d in dict(proof='native-hold-buffer-proof-v1-20261025',tests='native-hold-buffer-tests-v2-20261025',
        publication='native-hold-publication-v2-20261025',prefix='native-hold-prefix-proof-v1-20261025').items()}
    states={n:json.loads((d/'run-state.json').read_text())for n,d in runs.items()}
    for n,status in dict(proof='PASS_SOURCE_BOUND_NATIVE_HOLD_BUFFER_INDUCTION',tests='PASS_DUAL_ENGINE_SOURCE_HOLD_BUFFER_AND_NEGATIVES',
                         publication='PASS_COMPLETE_NATIVE_PREPARED_HOLD_PUBLICATION',prefix='PASS_NATIVE_HOST_PREFIX_AFTER_HOLD_INDEPENDENCE').items():
        r=states[n];assert r['status']==status and all(a['passed']for a in r['actions'])
        for a in r['actions']:
            if n=='tests':continue
            verify(runs[n]/a['label'],a['files']);assert sha(runs[n]/a['label']/'proof.log')==a['log_sha256']
    assert states['publication']['compared_public_outputs']==2970 and states['publication']['assumptions']==0
    assert states['publication']['source_bound_buffer_induction_sha256']==sha(runs['proof']/'run-state.json')
    assert states['prefix']['compared_prefix_bits']==269 and states['prefix']['assumptions']==0
    audit=BUILD/'native-prepared-hold-audit-v1-20261025';a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_PREPARED_HOLD_STRUCTURAL_AUDIT'and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['unchanged_original_sequential_processes']==3813 and a['added_host_preparation_processes']==1 and a['instances_after']==12788
    startup=BUILD/'native-prepared-hold-startup-v1-20261025';p=json.loads(gzip.decompress((startup/'provenance.json.gz').read_bytes()));verify(startup/'inputs',p['inputs'])
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and p['native_prepared_MemC_Hold_connected']
    assert p['host_limit']==1500000000 and(startup/'host-cycle-limit.txt').read_text()=='1500000000\n'
    assert p['normal_baseboard_boot_presses']==3 and len(p['pause_checked_scalar_vectors'])==2810
    original=(Path(p['baseline_run'])/'inputs/harness/tb_native_cedar.sv').read_text();raw,changes=retime(original)
    extra,signals,arrays=observer(BUILD/'native-clock-enable-sinks-v2-20261025',BUILD/'native-clock-enable-audit-xml-v1-20261025',BUILD/'native-prepared-hold-lint-v1-20261025')
    assert raw.replace('endmodule',MONITOR+memc_monitor(m)+operand_monitor(m)+extra+hold_monitor(m)+'\nendmodule',1)==(startup/'inputs/harness/tb_native_cedar.sv').read_text()
    assert changes==[tuple(v)for v in p['harness_reversible_retiming_edits']]and signals==p['pause_checked_scalar_vectors']and arrays==p['array_scope_not_in_scalar_image']
    runtime=json.loads((startup/'run-state.json').read_text());assert runtime['status']=='RUNNING'
    assert all(v['returncode']==0 for v in runtime['actions'][:-1])and runtime['actions'][-1]['stage']=='test'
    pid=runtime['actions'][-1]['pid'];ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,stat=,command='],capture_output=True,text=True)
    assert ps.returncode==0 and str(startup/'obj/native-cedar')in ps.stdout
    log=(startup/'test.log').read_text();assert'NATIVE_INPUT_RESET_PASS'in log and'NATIVE_HPS_ACK host=207944'in log
    assert not any(v in log for v in ['%Error','Assertion failed','NATIVE_HOST_ERROR'])
    launch=BUILD/'native-prepared-hold-launch-v1-20261025';la=json.loads((launch/'launch-observation.json').read_text())
    assert la['manifest_sha256']==sha(stage/'r-front-control-native.json')and la['verified_source_count']==581
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
r=json.loads((p/'run-state.json').read_text());assert r['builder_pid']==BUILDER and not r['assembly_permitted']
processes=[]
if r['status']=='RUNNING':
 for pid in [r['builder_pid'],r['stages'][-1]['pid']]:
  s=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,stat=,args='],capture_output=True,text=True)
  processes.append({'pid':pid,'returncode':s.returncode,'process':s.stdout.strip()})
 assert any(s['returncode']==0 for s in processes)
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
print(json.dumps({'observed':time.time(),'source_files_verified':len(m['source_hashes']),'state':r,'processes':processes}))
'''.replace('PROJECT',repr(m['project'])).replace('BUILDER',str(la['builder_pid']))
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    assert remote.returncode==0,remote.stderr;fit=json.loads(remote.stdout)
    out.mkdir(parents=True);(out/'same-fit-query.py').write_text(query);(out/'same-fit-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    dirs=[stage,function,audit,BUILD/'native-hold-owners-v1-20261025',*runs.values(),launch,
          BUILD/'native-prepared-hold-lint-v1-20261025',BUILD/'native-hold-publication-v1-20261025',
          BUILD/'native-hold-buffer-tests-v1-20261025',startup]
    for d in dirs:retain(d,out/'retained'/d.name)
    result=dict(status='REVIEWED_NATIVE_PREPARED_HOLD_IMPLEMENTATION_AND_STARTED_JOBS',
                previous_goal_turn='progress: native-bound Hold counterexamples/producer audit, committed and pushed54153481e',
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_files_verified=581,
                prepared_context_bits=132,source_function_induction_assumptions=0,complete_publication_outputs=2970,
                host_preparation_prefix_independence_bits=269,complete_publication_and_prefix_assumptions=0,
                both_simulators_and_actual_wrong_tag_wrong_payload_wrong_native_pin_negatives_pass=True,
                four_state_fallback_tested_in_Icarus=True,original_unwired_physical_pulldowns_explicit=f['explicit_original_unwired_pulldowns'],
                original_sequential_processes_unchanged=3813,additional_host_preparation_processes=1,
                original_clock_SDRAM_ROM_RIOT_readiness_barriers_retained=True,
                rejected_combined_sequential_formal_tool_failure_retained=True,
                composed_induction_and_full_interface_congruence_pass=True,
                first_two_state_fixture_four_state_coverage_failure_retained=True,
                native_simulation=dict(pid=pid,process=ps.stdout.strip(),verified_live_at=time.time(),host_limit=1500000000,
                    original_boot_presses=3,pause_checked_vectors=2810,input_reset_pass=True,first_HPS_ack_pass=True,
                    every_published_Hold_compared_to_raw=True,preparation_waits_instrumented=True,firmware_qualified=False),
                same_timing_fit=fit,assembly_or_FPGA_load_permitted=False,new_hardware_speed_gain_measured=False,
                nominal_ordinary_ns=120,target_ordinary_ns=60,preparation_overhead_or_60ns_schedule_qualified=False,
                full_goal_active=True,
                next_action='Observe the same timing builder and immutable native simulation. Collect routing/all-corner/crossing/SDRAM reports, compare preparation waits/useful progress with parent, and reject selection if added stalls erase gains. Integrate true60ns overlapping capture/event phases and calibrated timers; finish all original Cedar/input/Checkpoint/Rollback/persistence/three-boot/two-hour gates. No FPGA snapshot work.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-prepared-hold',extra_tools=TOOLS)
    print(r['status'],flush=True)
