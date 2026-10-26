#!/usr/bin/env python3
"""Retain native runtime gates, terminal routing attribution and compact-tag proof.

Keep the running original-ROM job and separate global-routing fit pending.
The465-bit descriptor remains private; formal/XZ/Verilator checks do not
substitute for its outstanding Icarus replays or full hardware qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_common_clock_native_startup import observer,retime
from prepare_registered_read_native import MONITOR,memc_monitor,operand_monitor
from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'prepare_common_clock_native_startup.py','diagnose_native_operand_replay_context.py',
 'test_common_clock_native_peripherals.py','collect_native_clock_enable_fit.py','derive_native_operand_support.py',
 'qualify_native_operand_buffer.py','test_native_operand_unknown.py','replay_native_operand_buffer.py',
 'review_native_cp_read_timing.py','stage_native_clock_enables.py','start_native_phase_guard_timing.py']


def read(p):return json.loads(p.read_text())


def state(name,status=None):
    d=BUILD/name;s=read(d/'run-state.json')
    if status:assert s['status']==status,(name,s['status'])
    if s.get('files'):verify(d,s['files'])
    return d,s


def live(s):
    action=s['actions'][-1];pid=action['pid']
    ps=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
    assert ps.returncode==0,(pid,ps.stderr)
    return dict(pid=pid,process=ps.stdout.strip(),verified_live=True,observed=time.time())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    stage=BUILD/'native-clock-enable-stage-v4-20261025';m=read(stage/'r-front-control-native.json');verify(stage,m['source_hashes'])
    routing=BUILD/'native-clock-enable-routing-review-v1-20261025';r=read(routing/'results.json')
    c=read(routing/'terminal/collection.json');verify(routing/'terminal',c['collected_hashes'])
    assert c['source_readback']==m['source_hashes']and r['source_files_verified']==578
    assert r['status']=='COLLECTED_TERMINAL_NATIVE_CLOCK_ENABLE_ROUTING_REJECTION'and r['terminal_state']['builder_pid']==458549
    fit=(routing/'terminal/core/output_files/Dorado.fit.rpt').read_text(errors='replace')
    assert re.search(r';\s*emu:emu\|network_step\s*;\s*7764\s*;',fit)
    assert '40,486 / 41,910 ( 97 % )'in r['fit_summary']and 'Total RAM Blocks : 330 / 553'in r['fit_summary']
    maps=[(routing/'terminal/core/output_files/Dorado.map.rpt').read_text(),
          (BUILD/'native-operand-buffer-full15-timing-review-v1-20261025/terminal/core/output_files/Dorado.map.rpt').read_text()]
    assert all('EccComputeErr_p_~0'in text and re.search(r'Number of logic cells representing combinational loops\s*;\s*1\s*;',text)for text in maps)
    periph,p=state('native-common-clock-peripherals-v2-20261025','PASS_ACTUAL_COMMON_CLOCK_NATIVE_INPUT_AND_HOST_DISK')
    assert len(p['cases'])==10 and all(v['passed']and v['returncode']==0 for v in p['cases'])
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    for n,h in p['staged_source_hashes'].items():assert sha(stage/n)==h
    for n,h in p['simulation_source_hashes'].items():assert sha(Path(n))==h
    # Every original fixture assertion survives; only clock/enable connections
    # are retimed. Both engines use the exact same declaration-order adapter.
    for name,fixture in p['fixtures'].items():
        raw=(periph/(name+'.sv')).read_text()
        for before,after in reversed(fixture['retiming_edits']):raw=raw.replace(after,before)
        assert raw==(periph/(name+'-original.sv')).read_text()
        assert raw.count('$fatal(')==fixture['original_assertions']
    diagnostic,d=state('native-operand-context-diagnostic-v1-20261025','OBSERVED_BOUNDED_NATIVE_OPERAND_CONTEXT')
    log=(diagnostic/'test.log').read_text()
    assert re.findall(r'OPERAND_UNKNOWN_INPUT record=\d+ name=(\w+)',log)==['interior_read_data']*6
    assert 'OPERAND_CONTEXT_DIAGNOSTIC_BOUND records=32'in log
    known=[]
    for engine in ['icarus','verilator']:
        directory,s=state('native-operand-buffer-full-known-'+engine+'-v3-20261025','PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER')
        assert s['known_invalid_interior_payload_fixture']and s['operand_prepare_edges']==2700
        assert outcomes((directory/'test.log').read_text())==outcomes((Path(s['source'])/'test.log').read_text())
        known.append(dict(engine=engine,state_sha256=sha(directory/'run-state.json'),preparation_edges=2700))
    startup,ss=state('native-common-clock-startup-v2-20261025');provenance=read_gzip(startup/'provenance.json.gz')
    verify(startup/'inputs',provenance['inputs']);assert provenance['host_limit']==1500000000
    assert int((startup/'host-cycle-limit.txt').read_text())==1500000000
    assert provenance['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    baseline=Path(provenance['baseline_run']);base=read_gzip(baseline/'provenance.json.gz')
    original=(baseline/'inputs/harness/tb_native_cedar.sv').read_text();text,changes=retime(original)
    assert changes==[tuple(v)for v in provenance['harness_reversible_retiming_edits']]
    extra,signals,arrays=observer(BUILD/'native-clock-enable-sinks-v2-20261025',BUILD/'native-clock-enable-audit-xml-v1-20261025')
    monitors=MONITOR+memc_monitor(m)+operand_monitor(m)+extra
    assert(startup/'inputs/harness/tb_native_cedar.sv').read_text()==text.replace('endmodule',monitors+'\nendmodule',1)
    assert len(signals)==2785 and len(arrays)==38 and provenance['normal_baseboard_boot_presses']==3
    assert sha(startup/'inputs/harness/host.cpp')==base['inputs']['harness/host.cpp']
    for n,h in m['source_hashes'].items():
        if n=='harness/tb_native_cedar.sv':assert sha(baseline/'inputs'/n)==h
        else:assert sha(startup/'inputs'/n)==h,n
    assert ss['actions'][2]['stage']=='compile'and ss['actions'][2]['returncode']==0
    startup_observation=live(ss)if ss['status']=='RUNNING'else ss
    _,support=state('native-operand-binary-support-v1-20261025','DERIVED_NATIVE_OPERAND_SUPPORT_REQUIRES_PROOF')
    assert support['binary_context_support']and support['support_bits']==465
    proof,q=state('native-operand-binary-mask-proof-v1-20261025','PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW')
    assert q['tag_support_bits']==465 and q['compared_physical_bits']==844 and q['assumptions']==0
    for a in q['actions']:
        assert a['passed']and a['returncode']==0;verify(proof/a['label'],a['files'])
        assert sha(proof/a['label']/'proof.log')==a['log_sha256']
    candidate=BUILD/'native-operand-binary-mask-candidate-v1-20261025/dorado_native_operand_buffer.sv'
    original_helper=stage/'src/verilog/rtl/dorado_native_operand_buffer.sv'
    assert sha(candidate)==q['helper_sha256']and sha(original_helper)==q['original_helper_sha256']
    mask_pattern=r"OPERAND_TAG_SUPPORT=1264'h[0-9a-f]+;"
    assert re.sub(mask_pattern,'MASK',candidate.read_text())==re.sub(mask_pattern,'MASK',original_helper.read_text())
    replays=[]
    for engine in ['verilator','icarus']:
        for mode in ['full','words']:
            directory,s=state('native-operand-binary-mask-'+mode+'-'+engine+'-v1-20261025')
            assert s['operand_proof_state_sha256']==sha(proof/'run-state.json')and s['buffer_helper_sha256']==sha(candidate)
            if s['status']=='RUNNING':observation=live(s)
            else:
                assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'
                assert s['operand_prepare_edges']==(2353 if mode=='full'else 8 if engine=='verilator'else 3)
                observation=dict(status=s['status'],preparation_edges=s['operand_prepare_edges'])
            replays.append(dict(engine=engine,mode=mode,observation=observation))
    for name,status in [('native-operand-binary-mask-xz-v1-20261025','PASS_NATIVE_OPERAND_UNKNOWN_FALLBACK'),
                        ('native-operand-binary-mask-xz-negative-v1-20261025','PASS_NATIVE_OPERAND_UNKNOWN_BYPASS_COUNTEREXAMPLE')]:
        directory,s=state(name,status);assert s['helper_sha256']==sha(candidate)
        if not s['broken_unknown_fallback']:assert 'checks=11895 unknown=11892 outputs=2969 physical=844'in(directory/'test.log').read_text()
    global_stage=BUILD/'native-clock-enable-global-stage-v1-20261025';g=read(global_stage/'r-front-control-native.json');verify(global_stage,g['source_hashes'])
    assert g['physical_global_advance_assignment']
    assert {n:h for n,h in g['source_hashes'].items()if Path(n).suffix in ['.v','.sv']}=={n:h for n,h in m['source_hashes'].items()if Path(n).suffix in ['.v','.sv']}
    launch=BUILD/'native-clock-enable-global-launch-v1-20261025';la=read(launch/'launch-observation.json')
    assert la['manifest_sha256']==sha(global_stage/'r-front-control-native.json')and la['builder_pid']==576472
    query='''from pathlib import Path
import hashlib,json,subprocess,time
p=Path(PROJECT);m=json.loads((p/'r-front-control-native.json').read_text())
assert all(hashlib.sha256((p/n).read_bytes()).hexdigest()==h for n,h in m['source_hashes'].items())and len(m['source_hashes'])==578
s=json.loads((p/'run-state.json').read_text());assert s['builder_pid']==576472 and not s['assembly_permitted']
ps=[]
if s['status']=='RUNNING':
 for pid in [s['builder_pid'],s['stages'][-1]['pid']]:
  r=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True);ps.append({'pid':pid,'returncode':r.returncode,'process':r.stdout.strip()})
 assert any(r['returncode']==0 for r in ps)
lines=[line for line in(p/'fit.log').read_text().splitlines()if 'network_step'in line]if(p/'fit.log').exists()else[]
assert any('network_step~CLKENA0 with 8023 fanout uses global clock'in line for line in lines)
print(json.dumps({'observed':time.time(),'run_state':s,'processes':ps,'global_advance_report':lines,'source_files_verified':len(m['source_hashes'])}))
'''.replace('PROJECT',repr(g['project']))
    (out/'same-global-routing-query.py').write_text(query)
    remote=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=query,text=True,capture_output=True)
    (out/'same-global-routing-query.stderr').write_text(remote.stderr);assert remote.returncode==0,remote.stderr
    remote_state=json.loads(remote.stdout);(out/'same-global-routing-observation.json').write_text(json.dumps(remote_state,indent=2)+'\n')
    medium=BUILD/'cedar-work-native-hints.pack';assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    names=['native-common-clock-followup-review-v1-20261025','native-clock-enable-routing-review-v1-20261025','native-common-clock-peripherals-v1-20261025','native-common-clock-peripherals-v2-20261025',
       'native-common-clock-startup-v1-20261025','native-common-clock-startup-v2-20261025','native-operand-context-diagnostic-v1-20261025',
       'native-operand-buffer-full-known-icarus-v3-20261025','native-operand-buffer-full-known-verilator-v3-20261025',
       'native-clock-enable-global-stage-v1-20261025','native-clock-enable-global-lint-v1-20261025','native-clock-enable-global-audit-v1-20261025','native-clock-enable-global-launch-v1-20261025',
       'native-operand-binary-support-v1-20261025','native-operand-binary-mask-candidate-v1-20261025','native-operand-binary-mask-proof-v1-20261025',
       'native-operand-binary-mask-xz-v1-20261025','native-operand-binary-mask-xz-negative-v1-20261025']
    names+=['native-operand-binary-mask-'+mode+'-'+engine+'-v1-20261025'for mode in ['full','words']for engine in ['verilator','icarus']]
    for name in names:retain(BUILD/name,out/'retained'/name)
    result=dict(status='REVIEWED_COMMON_CLOCK_NATIVE_RUNTIME_AND_ROUTING_FOLLOWUP',scope=__doc__,
        terminal_fit=r,peripheral_cases=p['cases'],known_auxiliary_payload_full_replays=known,
        native_startup=dict(bound=1500000000,paused_vectors=2785,observation=startup_observation,firmware_qualified=False),
        compact_descriptor=dict(candidate_only=True,tag_bits_before=909,tag_bits_after=465,all844_view_bits_inductively_proved=True,
          assumptions=0,negative_controls=2,XZ_checks=11895,replays=replays,Verilator_preparation_edges_before=[2700,43],Verilator_preparation_edges_after=[2353,8]),
        same_global_routing_build=remote_state,FPGA_and_protected_media_changed=False,hardware_gain_measured=False,
        original60ns_cadence_or_complete_goal_qualified=False,full_goal_active=True,
        next_action='Observe SAME global576472, native38212, and compact-tag Icarus peers. Collect terminal all-corner/SDRAM reports. Select compact tag only after its remaining replays and source-bound integration; then original60ns phases and full hardware/Cedar/input/Rollback gates.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


def read_gzip(p):return json.loads(gzip.decompress(p.read_bytes()))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-common-clock-runtime',extra_tools=TOOLS)
    print(r['status'],flush=True)
