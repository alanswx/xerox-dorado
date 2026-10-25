#!/usr/bin/env python3
"""Review descriptor safety, real owner/SRAM barriers and bounded processor runs.

Retain every failure and original recording. Recover the early fixture record
from the first transaction; validate it against the independently checked cold
fixture. This is not full native cell/clock integration or a speed qualification.
"""
import argparse
import hashlib
from collections import Counter
import json
from pathlib import Path
import re

from generate_cp_read_caller import generate
from prove_cp_read_owner import instrument_memory,instrument_caller,TOP
from prove_cp_interior_tags import TOP as TAG_TOP
from review_baseboard_clock_consumer import verify
from review_cp_interior_stage import event_assertions,generated_candidate
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
PASS='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE'
TOOLS=['generate_cp_read_caller.py','prove_cp_read_owner.py','prove_cp_interior_tags.py',
       'test_cp_read_owner.py','test_cp_interior_tags.py','test_r_front_cp_source.py','review_cp_read_caller.py']


def read(p):return json.loads(p.read_text())


def wave(row):
    data=iter(row.get('data',[]));v=None;out=[]
    for c in row['wave']:
        if c=='.':pass
        elif c in '01':v=int(c)
        elif c in '=23456789':
            q=next(data,'');v=int(q,2)if q and set(q)<={'0','1'}else None
        else:v=None
        out.append(v)
    return out


