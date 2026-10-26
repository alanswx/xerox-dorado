#!/usr/bin/env python3
"""Bind actual five-RIOT native memory inference and exact original-ROM startup.

Observe the existing native fitter and immutable startup by their real handles.
Mapping resource counts are compared with the compact parent's mapping, never
with placement counts. Running, early startup and physical timing stay distinct.
"""
import argparse
import base64
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from prepare_common_clock_native_startup import observer,retime
from prepare_registered_read_native import MONITOR,memc_monitor,operand_monitor
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'prepare_common_clock_native_startup.py','run_native_cedar_memory.py']


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'native-riot-blockram-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    receipt=ROOT/'fpga/mister/reports/2026-10-23-native-flag-windows/native-riot-blockram-integration-summary.json'
    prior=json.loads(receipt.read_text());assert prior['status']=='REVIEWED_NATIVE_RIOT_BLOCK_RAM_INTEGRATION'
    assert prior['native_stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    launch=BUILD/'native-riot-blockram-launch-v1-20261025';la=json.loads((launch/'launch-observation.json').read_text())
    assert la['builder_pid']==1029546 and la['manifest_sha256']==sha(stage/'r-front-control-native.json')
    query='''from pathlib import Path
import base64,hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert len(m['source_hashes'])==580 and all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==1029546 and not s['assembly_permitted']
assert s['stages'][0]['stage']=='map'and s['stages'][0]['returncode']==0
ps=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
  ps.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(v['returncode']==0 for v in ps)
assert not list((p/'core/output_files').glob('*.sof'))and not list((p/'core/output_files').glob('*.rbf'))
names=['map.log','core/output_files/Dorado.map.rpt','core/output_files/Dorado.map.summary','run-state.json']
files={n:base64.b64encode((p/n).read_bytes()).decode()for n in names}
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps,'source_files_verified':580,'files':files,'hashes':{n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in names}}))
'''.replace('PROJECT',repr(m['project']))
    (out/'same-native-map-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-native-map-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    fit=json.loads(remote.stdout);files=fit.pop('files');mapped=out/'completed-native-map'
    for name,data in files.items():
        assert not Path(name).is_absolute()and'..'not in Path(name).parts
        q=mapped/name;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(base64.b64decode(data))
        assert sha(q)==fit['hashes'][name]
    (out/'same-native-map-observation.json').write_text(json.dumps(fit,indent=2)+'\n')
    table=(mapped/'core/output_files/Dorado.map.rpt').read_text(errors='replace')
    rows=[]
    for line in table.splitlines():
        fields=[f.strip()for f in line.split(';')]
        if len(fields)>9 and 'native_ram'in fields[1]and fields[2]=='M10K block':
            assert fields[3]=='Simple Dual Port'and fields[4:8]==['128','8','128','8']and fields[8]=='1024',fields
            rows.append(fields[1])
    assert len(rows)==len(set(rows))==5,rows
    mapped_summary=(mapped/'core/output_files/Dorado.map.summary').read_text()
    baseline=BUILD/'native-compact-operand-routing-review-v1-20261025/terminal/core/output_files/Dorado.map.summary'
    old=baseline.read_text()
    def value(raw,label):
        matches=re.findall(r'^'+label+r' : ([\d,]+)$',raw,re.M);assert len(matches)==1,label
        return int(matches[0].replace(',',''))
    old_registers,new_registers=value(old,'Total registers'),value(mapped_summary,'Total registers')
    old_bits,new_bits=value(old,'Total block memory bits'),value(mapped_summary,'Total block memory bits')
    assert (old_registers,new_registers)==(54835,50362)and new_bits-old_bits==5120
    (out/'compact-parent-map.summary').write_bytes(baseline.read_bytes())
    startup=BUILD/'native-riot-blockram-startup-v1-20261025'
    p=json.loads(gzip.decompress((startup/'provenance.json.gz').read_bytes()));verify(startup/'inputs',p['inputs'])
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and p['native_RIOT_block_ram_connected']
    assert p['host_limit']==1500000000 and (startup/'host-cycle-limit.txt').read_text()=='1500000000\n'
    assert p['normal_baseboard_boot_presses']==3 and p['every_old_data_state_and_device_assertion_retained']
    assert p['host_clock_hz']==66666667 and p['original120ns_phase_schedule_unchanged']and not p['full60ns_event_batch_connected']
    original=(Path(p['baseline_run'])/'inputs/harness/tb_native_cedar.sv').read_text()
    raw,changes=retime(original)
    extra,signals,arrays=observer(BUILD/'native-clock-enable-sinks-v2-20261025',BUILD/'native-clock-enable-audit-xml-v1-20261025',BUILD/'native-riot-blockram-lint-v1-20261025')
    expected=raw.replace('endmodule',MONITOR+memc_monitor(m)+operand_monitor(m)+extra+'\nendmodule',1)
    assert expected==(startup/'inputs/harness/tb_native_cedar.sv').read_text()
    assert changes==[tuple(v)for v in p['harness_reversible_retiming_edits']]
    assert signals==p['pause_checked_scalar_vectors']and len(signals)==2810 and arrays==p['array_scope_not_in_scalar_image']
    for name in m['additional_native_lint_sources']:assert name in p['used_hdl']and p['inputs'][name]==m['source_hashes'][name]
    run=json.loads((startup/'run-state.json').read_text());assert run['status']=='RUNNING'
    assert all(a['returncode']==0 for a in run['actions'][:-1])and run['actions'][-1]['stage']=='test'and'returncode'not in run['actions'][-1]
    pid=run['actions'][-1]['pid'];ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,command='],text=True,capture_output=True)
    assert ps.returncode==0 and str(startup/'obj/native-cedar')in ps.stdout
    log=(startup/'test.log').read_text()
    assert 'NATIVE_INPUT_RESET_PASS'in log and 'NATIVE_HPS_ACK host=207944'in log
    assert not any(k in log for k in ['Assertion failed','%Error','NATIVE_HOST_ERROR'])
    retain(startup,out/'retained'/startup.name)
    (out/'retained'/startup.name/'run-state.json').write_bytes((startup/'run-state.json').read_bytes())
    result=dict(status='REVIEWED_NATIVE_RIOT_RAM_MAPPING_AND_STARTED_ORIGINAL_ROM_RUNTIME',
                previous_goal_turn='progress: proved/tested RAM implementation, staged/audited native hierarchy, launched same native fit and pushed312eaccff',
                prior_integration_receipt_sha256=sha(receipt),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                same_native_fit=fit,mapped_RIOT_M10K_instances=rows,
                synthesis_registers_before=old_registers,synthesis_registers_after=new_registers,synthesis_register_reduction=old_registers-new_registers,
                additional_native_memory_bits=new_bits-old_bits,complete_native_placement_routing_or_timing_qualified=False,
                startup=dict(pid=pid,process=ps.stdout.strip(),verified_live_at=time.time(),host_limit=1500000000,
                             pause_checked_vectors=2810,original_boot_presses=3,input_reset_pass=True,first_HPS_ack_pass=True,firmware_qualified=False),
                original_ROM_host_services_SDRAM_and_all_original_assertions_retained=True,
                nominal_ordinary_ns=120,target_ordinary_ns=60,new_hardware_speed_gain_measured=False,
                FPGA_or_protected_media_changed=False,full_goal_active=True,
                next_action='Observe SAME1029546 and runtime'+str(pid)+' at unchanged1.5B bound; collect terminal routing or all-corner/crossing/SDRAM reports. Then actual60ns chronological capture/timers/SDRAM and pinned hardware/Cedar/Rollback/persistence/three-boot/stress gates. Existing38212/62048 stay frozen; complete snapshots deferred.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-riot-blockram-runtime',extra_tools=TOOLS)
    print(r['status'],flush=True)
