#!/usr/bin/env python3
"""Archive shared capture progress without accepting pending native/full gates.

Keep the wrong retained-prefix capture, declaration failure, architectural
counterexample, complete passing fixtures and verified live peers. Review the
earlier binary budget independently; its timing miss rejects15ns even while
the corrected full-context variant and Icarus runs are still being observed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from fit_owned_r_packet import TOP
from fit_shared_control_capture import checked
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import CORNERS,verify
from review_late_hold_and_input_reset import first_path,retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_shared_control_capture.py','test_shared_control_capture.py','prove_shared_control_frames.py',
       'test_shared_capture_unknown.py','fit_shared_control_capture.py','review_shared_control_capture.py']


def read(p):return json.loads(p.read_text())


def budget():
    stage=BUILD/'shared-control-capture-owned-fit-stage-v2-20261024';p=read(stage/'provenance.json');verify(stage,p['files'])
    d=BUILD/'shared-control-capture-owned-terminal-v2-20261024';receipt=BUILD/(d.name+'.tgz.json');r=read(receipt)
    assert sha(BUILD/(d.name+'.tgz'))==r['archive_sha256'] and len(r['members'])==92
    for row in r['members']:
        f=d/row['path'];assert sha(f)==row['sha256'] and f.stat().st_size==row['bytes']
    assert read(d/'provenance.json')==p and p['input_bits']==422 and p['result_bits']==4943
    assert (d/'top.sv').read_text()==TOP and not p['assembly_permitted']
    s=read(d/'run-state.json');back=read(d/'collected-input-check.json')
    assert s['status']=='TERMINAL_REVIEW_REQUIRED' and [a['stage']for a in s['stages']]==['map','fit','sta','corners']
    assert all(a['returncode']==0 for a in s['stages'])
    assert back['job_pids_absent']==[s['builder_pid']]+[a['pid']for a in s['stages']]
    for n,h in p['files'].items():
        assert sha(d/n)==back['actual'][n]
        if sha(d/n)!=h:
            assert n=='clock_fit.qsf' and (d/n).read_bytes()==(stage/n).read_bytes()+b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    corners=[]
    for c in CORNERS:
        ucp=re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;',(d/f'clock-ucp-{c}.rpt').read_text())
        assert len(ucp)==6 and all(a=='0' and b in ['', '0'] for a,b in ucp)
        for mode in ['recovery','removal']:assert 'Nothing to report.' in (d/f'clock-{mode}-{c}.rpt').read_text()
        pulse=float(re.search(r'Worst case slack is (-?[0-9.]+)',(d/f'clock-mpw-{c}.rpt').read_text())[1])
        corners.append(dict(condition=c,setup=first_path(d/f'clock-setup-{c}.rpt'),hold=first_path(d/f'clock-hold-{c}.rpt'),
                            evaluator=first_path(d/f'clock-evaluator-{c}.rpt'),pulse_width_ns=pulse))
    for label,width in [('INPUT',422),('OUTPUT',4943)]:
        assert re.findall('OWNED_PACKET_'+label+r'_BITS (\d+)',(d/'corners.log').read_text())==[str(width)]*4
    fit=(d/'clock_fit.fit.summary').read_text();report=(d/'clock_fit.fit.rpt').read_text(encoding='latin1')
    area=dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([0-9,]+)',fit)[1].replace(',','')),
              registers=int(re.search(r'Total registers : ([0-9,]+)',fit)[1].replace(',','')),
              MLAB_bits=int(re.search(r'Total MLAB memory bits\s*;\s*([0-9,]+)',report)[1].replace(',','')))
    assert area['MLAB_bits']==9216
    assert [c['setup']['slack_ns']for c in corners]==[-1.499,-1.284,4.291,4.675]
    return dict(status='REJECTED_15NS_EARLIER_BINARY_COMPONENT',corners=corners,area=area,
                full_context_XZ_revision_fitted=False,prior_102_pin_worst_setup_ns=-2.305,
                worst_setup_improvement_ns=0.806,collected_members=92,collection_sha256=sha(receipt)),[stage,d],receipt


def review(out):
    assert not out.exists();v=BUILD/'shared-control-capture-verilator-v4-20261024';s=checked(v)
    frame=BUILD/'shared-control-frame-proof-v1-20261024';f=read(frame/'run-state.json');verify(frame,f['files'])
    assert f['status']=='PASS_SHARED_CONTROL_FRAME_RESULTS_AND_CHRONOLOGY' and [r['waves']for r in f['cases']]==[5,24]
    for r in f['cases']:
        d=frame/f"w{r['waves']}";verify(d,r['files']);assert r['passed'] and r['returncode']==0 and sha(d/'proof.log')==r['log_sha256']
    for name in ['dorado_capture_hold_atomic_step','dorado_capture_terminal_body']:
        assert module((v/'capture.sv').read_text(),name)==module((frame/'actual.sv').read_text(),name)
    four=BUILD/'shared-control-capture-unknown-v4-20261024';x=read(four/'run-state.json');verify(four,x['files'])
    assert x['status']=='PASS_SHARED_CAPTURE_FULL_CONTEXT_XZ_FALLBACK' and x['source_state_sha256']==sha(v/'run-state.json')
    for a in x['actions']:assert a['returncode']==0 and sha(four/(a['label']+'.log'))==a['log_sha256']
    assert 'known=4 unknown=15528 checks=15532 bad_architectural_Q=15528' in (four/'test.log').read_text()
    dirs=[v,frame,four];historical=[]
    for name in ['shared-control-capture-verilator-v1-20261024','shared-control-capture-verilator-diagnostic-v1-20261024',
                 'shared-control-capture-disabled-verilator-v1-20261024','shared-control-capture-verilator-v2-20261024',
                 'shared-control-capture-wrong-start-verilator-v2-20261024','shared-control-capture-verilator-v3-20261024',
                 'shared-control-capture-icarus-v3-20261024']:
        d=BUILD/name;r=read(d/'run-state.json');verify(d,r['files'])
        assert r['status']!='RUNNING' and all(a.get('returncode') is not None for a in r['actions'])
        historical.append(dict(directory=name,status=r['status'],state_sha256=sha(d/'run-state.json')));dirs.append(d)
    bad=BUILD/'shared-control-capture-wrong-start-verilator-v2-20261024';log=(bad/'test.log').read_text()
    actual_bits=[int(v)for v in re.findall(r'PACKET_DIFF last bit=(\d+)',log)]
    assert 'PACKET second result mismatch record=14' in log and any(v>=1626 for v in actual_bits)
    peers=[];snapshots={}
    for version in [2,4]:
        d=BUILD/f'shared-control-capture-icarus-v{version}-20261024';data=(d/'run-state.json').read_bytes();r=json.loads(data);verify(d,r['files'])
        if r['status']=='RUNNING':
            assert len(r['actions'])==2 and r['actions'][0]['returncode']==0
            a=r['actions'][-1];assert 'returncode' not in a
            ps=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,stat=,command='],text=True).strip();assert str(d/'test.vvp') in ps
            observation=dict(status='VERIFIED_LIVE_FULL_ICARUS',pid=a['pid'],ps=ps,observed=time.time(),terminal_pass_required=True)
        else:
            assert r['status']=='PASS_FULL_PROCESSOR_SHARED_CAPTURE' and r['actions'][-1]['returncode']==0
            observation=dict(status=r['status'],observed=time.time())
        if version==4:assert r['files']==s['files']
        peers.append(dict(directory=d.name,state_sha256=hashlib.sha256(data).hexdigest(),**observation));dirs.append(d);snapshots[d.name]=data
    fit,fd,receipt=budget();dirs+=fd
    latest=BUILD/'shared-control-capture-owned-fit-stage-v4-20261024';p=read(latest/'provenance.json');verify(latest,p['files'])
    assert p['full_context_XZ_fallback'] and p['tests_state_sha256']==sha(v/'run-state.json') and not p['assembly_permitted'];dirs.append(latest)
    native=BUILD/'control-short-native-jumper-read-v3-20261024';n=read(native/'result.json')
    assert n['read_only_guest_inspection'] and n['pid']==69736 and 'detached' in (native/'lldb.log').read_text().lower()
    assert n['riot_ram']['timer100ms_word']==13 and n['riot_ram']['EndOfDelay_word']==30;dirs.append(native)
    out.mkdir(parents=True)
    for d in dirs:
        retain(d,out/d.name)
        if d.name in snapshots:(out/d.name/'run-state.json').write_bytes(snapshots[d.name])
    for name in ['shared-control-capture-remote-observation-v1-20261024.json',
                 'shared-control-capture-owned-remote-launch-v2-20261024.json','shared-control-capture-owned-remote-launch-v4-20261024.json']:
        (out/name).write_bytes((BUILD/name).read_bytes())
    (out/receipt.name).write_bytes(receipt.read_bytes())
    result=dict(status='STAGED_SHARED_CONTROL_CAPTURE_WITH_PENDING_FULL_GATES',scope=__doc__,
                full_Verilator_transactions=14884,all2470_results=True,all844_owners=True,one9216_bit_SRAM=True,
                all100_clock_counts=True,geometry_cases=403,cache_coverage=s['cache_coverage'],
                binary_frame_equivalence_inputs=955,frame_output_results=363,frame_depths=[5,24],
                full_context_bits=1941,XZ_cases=15528,bad_guard_changes_actual_processor_Q=True,
                wrong_start_changes_actual_architectural_bits=actual_bits,peers=peers,historical=historical,earlier_fit=fit,
                latest_fit_pending=True,latest_fit_project='/home/alans/mister/shared-control-capture-owned-packet-v4-20261024',
                latest_builder_pid=1918004,older_fit_collected_once=True,old_payload_fast_branch_error_preserved=True,
                native_same_process_read=dict(pid=69736,host=n['counters']['host_ticks'],guest=n['counters']['guest_ticks'],
                                             timer100ms=13,EndOfDelay=30,debugger_detached=True),
                actual_CP_source_and_consumer_arrivals_pending=True,original16_R_latch_epoch_qualification_pending=True,
                sparse_pair_first_and_unknown_use_original_consumers=True,extra_architectural_state=0,
                native_integrated=False,hardware_changed=False,FPGA_available=True,new_RBF_ready=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),[],label='shared-control-capture-staging',extra_tools=TOOLS)
    print(r['status'])
