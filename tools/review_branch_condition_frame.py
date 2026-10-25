#!/usr/bin/env python3
"""Review early/late flag capture with original processor/control producers.

Retain full physical-state, SRAM, event and observable comparisons, two wrong
frame bindings, the digital arrival witness, and all-input depth equivalence.
Native ALU arrivals and full-core timing remain unqualified.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from native_branch_flag_sources import alu_producers
from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_processor_branch_composition import checked
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=[Path(__file__).name,'test_processor_branch_composition.py',
       'prove_branch_condition_depth.py','native_branch_flag_sources.py']


def review(out,timeout_readback=None,timeout_decoded=None):
    assert not out.exists();out.mkdir(parents=True)
    good=[BUILD/f'processor-branch-early-frame-{e}-v1-20261025'for e in ['verilator','icarus']]
    states=[checked(d)for d in good]
    candidate=ROOT/'verilog/rtl/dorado_branch_condition_frame_step.sv'
    for d,s in zip(good,states):
        assert s['status']=='PASS_ORIGINAL_PROCESSOR_BRANCH_COMPOSITION'and s['returncode']==0
        assert s['explicit_early_frame']and s['optimized_branch_transfer_checked']and s.get('mutation')is None
        assert sha(d/'branch-step.sv')==sha(candidate)
        pins=s['reference']['branch_reference']['pin_index']
        direct=set(s['reference']['extra_producer_pins'])|{'alu.00'}
        assert len(direct)==13
        assert s['early_direct_producer_mask']==sum(1<<pins[name]for name in direct)
    assert states[0]['outcomes']==states[1]['outcomes']
    assert 'events=14884 states=924 RAM=9216 clocks=104 observations=541'in states[0]['outcomes'][-1]
    for relative in ['machines.sv','reference/branch/cone.sv','reference/timed/processor_branch_reference.sv']:
        assert sha(good[0]/relative)==sha(good[1]/relative)
    a=(good[0]/'tb.sv').read_text().replace(str(good[0]),'RUN')
    b=(good[1]/'tb.sv').read_text().replace(str(good[1]),'RUN');assert a==b
    negatives=[];dirs=list(good)
    for name,expected in [
        ('old',[('15','69','1','0'),('15','70','0','1'),('15','74','0','1'),('15','79','1','0')]),
        ('late',[('23','65','1','0'),('23','66','1','0'),('23','67','1','0')])]:
        d=BUILD/f'processor-branch-early-{name}-negative-icarus-v1-20261025';s=checked(d)
        assert s['returncode']!=0 and s['mutation']=='early-all-'+name
        assert sha(d/'branch-step.sv')==sha(candidate)
        log=(d/'test.log').read_text()
        bits=re.findall(r'^COMPOSITION_TRANSFER_DIFF event=(\d+) bit=(\d+) predicted=([01]) original=([01])$',log,re.M)
        assert bits==expected
        # Only the DUT's early-frame binding changes. All stimulus, original
        # circuits and assertions retain their exact text.
        bad=(d/'tb.sv').read_text().replace(str(d),'RUN')
        wrong='transfer_before_pins'if name=='old'else'full.branch_pins'
        wrong='transfer_before_pins,'+wrong+',full.branch_pins,'
        assert bad.count(wrong)==1
        assert bad.replace(wrong,'transfer_before_pins,transfer_early_pins,full.branch_pins,',1)==b
        for p in (d/'reference/timed').iterdir():
            if p.suffix in ['.sv','.v']:assert sha(p)==sha(good[1]/'reference/timed'/p.name)
        negatives.append(dict(binding=name,actual_state_differences=bits));dirs.append(d)
    proof=BUILD/'branch-condition-frame-depth-v1-20261025'
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_BRANCH_THREE_PASS_DEPTH_AND_VISIBLE_NEGATIVE'
    assert p['explicit_early_frame']and p['arbitrary_binary_input_bits']==196
    assert p['compared_result_bits']==85 and p['protocol_assumptions']==0
    assert sha(proof/'candidate.sv')==sha(candidate)
    assert all(a['returncode']==0 and a['passed']for a in p['actions'])
    witness=json.loads((proof/'depth1-counterexample.json').read_text());signals={s['name']:s for s in witness['signal']}
    assert ((int(signals['actual']['data'][0],2)^int(signals['expected']['data'][0],2))>>69)&15
    dirs.append(proof)
    trace=BUILD/'processor-branch-arrivals-icarus-v1-20261025';s=checked(trace)
    assert s['status']=='PASS_ORIGINAL_PROCESSOR_BRANCH_COMPOSITION'and s['passive_flag_arrival_trace']
    log=(trace/'test.log').read_text()
    assert 't=2401.000 event=19 root=1 C=0 S=1 A=f B=e'in log
    assert 't=2405.000 event=19 root=1 C=1'in log
    assert 't=2805.000 event=23 root=1 C=1 S=0 A=0'in log
    assert 't=2806.000 event=23 root=1 C=1 S=0 A=e'in log
    dirs.append(trace)
    native=BUILD/'native-cp-read-j19-boot-v1-20261025';sources=alu_producers(native)
    inventory=BUILD/'branch-frame-source-binding-v1-20261025'
    assert json.loads(json.dumps(sources))==json.loads((inventory/'results.json').read_text());dirs.append(inventory)
    live=[]
    for d in [native,BUILD/'control-short-native-jumper-boot-v2-20261024']:
        s=json.loads((d/'run-state.json').read_text());a=s['actions'][-1]
        assert s['status']=='RUNNING'and a['stage']=='test'and 'returncode'not in a
        ps=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,command='],text=True).strip()
        assert str(d)in ps and int((d/'host-cycle-limit.txt').read_text())==5000000000
        progress=[x for x in (d/'test.log').read_text().splitlines()if x.startswith('NATIVE_PROGRESS')][-1]
        live.append(dict(directory=d.name,pid=a['pid'],ps=ps,observed=time.time(),host_limit=5000000000,progress=progress))
    timeout=None
    if timeout_readback is not None:
        assert timeout_decoded is not None
        dec=json.loads((timeout_decoded/'results.json').read_text())
        verify(timeout_decoded/'inputs',dec['input_hashes'])
        assert dec['status']=='PASS_ORIGINAL_ROM_WAIT_BINDING'
        assert dec['readback_sha256']==sha(timeout_readback/'result.json')
        assert dec['actual_RTC430_address']==179
        assert dec['RTC430_value']==dec['emulator_task_T_threshold']==13
        assert dec['timeout_subtraction']==65535 and dec['timeout_subtraction_is_negative']
        assert not dec['original_ROM_or_guest_state_changed']
        sampled=json.loads((timeout_readback/'result.json').read_text())
        assert f"Process {sampled['pid']} detached"in(timeout_readback/'lldb.log').read_text()
        timeout=dict(decoded=dec,readback_directory=timeout_readback.name,detached_and_resumed=True,
                     interpretation='Actual RTC430 has advanced to13; 13-13-1 remains negative, so the original timeout is not yet expired')
        dirs.extend([timeout_readback,timeout_decoded])
    for d in dirs:retain(d,out/d.name)
    (out/'candidate.sv').write_bytes(candidate.read_bytes())
    result=dict(status='REVIEWED_COUPLED_EARLY_LATE_FLAG_CAPTURE',scope=__doc__,summary=states[0]['outcomes'][-1],
                all104_clock_counts=states[0]['outcomes'][:-1],explicit_early_frame=True,
                binary_depth=dict(inputs=196,outputs=85,assumptions=0,depth=3,baseline=8,one_pass_changes_visible_flags=True),
                wrong_frame_binding_negatives=negatives,
                original_digital_arrivals=dict(profile_ps=[1000,1000,1000],arithmetic_data_ns=2401,close_ns=2405,
                                               task_file_close_ns=2805,late_task_file_data_ns=2806),
                ALU_source_inventory=sources,native_observations=live,
                timeout_observation=timeout,
                fixture_direct_early_mask_is_not_a_native_binding=True,
                next_action='Use actual ALU function/operand producers to construct early and late frames, including registered ProcL e12 mode; integrate the common native owner and then full-core timing',
                native_qualified=False,source_arrivals_and_unknowns_qualified=False,physical_timing_qualified=False,
                firmware_Cedar_or_hardware_speed_qualified=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path);p.add_argument('--timeout-readback',type=Path);p.add_argument('--timeout-decoded',type=Path)
    a=p.parse_args();assert bool(a.timeout_readback)==bool(a.timeout_decoded)
    r=review(a.output.resolve(),a.timeout_readback.resolve()if a.timeout_readback else None,a.timeout_decoded.resolve()if a.timeout_decoded else None)
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='processor-branch-frame-timeout'if a.timeout_readback else'processor-branch-frame',extra_tools=TOOLS)
    print(r['status'])
