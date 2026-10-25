#!/usr/bin/env python3
"""Preserve the directed full-processor capture fix and boot FIFO diagnosis.

Bounded digital timing-profile evidence only. The j19 pulse is1.35ns in this
illustrative model. Original MC10231 data sheets specify setup/hold and toggle
frequency, but no independent minimum clock LOW width in their tables; this
does not establish physical acceptance of that pulse. Native clocks, all-board
phase/event closure, unknown arrivals and SAME-fixture timing remain open.
"""
import argparse
import json
from pathlib import Path
import re

from generate_j19_capture import rewrite
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_cp_interior_stage import event_assertions
from review_cp_paired_read import observed
from review_late_hold_and_input_reset import retain
from review_native_r_capture import clock_outcomes
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_j19_capture.py','probe_full_tpc_capture.py','review_j19_capture.py',
       'analyze_native_boot_read.py','inspect_native_baseboard_process.py',
       'replay_cp_paired_read.py']


def read(path):return json.loads(path.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    base=BUILD/'native-R-single-verilator-v1-20261025'
    adapter='adapters/dorado_local_r_front_step.sv'
    original=(base/adapter).read_text();changed=rewrite(original)
    original_checks=event_assertions((base/'inputs/tb_r_front_local.sv').read_text())
    directed=[];rows=[];pending=[]
    for d in sorted(BUILD.glob('full-tpc-*-20261025')):
        state,row=observed(d);rows.append(row);retain(d,out/d.name)
    for engine in ['icarus','verilator']:
        positive=BUILD/f'full-tpc-j19-epochs-{engine}-v2-20261025'
        negative=BUILD/f'full-tpc-j19-epochs-late-{engine}-v2-20261025'
        p=read(positive/'run-state.json');n=read(negative/'run-state.json')
        assert p['returncode']==0 and p['original_program_passed']
        assert n['returncode']!=0 and not n['original_program_passed']
        for d in [positive,negative]:
            assert (d/adapter).read_text()==changed
            assert not original_checks-event_assertions((d/'inputs/tb_r_front_local.sv').read_text())
            assert read(d/'run-state.json')['original_staged_read_case_retained_first']
        log=(positive/'test.log').read_text()
        assert 'FULL_PROCESSOR_CP_BLOCK_PASS checks=204 Qs=844 RAM=9216 clocks=100'in log
        assert 'READ_OWNER_FULL_PROCESSOR_PASS reads=1'in log
        assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',log,re.M))==100
        bad=(negative/'test.log').read_text()
        assert 'TPC_STATE_DIFF check=202 bit=74 predicted=0 actual=1'in bad
        assert 'R_FRONT_DIFF bit=74 old=0 expected=0 original=1'in bad
        assert 'R front state mismatch check=202'in bad
        assert re.findall(r'^R_FRONT_DIFF bit=(\d+)',bad,re.M)==['74']
        candidate=(positive/'candidate.sv').read_text()
        changed_candidate=(negative/'candidate.sv').read_text()
        for family in ['control','capture']:
            before='module dorado_'+family+'_hold_atomic_step #(parameter integer WAVES=24,parameter integer CP_TPC_BLOCK_EARLY=1)('
            after=before.replace('CP_TPC_BLOCK_EARLY=1','CP_TPC_BLOCK_EARLY=0')
            assert changed_candidate.count(after)==1
            changed_candidate=changed_candidate.replace(after,before,1)
        assert candidate==changed_candidate
        directed.append(dict(engine=engine,checks=204,retained_original_staged_reads=1,
                             sole_actual_state_negative_bit=74,negative_check=202))
    positives=[BUILD/f'full-tpc-j19-epochs-{e}-v2-20261025'for e in ['icarus','verilator']]
    for name in ['transactions.hex','initial.hex']:
        assert sha(positives[0]/name)==sha(positives[1]/name)
    gates=set()
    for path in (positives[0]/'timed').glob('cell_*.v'):
        gates.update(map(int,re.findall(r'assign\s+#\((\d+)\)',path.read_text())))
    clocks=(positives[0]/'timed/run_control_original_ff.sv').read_text()
    phase=module(clocks,'original_SE10231')
    assert gates=={1750}
    assert set(re.findall(r'<=#\((\d+)\)',phase))=={'1250'}
    assert set(re.findall(r'<=#\((\d+)\)',clocks.replace(phase,'')))=={'400'}
    for path in (positives[0]/'timed').glob('*'):
        if path.is_file():assert sha(path)==sha(positives[1]/'timed'/path.name)
    # The earlier case was blocked by the SAME real missed pulse in BOTH
    # engines; do not waive clock checks to reach the desired state assertion.
    for name in ['full-tpc-memory-Block-arrival-verilator-v3-20261025',
                 'full-tpc-memory-Block-arrival-icarus-v1-20261025']:
        d=BUILD/name;log=(d/'test.log').read_text()
        assert 'TPC_CLOCK_DIFF check=196 channel=55 actual=1 expected=0'in log
        assert 'TPC_CLOCK_DIFF check=196 channel=56 actual=1 expected=0'in log
        assert 't=163414.400 check=196'in log and 'j19_C=00'in log
        assert 't=163415.750 check=196'in log and 'j19_C=11'in log
    replays={}
    for mode in ['full','words']:
        states=[]
        for engine in ['icarus','verilator']:
            d=BUILD/f'j19-{mode}-{engine}-v1-20261025';s,row=observed(d)
            rows.append(row);retain(d,out/d.name);states.append(s)
            assert (d/adapter).read_text()==changed
            assert s['experimental_j19_ordered_epochs']and s['actual_native_visible_R_cells']
            if row.get('verified_live'):pending.append(row)
        assert states[1]['status']=='PASS_COMBINED_CP_COHERENT_PACKETS'
        replays[mode]=dict(Verilator=states[1]['summary'],Icarus=states[0]['status'])
        if states[0]['status']=='PASS_COMBINED_CP_COHERENT_PACKETS':
            peers=[BUILD/f'j19-{mode}-{e}-v1-20261025'for e in ['icarus','verilator']]
            assert clock_outcomes(peers[0])==clock_outcomes(peers[1])
    for d in sorted(BUILD.glob('native-R-words-late-tpc-*-20261025')):retain(d,out/d.name)
    analysis=BUILD/'control-short-native-receiver-analysis-v1-20261025'
    a=read(analysis/'results.json');verify(analysis,a['files'])
    assert a['status']=='REVIEWED_LAST_NATIVE_RX_PACKET_AND_BOOT_REGISTERS'
    assert a['registers']['SEQNO']['value']==0 and a['registers']['RHOST']['value']==0o177400
    assert a['whole_packet_CRC_and_zero_EOP_match']==[
        dict(start=10,CRC_address=7,CRC_word=38980,EOP_address=8,status_word=0)]
    assert a['pointer_write']==a['pointer_read']==9 and a['FIFO_empty']and not a['FIFO_full']
    retain(analysis,out/analysis.name)
    # Record primary data-sheet identities, without inventing a minimum LOW
    # width from the maximum toggle rate or treating an illustrative delay as
    # an actual part characterization.
    sources={}
    for name in ['Motorola_MECL_DataBook_1978.pdf','ON_MECL_DataBook_DL122_rev7.pdf']:
        path=ROOT/'DoradoDocs/datasheets'/name;sources[name]=sha(path)
    result=dict(status='REVIEWED_DIRECTED_J19_CAPTURE_AND_TPC_STATE_NEGATIVE',scope=__doc__,
                runs=rows,pending_replays=pending,directed_both_engines=directed,
                positive_traces_byte_identical=True,stored_bits_checked_per_event=844,
                SRAM_bits_checked_per_event=9216,clock_channels_checked=100,
                missed_original_clock_channels=[55,56],physical_clock_package='ProcL j19',
                ordered_epochs=['BEFORE','changed inputs before root','after root'],
                new_owner_bits=0,new_guest_cycles=0,new_RAM_ports=0,
                old_original_case_and_staged_read_retained=True,source_MIR_Block_input=261,
                actual_TPCBypass_corruption_bit=74,illustrative_j19_clock_LOW_ns=1.35,
                compiled_digital_profile_ps=dict(gate=1750,ordinary_FF=400,SE10231=1250),
                digital_profile_is_not_actual_part_characterization=True,
                primary_data_sheets=sources,physical_minimum_LOW_width_qualified=False,
                first_fixture_failed_final_staged_read_coverage_preserved=True,
                first_late_mutation_only_legacy_copy_preserved=True,
                generic_word_replay_without_required_phase_does_not_calibrate_TPC_negative=True,
                replays=replays,last_boot_packet_analysis=a,
                original_first_packet_loss_reason_established=False,
                new_j19_adapter_selected_in_running_native=False,
                full_source_arrival_and_unknown_capture_qualified=False,
                native_paired_phase_or60ns_qualified=False,physical_timing_qualified=False,
                original_firmware_Cedar_qualified=False,hardware_changed=False,new_RBF_ready=False,
                full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--archive',type=Path,required=True)
    parser.add_argument('--label',default='j19-capture-and-native-RX')
    args=parser.parse_args();result=review(args.output.resolve())
    append_archive(args.archive.resolve(),args.output.resolve(),[],label=args.label,extra_tools=TOOLS)
    print(result['status'])
