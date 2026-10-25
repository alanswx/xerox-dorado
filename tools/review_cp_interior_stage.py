#!/usr/bin/env python3
"""Retain the staged one-port correction, bounded passes and outstanding gates.

Full original assertions remain. A live peer is observed by its actual process,
not inferred from a lock/state file. This does not select a native core or
qualify arbitrary owners, unknown contexts, other arrival orders or timing.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import subprocess
import time

from generate_cp_block_capture import rewrite as block
from generate_cp_prblock_capture import rewrite as prblock
from generate_cp_interior_read import rewrite as interior,sequential_stage_bench,ram_sweep_bench
from generate_r_front_cp_source_reference import generate
import generate_shared_control_capture as shared
from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_source import actual_profile
from test_r_front_cp_source import source_top,bench
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
PASS='PASS_ORIGINAL_FULL_PROCESSOR_CP_SOURCE'
TOOLS=['generate_cp_interior_read.py','prove_cp_interior_stage.py','review_cp_interior_stage.py',
       'test_r_front_cp_source.py','generate_cp_prblock_capture.py','generate_shared_control_capture.py']


def read(p):return json.loads(p.read_text())


def event_assertions(raw):
    raw=raw[raw.index(' task automatic event_step('):];raw=raw[:raw.index('\n endtask')]
    return Counter(re.findall(r'\$fatal\(1,"([^"\n]+)"',raw))


def generated_candidate(enabled):
    raw=(BUILD/'supported-fall-guard-owned-verilator-v1-20261024/candidate.sv').read_text()
    raw=prblock(block(raw,True),True,True)
    if enabled:raw=interior(raw)
    return source_top(shared.kernel(raw,share_native=True),(BUILD/'native-cp-source-v1-20261024/functions.sv').read_text())+'\n'+shared.control(raw)


def review(out,require_words=False):
    assert not out.exists();out.mkdir(parents=True)
    ref=out/'independent-reference';r=generate(ROOT,ref)
    assert (r['physical_bits'],r['ram_bits'],r['clock_channels'])==(844,9216,100)
    actual=generated_candidate(True);disabled=generated_candidate(False)
    assert disabled==(BUILD/'r-front-cp-source-ready0-later-prblock-icarus-v5-20261024/candidate.sv').read_text()
    original_tb=(BUILD/'processor-original-trace-v2-20261024/inputs/tb_r_front_local.sv').read_text()
    required_assertions=event_assertions(original_tb)
    records=[];dirs=[];snapshots={};live=[]
    for pattern in ['cp-interior-read-*-20261024','cp-interior-stage-*-20261024','cp-interior-stage-*-20261025']:
        for d in sorted(BUILD.glob(pattern)):
            if d in dirs:continue
            f=d/'run-state.json';data=f.read_bytes();s=json.loads(data)
            verify(d,s.get('files',{}))
            for row in s.get('source_files',[]):assert sha(d/row['path'])==row['output_sha256']
            actions=s.get('actions',[])
            for a in actions:
                if 'returncode'in a:assert sha(d/(a['label']+'.log'))==a['log_sha256']
            if s['status']=='RUNNING':
                assert len(actions)==2 and actions[0]['returncode']==0 and 'returncode'not in actions[-1]
                a=actions[-1];ps=subprocess.check_output(['ps','-p',str(a['pid']),'-o','pid=,etime=,stat=,command='],text=True).strip()
                assert str(d/'test.vvp')in ps
                live.append(dict(directory=d.name,pid=a['pid'],ps=ps,observed=time.time(),source_sha256=sha(d/'candidate.sv'),terminal_pass_required=True))
                snapshots[d.name]=data
            elif 'proof'not in d.name:
                assert s['status']not in ['PREPARING','RUNNING'] and all('returncode'in a for a in actions)
            if s.get('source_files'):
                assert actual_profile(d)==[1750,400,1250]
                assert s['all_original_event_assertions_retained']
                assert s['physical_Q_bits']==844 and s['physical_RAM_bits']==9216 and s['clock_channels']==100
                for p in ref.glob('*.sv'):assert p.read_bytes()==(d/'reference'/p.name).read_bytes()
                tb=(d/'inputs/tb_r_front_local.sv').read_text()
                assert not required_assertions-event_assertions(tb),(d.name,required_assertions-event_assertions(tb))
            if s['status']==PASS:
                assert len(actions)==2 and all(a['returncode']==0 for a in actions)
                log=(d/'test.log').read_text();assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',log,re.M))==100
            records.append(dict(directory=d.name,status=s['status'],state_sha256=sha(f),
                                source_sha256=sha(d/'candidate.sv')if(d/'candidate.sv').exists()else None))
            dirs.append(d)
    def positive(name,marker):
        d=BUILD/name;s=read(d/'run-state.json');assert s['status']==PASS
        assert (d/'candidate.sv').read_text()==actual,name
        assert s['synthesizable_interior_sequencer']
        assert s['native_caller_integration_pending'] and s['initial_ready_fixture']=='0000'
        assert not s['matched_intermediate_port_data'] and not s['equal_RM_STK_fixture_data']
        tb=(d/'inputs/tb_r_front_local.sv').read_text()
        assert tb.count(' dorado_r_file_ports files(')==tb.count(' dorado_r_interior_read_stage interior_stage(')==1
        assert 'advance=0;stage_start=1;host_clk=1;'in tb and 'stored_q=expected;cb=next_root;'in tb
        assert sha(d/'verilog/rtl/dorado_r_interior_read_stage.sv')==sha(ROOT/'verilog/rtl/dorado_r_interior_read_stage.sv')
        log=(d/'test.log').read_text();assert marker in log
        # Independently regenerate the exact bench, including every original
        # check and the explicit stalls/one-port read. Only output paths differ.
        parent=bench(original_tb,d,s['reproduction_only'],s['root_CP_collision_program'],True,s['passive_g16_arrival_trace'],False,1,False,False)
        if s.get('ram_sweep_cases'):parent=ram_sweep_bench(parent,s['ram_sweep_cases'])
        assert sequential_stage_bench(parent)==tb,name
        return d,s,log
    focused=[]
    for e in ['icarus','verilator']:
        d,s,log=positive(f'cp-interior-stage-{e}-v3-20261025','INTERIOR_READ_COMPONENT_PASS reads=1')
        assert 'before=046f4 capture=2632f'in log
        focused.append(dict(engine=e,directory=d.name,checks=196,interior_reads=1))
    a,_,al=positive('cp-interior-stage-all-verilator-v2-20261025','checks=12481 words=4096 Qs=844 RAM=9216 clocks=100 sets=3ffff')
    c,cs,cl=positive('cp-interior-stage-collisions-verilator-v2-20261025','checks=17089 collisions=128 Qs=844 RAM=9216 clocks=100 sets=3ffff')
    m,ms,ml=positive('cp-interior-stage-ram64-verilator-v2-20261025','checks=12737 RAM_cases=64 Qs=844 RAM=9216 clocks=100 sets=20000')
    assert cs['trace_context_discontinuities']==128 and ms['trace_context_discontinuities']==64
    assert len(re.findall(r'^FULL_CP_FIXTURE_BOUNDARY ',cl,re.M))==128
    assert len(re.findall(r'^FULL_CP_FIXTURE_BOUNDARY ',ml,re.M))==64
    assert len(re.findall(r'^INTERIOR_READ check=',cl,re.M))==3
    assert len(re.findall(r'^INTERIOR_READ check=',ml,re.M))==64
    negatives=[]
    for n,mutation in [('stale','stale-data'),('no-valid','missing-valid')]:
        d=BUILD/f'cp-interior-stage-{n}-icarus-v3-20261025';s=read(d/'run-state.json');log=(d/'test.log').read_text()
        assert s['interior_mutation']==mutation and s['status']!=PASS and s['actions'][-1]['returncode']!=0
        assert (d/'candidate.sv').read_text()==actual
        assert 'operand gate output mismatch check=194'in log
        negatives.append(dict(mutation=mutation,directory=d.name,real_original_output_failure=True))
    # Preserve why the rising-only version was insufficient; no narrowing of
    # the continuous CP-word requirement to its previously passing collisions.
    failed=BUILD/'cp-interior-stage-all-verilator-v1-20261024'
    assert read(failed/'run-state.json')['status']!=PASS and 'output mismatch check=242'in(failed/'test.log').read_text()
    proof=BUILD/'cp-interior-stage-proof-v4-20261025';p=read(proof/'run-state.json');verify(proof,p['files'])
    assert p['status']=='PASS_INTERIOR_STAGE_CONTRACTS_AND_NEGATIVES' and p['source_sha256']==sha(ROOT/'verilog/rtl/dorado_r_interior_read_stage.sv')
    assert p['no_assumptions'] and p['initial_buffer_bits_unconstrained']==36 and len(p['cases'])==4
    for v in p['cases']:
        d=proof/(v['mutation']or'contracts');verify(d,v['files'])
        assert v['passed']and v['returncode']==0 and sha(d/'proof.log')==v['log_sha256']
        assert '-set-init-zero'not in(d/'proof.ys').read_text()
        if v['mutation']:
            names={v['name']for v in read(d/'counterexample.json')['signal']}
            assert {'ready','valid','active','reading','before_data','capture_data'}<=names
    all_peer=BUILD/'cp-interior-stage-all-icarus-v2-20261025';s=json.loads(snapshots[all_peer.name])if all_peer.name in snapshots else read(all_peer/'run-state.json')
    assert (all_peer/'candidate.sv').read_text()==actual
    assert s['status']in['RUNNING',PASS]
    if s['status']=='RUNNING':assert any(v['directory']==all_peer.name for v in live)
    if require_words:
        _,_,il=positive(all_peer.name,'checks=12481 words=4096 Qs=844 RAM=9216 clocks=100 sets=3ffff')
        for name in ['candidate.sv','initial.hex','transactions.hex']:
            assert sha(all_peer/name)==sha(a/name),name
    old_i=BUILD/'cp-interior-stage-collisions-icarus-v1-20261024';old_v=BUILD/'cp-interior-stage-collisions-verilator-v1-20261024'
    older_collision_terminal=read(old_i/'run-state.json')['status']==PASS
    if older_collision_terminal:
        assert 'checks=17089 collisions=128 Qs=844 RAM=9216 clocks=100 sets=3ffff'in(old_i/'test.log').read_text()
        for name in ['candidate.sv','initial.hex','transactions.hex']:assert sha(old_i/name)==sha(old_v/name)
    native=BUILD/'control-short-native-jumper-read-v4-20261025';n=read(native/'result.json')
    assert n['read_only_guest_inspection']and n['pid']==69736 and 'detached'in(native/'lldb.log').read_text().lower()
    assert n['riot_ram']['timer100ms_word']==26 and n['riot_ram']['EndOfDelay_word']==30;dirs.append(native)
    medium=BUILD/'cedar-work-native-hints.pack';assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    for d in dirs:
        retain(d,out/d.name)
        if d.name in snapshots:(out/d.name/'run-state.json').write_bytes(snapshots[d.name])
    result=dict(status='REVIEWED_SINGLE_PORT_CP_WORDS_WITH_PENDING_NATIVE_EPOCH_GATES'if require_words else'STAGED_SINGLE_PORT_CP_CAPTURE_WITH_PENDING_GATES',scope=__doc__,
                current_Icarus_Verilator_full_word_trace_byte_identical=require_words,
                older_rising_collision_peer_terminal=older_collision_terminal,
                older_rising_collision_trace_byte_identical=older_collision_terminal,
                older_rising_candidate_not_the_extended_current_candidate=True,
                focused_both_engines=focused,Verilator_continuous_words=4096,continuous_checks=12481,
                Verilator_fresh_collision_fixtures=128,collision_checks=17089,collision_interior_reads=3,
                Verilator_RAM_fixtures=64,RAM_checks=12737,RAM_interior_reads=64,
                RAM_high_byte_edge_pairs=64,other_cells_independently_seeded=True,
                original_844_Q_9216_RAM_100_clock_event_assertions_retained=True,
                original_event_fatal_checks_preserved=sum(required_assertions.values()),
                only_one_DUT_physical_SRAM_read_port_per_bank=True,extra_architectural_owner_bits=0,
                stage_auxiliary_data_bits=36,stage_auxiliary_phase_bits=2,
                controller_ack_hold_reset_contracts_inductively_proven=True,controller_initial_buffers_unconstrained=True,
                controller_no_protocol_assumptions=True,controller_negative_cases=['early-ready','stale-capture','ordinary-stall'],
                processor_real_output_negatives=negatives,before_physical_observations_remain_original=True,
                data_and_parity_from_same_interior_read=True,ordinary_need_false_requires_no_extra_cycle=True,
                disabled_generated_candidate_byte_identical=True,stable_LOW_failure_preserved_and_fixed=True,
                runs=records,verified_live_peers=live,full_current_Icarus_words_pending=s['status']=='RUNNING',
                fresh_native_timer_observation=dict(pid=69736,host=n['counters']['host_ticks'],guest_ticks=n['counters']['guest_ticks'],timer100ms=26,EndOfDelay=30,debugger_detached=True),
                native_timer_ticks_not_microinstructions=True,original_native_5B_host_bound_unchanged=True,
                native_caller_freeze_publication_and_barrier_integration_pending=True,
                read_controller_is_synthesizable_RTL=True,caller_is_still_testbench=True,
                historical_caller_flag_corrected="Older states set caller_testbench_only false when the RTL controller was selected; caller start/ack/release/freeze/publication still come from the testbench. Compiled files and old states are preserved.",
                unknown_context_and_arbitrary_owner_arrival_qualification_pending=True,
                original16_R_interior_edges_and_IFU_Memory_pending=True,
                nonvacuous_full_processor_TPC_negative_pending=True,physical_timing_qualified=False,
                native_integrated=False,new_RBF_ready=False,hardware_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','archive']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--label',default='single-port-cp-capture-staging')
    p.add_argument('--require-words',action='store_true')
    a=p.parse_args();r=review(a.output.resolve(),a.require_words)
    append_archive(a.archive.resolve(),a.output.resolve(),[],label=a.label,extra_tools=TOOLS)
    print(r['status'])