def recover_fixture(d,baseline,out):
    raw=(d/'initial.hex').read_text().strip();initial_ram=int(raw[-2304:],16)
    lines=(d/'transactions.hex').read_text().splitlines()
    first_owner=(int(lines[0],16)>>12490)&((1<<844)-1)
    expected=(baseline/'initial.hex').read_bytes()
    actual=f'{first_owner:0211x}{initial_ram:02304x}\n'.encode()
    assert actual==expected,(d.name,'first committed-input owner/RAM differs from cold fixture')
    target=out/d.name;target.mkdir(parents=True,exist_ok=True)
    (target/'initial-recovered.hex').write_bytes(actual)
    pattern=r'^FULL_CP_FIXTURE_BOUNDARY fixture=(\d+) record=(\d+) owner=([0-9a-fx]+) memory=([0-9a-fx]+)$'
    boundaries=re.findall(pattern,(d/'test.log').read_text(),re.M)
    original=re.findall(pattern,(baseline/'test.log').read_text(),re.M)
    assert len(boundaries)==len(original)
    corrected=[]
    for row,old in zip(boundaries,original):
        n,k,q,m=row;old_n,old_k,old_q,old_m=old
        owner=(int(lines[int(k)],16)>>12490)&((1<<844)-1)
        assert (n,k,m)==(old_n,old_k,old_m) and owner==int(old_q,16)
        corrected.append(dict(fixture=int(n),record=int(k),owner=f'{owner:0211x}',memory_sha256=hashlib.sha256(bytes.fromhex(m)).hexdigest(),baseline_log_sha256=sha(baseline/'test.log'),
                              original_owner_record_wrong=q!=f'{owner:0211x}'))
    (target/'fixture-boundaries-recovered.json').write_text(json.dumps(corrected,indent=2)+'\n')
    return dict(directory=d.name,original_initial_sha256=sha(d/'initial.hex'),recovered_sha256=sha(target/'initial-recovered.hex'),
                original_initial_record_wrong=(raw+'\n').encode()!=actual,recovered_fixture_boundaries=len(corrected),
                original_wrong_boundary_count=sum(v['original_owner_record_wrong']for v in corrected),
                recovery_uses_actual_pre_event_owner=True,validated_against_independent_cold_fixture=True)


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    cp=BUILD/'native-cp-source-v1-20261024';m=read(cp/'manifest.json');verify(cp,m['files'])
    source=BUILD/'supported-fall-guard-owned-verilator-v1-20261024'
    caller=generate((source/'caller.sv').read_text(),(cp/'functions.sv').read_text())
    actual=generated_candidate(True)
    original_asserts=event_assertions((BUILD/'processor-original-trace-v2-20261024/inputs/tb_r_front_local.sv').read_text())
    dirs=[];records=[]
    for pattern in ['cp-tagged-*-20261025','cp-read-owner-*-20261025','cp-interior-tags-proof-*-20261025']:
        for d in sorted(BUILD.glob(pattern)):
            if d in dirs:continue
            s=read(d/'run-state.json');assert s['status']not in ['PREPARING','RUNNING'],d.name
            verify(d,s.get('files',{}))
            for r in s.get('source_files',[]):assert sha(d/r['path'])==r['output_sha256']
            for a in s.get('actions',[]):
                assert 'returncode'in a and sha(d/(a['label']+'.log'))==a['log_sha256']
            if s.get('source_files'):
                assert (d/'candidate.sv').read_text()==actual
                tb=(d/'inputs/tb_r_front_local.sv').read_text()
                assert not original_asserts-event_assertions(tb),d.name
            records.append(dict(directory=d.name,status=s['status'],state_sha256=sha(d/'run-state.json')));dirs.append(d)
    def positive(name):
        d=BUILD/name;s=read(d/'run-state.json');assert s['status']==PASS
        assert len(s['actions'])==2 and all(a['returncode']==0 for a in s['actions'])
        assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',(d/'test.log').read_text(),re.M))==100
        assert not s['native_integrated'] and not s['physical_timing_qualified']
        return d,s,(d/'test.log').read_text()
    # The existing staged processor is unchanged; tags/controller/publication
    # glue must not change any original transaction result.
    recoveries=[];groups=[]
    for mode,marker,count in [('all','checks=12481 words=4096',12481),('collisions','checks=17089 collisions=128',17089),('ram64','checks=12737 RAM_cases=64',12737)]:
        peers=[]
        for e in ['icarus','verilator']:
            d,s,log=positive(f'cp-read-owner-{mode}-{e}-v1-20261025')
            assert marker in log and s['synthesized_read_owner']and s['untrusted_write_probe']
            assert not s['ordinary_paired_path_integrated']and s['external16_visible_R_testbench_model']
            assert (d/'dorado_cp_read_caller.sv').read_text()==caller
            baseline=BUILD/f'cp-interior-stage-{mode}-{e}-v2-20261025'
            assert sha(d/'transactions.hex')==sha(baseline/'transactions.hex')
            recoveries.append(recover_fixture(d,baseline,out));peers.append(d)
        assert sha(peers[0]/'transactions.hex')==sha(peers[1]/'transactions.hex')
        groups.append(dict(mode=mode,checks=count,both_engines_pass=True,full_trace_byte_identical=True))
    for e in ['icarus','verilator']:
        d,s,log=positive(f'cp-read-owner-fixture-{e}-v2-20261025')
        assert '#1; // Settle the held-wire view'in(d/'inputs/tb_r_front_local.sv').read_text()
        assert sha(d/'initial.hex')==sha(BUILD/'cp-interior-stage-icarus-v3-20261025/initial.hex')
    reload,s,log=positive('cp-read-owner-fixture-reload-icarus-v2-20261025')
    assert s['ram_sweep_cases']==1
    row=re.search(r'^FULL_CP_FIXTURE_BOUNDARY fixture=1 record=(\d+) owner=(\w+) memory=(\w+)$',log,re.M)
    first=(int((reload/'transactions.hex').read_text().splitlines()[int(row[1])],16)>>12490)&((1<<844)-1)
    assert first==int(row[2],16),'new reload boundary recorded before its wires settled'
    # Tag tests exercise every descriptor bit in both waiting and DONE states.
    for e in ['icarus','verilator']:
        d,s,log=positive(f'cp-tagged-probe-{e}-v1-20261025')
        assert 'INTERIOR_TAG_FULL_PROCESSOR_PASS changes=144 READ=72 DONE=72'in log
    bad=BUILD/'cp-tagged-ignore-ready-icarus-v1-20261025'
    assert read(bad/'run-state.json')['status']!=PASS
    assert 'stale request accepted epoch=1 bit=0'in(bad/'test.log').read_text()
    for name,marker in [('early-publish','READ_OWNER pending read changed owner/context/RAM'),('early-ram','CP unready request reached the original SRAM owner check=0')]:
        d=BUILD/f'cp-read-owner-{name}-icarus-v1-20261025';s=read(d/'run-state.json')
        assert s['status']!=PASS and marker in(d/'test.log').read_text()
    early=(BUILD/'cp-read-owner-early-publish-icarus-v1-20261025/test.log').read_text()
    assert 'READ_OWNER_EARLY_Q bit=785'in early and 'READ_OWNER_EARLY_Q bit=838'in early
    tag=BUILD/'cp-interior-tags-proof-v2-20261025';t=read(tag/'run-state.json');verify(tag,t['files'])
    assert t['status']=='PASS_INTERIOR_DESCRIPTOR_TAGS_AND_NEGATIVES'and t['no_assumptions']and t['negative_target_actual_ready_or_valid']
    assert t['initial_data_and_tag_bits_unconstrained']==108 and len(t['cases'])==7
    for c in t['cases']:
        d=tag/(c['mutation']or'contracts');verify(d,c['files'])
        assert c['passed']and c['returncode']==0 and sha(d/'proof.log')==c['log_sha256']
        if c['mutation']:
            rows={r['name']:wave(r)for r in read(d/'counterexample.json')['signal']}
            assert any(v==0 and(r==1 or q==1)for v,r,q in zip(rows['stale_data_blocked'],rows['ready'],rows['valid']))
    failed_one=BUILD/'cp-read-owner-proof-v1-20261025';failed_two=BUILD/'cp-read-owner-proof-v2-20261025'
    assert 'wasmtime._trap.Trap'in(failed_one/'contracts/proof.log').read_text()
    stopped=read(failed_two/'cpu-limit-stop.json')
    assert read(failed_two/'run-state.json')['cases'][0]['returncode']==-15 and stopped['pid']==18639
    proof=BUILD/'cp-read-owner-proof-v3-20261025';p=read(proof/'run-state.json');verify(proof,p['files'])
    assert p['status']=='PASS_CP_READ_OWNER_AND_SRAM_BARRIER'and p['no_assumptions']and p['architectural_powerup_unconstrained']
    assert (p['architectural_owner_bits'],p['stored_context_bits'],p['SRAM_bits'])==(828,402,9216)
    model=proof/'contracts'
    assert (model/'caller.sv').read_text()==instrument_caller(caller)
    memory_source=BUILD/'cp-read-owner-icarus-v1-20261025/inputs/verilog/rtl/dorado_r_file_ports.sv'
    assert (model/'memory.sv').read_text()==instrument_memory(memory_source.read_text())
    assert (model/'contract.sv').read_text()==TOP.replace('FUNCTIONS',(cp/'functions.sv').read_text())
    counterexamples={}
    for c in p['cases']:
        d=proof/(c['mutation']or'contracts');verify(d,c['files'])
        assert c['passed']and c['returncode']==0 and sha(d/'proof.log')==c['log_sha256']
        assert '-set-init-zero'not in(d/'proof.ys').read_text()
        if c['mutation']:
            rows={r['name']:wave(r)for r in read(d/'counterexample.json')['signal']}
            now,old,holds=('owned','previous_owned','owner_holds')if c['mutation']=='early-owner'else('memory','previous_memory','memory_holds')
            events=[]
            for k,(a,b,h,commit)in enumerate(zip(rows[now],rows[old],rows[holds],rows['previous_commit'])):
                if h==0 and commit==0 and a is not None and b is not None:
                    delta=a^b;bits=[i for i in range(828 if now=='owned'else 9216)if(delta>>i)&1];assert bits
                    events.append(dict(step=k,changed_bits=bits))
            assert events;counterexamples[c['mutation']]=events
    for d in dirs:retain(d,out/d.name)
    # Re-add the derived records after retaining originals; no historical file
    # is overwritten. Explicitly retain the failed monolithic encodings too.
    for row in recoveries:
        assert (out/row['directory']/'initial-recovered.hex').exists()
    native=BUILD/'control-short-native-jumper-boot-v2-20261024';ns=read(native/'run-state.json')
    log=(native/'test.log').read_text();progress=re.findall(r'^NATIVE_(?:CP_PROGRESS|TRAFFIC_PROGRESS|PROGRESS).*$',log,re.M)[-3:]
    result=dict(status='REVIEWED_TAGGED_READ_CALLER_AND_PUBLICATION_BARRIER',scope=__doc__,groups=groups,runs=records,
                descriptor_bits=72,descriptor_full_processor_changes_per_engine=144,tag_proof_no_assumptions=True,
                tag_negative_cases_break_actual_ready_or_valid=True,actual_owner_bits=828,external_visible_R_bits=16,context_bits=402,
                same_one9216_bit_SRAM=True,full_SRAM_owner_context_and_read_snapshot_contracts_inductively_proven=True,
                proof_architectural_powerup_unconstrained=True,proof_processor_next_payload_abstracted=True,
                proof_formal_probe_not_a_physical_read_port=True,formal_real_state_corruptions=counterexamples,
                simulation_early_publication_changes_real_Q=[785,838],ordinary_need_false_adds_no_read_cycle=True,
                failed_formal_encodings_retained=['v1_WASM_exception','v2_declared_CPU_limit_exceeded_and_verified_child_stopped'],
                actual_native_publication_seam_reused=True,actual_CP_source_closed_context_publication=True,
                initial_record_bug='Earlier caller fixture header/boundaries sampled wires before a delta cycle; actual per-event traces start from the correct fixture. Originals are retained, recoveries use actual transaction owners and checked baseline RAM. Fixed fixture task settles1ns.',
                fixture_recoveries=recoveries,fixed_initial_and_reload_recording_validated=True,
                old_legacy_caller_testbench_flag_not_used_as_qualification=True,
                caller_publication_and_SRAM_barrier_in_synthesizable_RTL=True,
                actual_native_visible_R_cells_and_clock_hookup_qualified=False,ordinary_paired_path_qualified=False,
                full_original16_R_arbitrary_arrival_qualification_pending=True,unknown_and_noncanonical_paths_pending=True,
                native_boot_observation=dict(status=ns['status'],latest_complete_log_lines=progress,original5B_host_bound_unchanged=True,not_a_boot_pass=True),
                physical15ns_or60ns_qualified=False,new_RBF_ready=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='tagged-read-caller',extra_tools=TOOLS)
    print(r['status'])
