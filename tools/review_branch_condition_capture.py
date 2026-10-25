#!/usr/bin/env python3
"""Preserve bounded branch-family qualification and a fresh native boot diagnostic.

Separate all-input binary depth equivalence, delayed original-storage tests,
the rejected hierarchical-port proof attempt, and native observations. Actual
ALU/common-owner composition, unknown inputs, source arrivals and physical
timing remain open. Neither a native fit nor hardware speed is selected.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from native_branch_flag_sources import bindings
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_branch_condition_reference.py','test_branch_condition_capture.py',
       'prove_branch_condition_depth.py','review_branch_condition_capture.py',
       'native_branch_flag_sources.py','stage_native_cp_read.py']


def read(path):return json.loads(path.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    rows=[];directories=[]
    for directory in sorted(BUILD.glob('branch-condition-*-20261025')):
        if not (directory/'run-state.json').exists():continue
        state=read(directory/'run-state.json')
        if 'depth' in directory.name:
            verify(directory,state['files'])
        else:
            state,row=observed(directory);assert not row.get('verified_live');rows.append(row)
        retain(directory,out/directory.name);directories.append(directory)
    candidate=(ROOT/'verilog/rtl/dorado_branch_condition_step.sv').read_text()
    groups=[]
    for profile,values in [('balanced',(1000,1000,1000)),('early',(1750,400,1000)),('aperture',(3200,1500,1000))]:
        peers=[BUILD/f'branch-condition-{profile}-{e}-v4-20261025' for e in ['icarus','verilator']]
        states=[read(d/'run-state.json') for d in peers]
        assert all(s['status']=='PASS_BRANCH_CONDITION_CAPTURE' and s['returncode']==0 for s in states)
        assert states[0]['outcomes']==states[1]['outcomes']
        for d,s in zip(peers,states):
            assert (d/'candidate.sv').read_text()==candidate
            assert s['reference_manifest']['compiled_profile_ps']==dict(zip(['gate','FF','latch'],values))
            log=(d/'test.log').read_text()
            assert 'BRANCH_CAPTURE_PASS checks=4869 Qs=80 file_bits=64 tasks=16 operations=512 logical=128 freeze=128 carry_ones=136 changed=ffffffffffffffffffff' in log
            assert 'BRANCH_DISABLED_CLOCK_PASS sources=37 patterns=4' in log
            assert 'BRANCH_WRITE_WINDOWS_PASS windows=385 address_changes=0 data_changes=0' in log
            assert re.findall(r'^BRANCH_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)==[
                ('0','1604'),('1','1604'),('2','1604'),('3','385')]
        for relative in ['tb.sv','candidate.sv','reference/cone.sv','reference/branch_condition_reference.sv']:
            assert sha(peers[0]/relative)==sha(peers[1]/relative)
        for path in (peers[0]/'reference/timed').iterdir():
            assert sha(path)==sha(peers[1]/'reference/timed'/path.name)
        groups.append(dict(profile=profile,events=4869,stored_bits=80,write_windows=385,
                           exact_both_engine_results=True,outcomes=states[0]['outcomes']))
    negatives=[]
    for mutation in ['flag-select','task-address','write-after-close']:
        differences=[]
        for engine in ['icarus','verilator']:
            good=BUILD/f'branch-condition-early-{engine}-v4-20261025'
            bad=BUILD/f'branch-condition-early-{mutation}-{engine}-v4-20261025'
            state=read(bad/'run-state.json');assert state['status']=='PASS_BRANCH_CAPTURE_COUNTEREXAMPLE'
            assert state['actions'][0]['returncode']==0 and state['returncode']!=0
            log=(bad/'test.log').read_text()
            bits=re.findall(r'^BRANCH_STATE_DIFF check=(\d+) bit=(\d+) predicted=([01]) original=([01])$',log,re.M)
            assert bits and all(p!=a for c,b,p,a in bits)
            assert 'BRANCH original state mismatch' in log
            assert sha(good/'tb.sv')==sha(bad/'tb.sv')
            assert sha(good/'reference/branch_condition_reference.sv')==sha(bad/'reference/branch_condition_reference.sv')
            for path in (good/'reference/timed').iterdir():assert sha(path)==sha(bad/'reference/timed'/path.name)
            actual=(bad/'candidate.sv').read_text();cone=(bad/'reference/cone.sv').read_text()
            if mutation=='flag-select':
                assert actual==candidate
                expression=state['reference_manifest']['visible_mux_expression']
                assert cone.count('!'+expression)==1
                assert cone.replace('!'+expression,expression,1)==(good/'reference/cone.sv').read_text()
            elif mutation=='task-address':
                wrong="4*(data[19:16]^4'b1)+:4";assert actual.count(wrong)==1
                assert actual.replace(wrong,'4*data[19:16]+:4',1)==candidate
            else:
                wrong='if(clocks[3] && !before_clocks[3])';assert actual.count(wrong)==1
                assert actual.replace(wrong,'if(!clocks[3])',1)==candidate
            if mutation!='flag-select':assert cone==(good/'reference/cone.sv').read_text()
            differences.append(bits)
        assert differences[0]==differences[1]
        negatives.append(dict(mutation=mutation,actual_state_differences=differences[0]))
    proof=BUILD/'branch-condition-depth-v3-20261025';p=read(proof/'run-state.json')
    assert p['status']=='PASS_BRANCH_THREE_PASS_DEPTH_AND_VISIBLE_NEGATIVE'
    assert p['arbitrary_binary_input_bits']==158 and p['compared_result_bits']==85 and p['protocol_assumptions']==0
    assert (proof/'candidate.sv').read_text()==candidate
    assert sha(proof/'cone.sv')==sha(BUILD/'branch-condition-early-icarus-v4-20261025/reference/cone.sv')
    assert 'no model found: SUCCESS!' in (proof/'depth3.log').read_text()
    assert 'model found: FAIL!' in (proof/'depth1.log').read_text()
    witness=read(proof/'depth1-counterexample.json')
    ports={s['name']:s for s in witness['signal']}
    a=int(ports['actual']['data'][0],2);b=int(ports['expected']['data'][0],2)
    assert (a>>69)&15!=(b>>69)&15
    failed=BUILD/'branch-condition-depth-v1-20261025'
    assert read(failed/'run-state.json')['status']=='TERMINAL_BRANCH_DEPTH_NOT_ESTABLISHED'
    assert "Found 249 problems in 'check -assert'" in (failed/'depth3.log').read_text()
    native=BUILD/'native-cp-read-j19-boot-v1-20261025'
    n=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()));verify(native/'inputs',n['inputs'])
    assert n['host_limit']==int((native/'host-cycle-limit.txt').read_text())==5000000000
    assert n['experimental_j19_ordered_epochs'] and not n['paired_mode_enabled']
    assert not n['new_branch_condition_family_integrated'] and not n['assembly_permitted']
    assert n['original_ROM_bytes_unchanged'] and len(n['used_hdl'])==194
    assert n['component_state_sha256']==sha(BUILD/'j19-full-verilator-v2-20261025/run-state.json')
    lint=read(native/'lint-state.json');assert lint['returncode']==0
    assert len(re.findall(r'^%Warning-UNOPTFLAT:',(native/'lint.log').read_text(),re.M))==8
    live=[]
    for d in [native,BUILD/'control-short-native-jumper-boot-v2-20261024']:
        s=read(d/'run-state.json');a=s['actions'][-1]
        assert s['status']=='RUNNING' and a['stage']=='test' and 'returncode' not in a
        ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True,check=True)
        assert str(d.resolve()) in ps.stdout
        live.append(dict(directory=d.name,pid=a['pid'],observed=time.time(),ps=ps.stdout.strip(),
                         host_limit=int((d/'host-cycle-limit.txt').read_text()),
                         progress=(d/'test.log').read_text().splitlines()[-6:]))
    retain(native,out/native.name)
    source=bindings(BUILD/'control-short-native-jumper-boot-v2-20261024')
    m=read(BUILD/'branch-condition-early-icarus-v4-20261025/reference/manifest.json')
    assert m['packages']['source_sha256']==source['source_hashes']['chm/sil/ProcH-Rev-Ce.dm!1_/ProcH-Rev-Ce.wl']
    for package,(start,width) in m['packages']['sequential'].items():
        assert source['packages'][package]['stored_bits']==width
        assert source['packages'][package]['kind']==m['packages']['packages'][package]
    assert sha(BUILD/'cedar-work-native-hints.pack')=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_BRANCH_CONDITION_COMPONENT_AND_NEW_NATIVE_BOOT_DIAGNOSTIC',scope=__doc__,
                runs=rows,positive_both_engine_groups=groups,actual_state_negatives=negatives,
                binary_depth=dict(input_bits=158,result_bits=85,assumptions=0,passes=3,baseline=8,
                                  one_pass_changes_actual_visible_flags=True),
                native_source_binding=source,native_observations=live,
                primitive_input_port_lowering_failure_preserved=True,
                test_ALU_status_inputs_are_controlled_not_native_ALU_execution=True,
                complete_80_bit_family_changed_in_tests=True,original_844_layout_changed=False,
                new_branch_family_native_integrated=False,native_paired_phases_qualified=False,
                new_native_is_fresh_diagnostic_not_fit_selection=True,
                source_arrival_and_unknown_qualification=False,physical_timing_qualified=False,
                firmware_Cedar_or_speed_qualified=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='branch-condition-capture',extra_tools=TOOLS)
    print(r['status'])
