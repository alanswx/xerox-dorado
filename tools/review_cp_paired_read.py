#!/usr/bin/env python3
"""Review combined digital paths and the RBase arrival-contract counterexample.

Pending simulator peers require a current process observation. Preserve every
failure, source model, full trace and exact phase experiment. No native timing,
60ns cadence, firmware boot or measured hardware improvement is implied.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time

from generate_cp_paired_read import processor,caller
from inspect_external_r_packet import structure
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_cp_interior_stage import event_assertions
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_cp_paired_read.py','test_cp_paired_read.py','record_cp_source_program.py',
       'probe_rbase_capture_arrival.py','replay_cp_paired_read.py','inspect_external_r_packet.py','review_cp_paired_read.py']


def read(p):return json.loads(p.read_text())


def observed(d):
    s=read(d/'run-state.json');verify(d,s.get('files',{}))
    source_files=s.get('source_files',[])
    if isinstance(source_files,dict):verify(Path(s['source']),source_files)
    else:
        for row in source_files:assert sha(d/row['path'])==row['output_sha256']
    for a in s.get('actions',[]):
        if 'log_sha256'in a:assert sha(d/(a['label']+'.log'))==a['log_sha256']
    row=dict(directory=d.name,status=s['status'],state_sha256=sha(d/'run-state.json'),observed=time.time())
    if s['status']=='RUNNING':
        a=s['actions'][-1];ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,etime=,time=,stat=,command='],capture_output=True,text=True)
        if str(d)not in ps.stdout:
            again=read(d/'run-state.json')
            assert again['status']!='RUNNING',(d,ps.stdout,ps.stderr)
            return observed(d)
        row.update(verified_live=True,pid=a['pid'],ps=ps.stdout.strip())
    return s,row


def review(out):
    assert not out.exists();out.mkdir(parents=True);runs=[];pending=[]
    directories=[]
    for pattern in ['cp-paired-read-*-20261025','cp-original-program-*-20261025','rbase-edge-*-20261025']:
        for d in sorted(BUILD.glob(pattern)):
            if 'review' in d.name:continue
            if (d/'run-state.json').exists():
                s,row=observed(d);runs.append(row)
                if row.get('verified_live'):pending.append(row)
            else:
                assert (d/'failure.json').exists(),d
                runs.append(dict(directory=d.name,status=read(d/'failure.json')['status'],failure_sha256=sha(d/'failure.json')))
            retain(d,out/d.name);directories.append(d)
    fn=(BUILD/'native-cp-source-v1-20261024/functions.sv').read_text()
    base=BUILD/'supported-fall-guard-owned-verilator-v1-20261024'
    candidate=processor((base/'candidate.sv').read_text(),fn);owned=caller((base/'caller.sv').read_text(),fn)
    old=(BUILD/'cp-read-owner-verilator-v1-20261025/candidate.sv').read_text()
    for n in ['dorado_r_front_control_step','dorado_r_front_pure_step']:
        assert module(candidate,n)==module(old,n)
    original_asserts=event_assertions((BUILD/'processor-original-trace-v2-20261024/inputs/tb_r_front_local.sv').read_text())
    for name in ['single-icarus-v3','single-verilator-v3','payload-icarus-v1','payload-verilator-v1',
                 'all-icarus-v1','all-verilator-v1','collisions-verilator-v1','ram64-verilator-v1']:
        d=BUILD/f'cp-paired-read-{name}-20261025';s=read(d/'run-state.json')
        assert s['status']=='PASS_COMBINED_CP_PACKET_SINGLE_FALLBACK'
        assert (d/'candidate.sv').read_text()==candidate and (d/'caller.sv').read_text()==owned
        assert not original_asserts-event_assertions((d/'inputs/tb_r_front_local.sv').read_text())
        assert s['full_trace_byte_identical_to_original_checked_fixture']
        if name.startswith('payload'):assert any(v[1]=='1'for v in s['payload_probes'])
    for name,marker in [('bad-payload','PAYLOAD_GATE accepted invalid payload'),
                        ('early-ram','CP unready request reached the original SRAM owner')]:
        d=BUILD/f'cp-paired-read-{name}-icarus-v1-20261025';s=read(d/'run-state.json')
        assert s['status']=='PASS_COMBINED_CP_PACKET_COUNTEREXAMPLE'and marker in(d/'test.log').read_text()
    full=BUILD/'cp-paired-read-replay-verilator-v2-20261025';f=read(full/'run-state.json')
    assert f['status']=='PASS_COMBINED_CP_COHERENT_PACKETS'
    assert f['summary']=='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
    assert f['geometry_independent_input_bits']==376 and f['geometry_cases']==378
    words=BUILD/'cp-paired-read-words-replay-verilator-v1-20261025';w=read(words/'run-state.json')
    assert w['status']=='PASS_COMBINED_CP_COHERENT_PACKETS'and w['staged_reads']==1
    assert w['summary']=='transactions=12481 commits=12387 pairs=94 candidates=94 fallbacks=0 paired_writes=0 forwarded_bytes=0 Qs=844 RAM=9216 results=2470 clocks=100'
    for d in [full,words]:
        assert (d/'candidate.sv').read_text()==candidate and (d/'caller.sv').read_text()==owned
        assert len(re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',(d/'test.log').read_text(),re.M))==100
    negatives={}
    for name,marker in [('drop-forward','PACKET second result mismatch record=203'),('early-ram','PACKET invalid geometry committed bit=0')]:
        d=BUILD/f'cp-paired-read-replay-{name}-icarus-v1-20261025';s=read(d/'run-state.json')
        assert s['status']=='PASS_COMBINED_CP_PACKET_COUNTEREXAMPLE'and marker in(d/'test.log').read_text()
        negatives[name]=[v for v in(d/'test.log').read_text().splitlines()if 'PACKET_DIFF'in v or 'FATAL'in v]
    d=BUILD/'cp-paired-read-structure-v1-20261025';s=read(d/'run-state.json')
    assert s['status']=='PASS_EXTERNAL_PACKET_ONE_SRAM_AND_DUPLICATE_CONTROL'
    for row in s['cases']:
        case=d/('duplicate-control'if row['bad']else'positive')
        assert row['passed']and row['returncode']==0 and structure(case/'hierarchy.xml')==row['structure']
        assert sha(case/'hierarchy.xml')==row['XML_sha256']
    # All actual output bits/RAM are unchanged by physical CP source closure.
    balanced=BUILD/'cp-original-program-balanced-verilator-v2-20261025'
    phase=BUILD/'cp-original-program-phase3000-verilator-v1-20261025'
    for d in [balanced,phase]:
        s=read(d/'run-state.json');assert s['status']=='PASS_ORIGINAL_FULL_PROGRAM_WITH_CP_SOURCE_CLOSURE'
        assert s['original_full_stimulus_program']and s['original_final_coverage_assertions_retained']
        assert not original_asserts-event_assertions((d/'inputs/tb_r_front_local.sv').read_text())
    for n in ['transactions.hex','initial.hex']:assert sha(balanced/n)==sha(phase/n)
    a=(balanced/'transactions.hex').read_text().splitlines()
    b=(BUILD/'processor-original-trace-v2-20261024/transactions.hex').read_text().splitlines()
    assert len(a)==len(b)==14884;mask=(1<<(2470+9216))-1
    assert all((int(x,16)&mask)==(int(y,16)&mask)for x,y in zip(a,b))
    raw=(phase/'timed/run_control_original_ff.sv').read_text();ph=module(raw,'original_SE10231')
    assert set(re.findall(r'<=#\((\d+)\)',ph))=={'3000'}
    assert set(re.findall(r'<=#\((\d+)\)',raw.replace(ph,'')))=={'400'}
    # Strict faster-control full runs fail in BOTH engines. Do not repair the
    # original RAM or substitute its illegal overlap for a legal write rule.
    for n in ['cp-original-program-verilator-v1-20261025','cp-original-program-icarus-trace-v2-20261025',
              'cp-original-program-icarus-arrival-v3-20261025']:
        assert 'R front resolved source mismatch check=761'in(BUILD/n/'test.log').read_text()
    trace=BUILD/'cp-original-program-icarus-arrival-v3-20261025/test.log'
    assert 'WE_n=0 address=7'in trace.read_text()and 'file=f0000000f0000000'in trace.read_text()
    probes=[]
    for d in directories:
        if not d.name.startswith('rbase-edge-'):continue
        s=read(d/'run-state.json')
        if not s['restored_original_fixture']:continue
        if d.name.startswith('rbase-edge-original-'):assert s['reproduced_source_mismatch']
        else:assert s['original_single_event_passed']
        probes.append(dict(directory=d.name,phase_ps=s.get('phase_delay_ps',s.get('phase_delay_unchanged_ps')),
                           FF_ps=s['FF_delay_ps'],single_event_passed=s['original_single_event_passed']))
    # The original explicitly legal primitive remains unchanged.
    primitive=read(BUILD/'rbase-window-reviewed-v1-20261023/results.json')
    assert sha(ROOT/'verilog/rtl/dorado_rbase_window_step.sv')==primitive['helper_sha256']
    medium=BUILD/'cedar-work-native-hints.pack'
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    failed=BUILD/'combined-cp-paired-read-review-v1-20261025'
    target=out/failed.name;target.mkdir()
    for name in ['failure.json','failed-review-script.py']:
        (target/name).write_bytes((failed/name).read_bytes())
    result=dict(status='REVIEWED_COMBINED_CP_READ_AND_PAIR_PATH_WITH_PENDING_PEERS',scope=__doc__,runs=runs,pending_peers=pending,
                same_corrected_CP_processors_all_invocations=True,original2470_bit_result_layout_preserved=True,
                full_coherent_replay_verilator=f['summary'],paired_write_bytes_forwarded=238,
                CP_word_mixed_pair_and_read_replay_verilator=w['summary'],mixed_history_staged_reads=1,
                actual_one_SRAM_bits=9216,stored_owner_bits=828,external_native_R_bits=16,context_bits=402,
                geometry_independent_source_bits=376,geometry_cases=378,geometry_dependent_aliases_regenerated=True,
                original_result_owner_RAM_and_clock_assertions_unmasked=True,actual_negative_failures=negatives,
                payload_barrier_both_engines=True,whole_machine_payload_validity_remains_upstream=True,
                full_program_faster_control_profile_counterexample=dict(check=761,state_bits=[668,669,670,671],source_bits=[156,157,158,159],
                    address_arrives_before_write_close_ns=0.5,raw_RAM_writes_unwanted_entry=7),
                RBase_arrival_probes=probes,RBase_legal_closing_rule_unchanged=True,
                phase3000_full_trace_identical_to_balanced=True,full_results_and_RAM_identical_to_prior_original_trace=True,
                phase3000_observed_edge_address_hold_ns=1.25,
                phase_delay_sweep_is_not_a_part_spec_or_full_arrival_theorem=True,
                native_source_clock_and_visible_R_hookup_pending=True,CP_device_event_retention_pending=True,
                full_IFU_Memory_all_device_and_unknown_arrival_gates_pending=True,
                physical15ns_or60ns_qualified=False,new_RBF_ready=False,hardware_changed=False,
                protected_medium_unchanged=True,measured_hardware_speed_gain=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='combined-cp-paired-read',extra_tools=TOOLS)
    print(r['status'])
