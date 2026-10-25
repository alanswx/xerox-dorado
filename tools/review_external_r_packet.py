#!/usr/bin/env python3
"""Retain the exact packet SRAM move, complete replays and reachable-RAM audit.

This closes a wiring prerequisite, not the combined staged/paired native path,
15ns physical timing, interactive boot or a measured performance improvement.
"""
import argparse
import json
import math
from pathlib import Path
import re

from generate_external_r_packet import packet,caller,bench
from inspect_external_r_packet import structure
from review_baseboard_clock_consumer import verify
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
TOOLS=['generate_external_r_packet.py','test_external_r_packet.py',
       'inspect_external_r_packet.py','review_external_r_packet.py','inspect_native_baseboard_process.py']


def read(p):
    return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    source=BUILD/'supported-fall-guard-owned-verilator-v1-20261024'
    parent=read(source/'run-state.json');assert parent['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS'
    out.mkdir(parents=True);runs=[];peers=[]
    for d in sorted(BUILD.glob('external-packet-*-20261025')):
        if not re.match(r'^external-packet-(?:icarus|verilator)-',d.name):
            continue
        s=read(d/'run-state.json');assert s['status']!='RUNNING',d.name
        verify(d,s.get('files',{}))
        for row in s.get('source_files',[]):
            assert sha(d/row['path'])==row['output_sha256']
        if s.get('test_log_sha256'):
            assert sha(d/'test.log')==s['test_log_sha256']
        assert all('returncode' in a for a in s.get('actions',[])),d.name
        runs.append(dict(directory=d.name,status=s['status'],state_sha256=sha(d/'run-state.json')))
        retain(d,out/d.name)
    for engine in ['icarus','verilator']:
        d=BUILD/f'external-packet-{engine}-v3-20261025';s=read(d/'run-state.json')
        assert s['status']=='PASS_EXTERNAL_FULL_PROCESSOR_PACKETS'
        assert s['all_original_assertions_retained'] and s['all100_clock_counts_match']
        assert s['summary']==parent['summary'] and s['cache_coverage']==parent['cache_coverage']
        assert s['summary']=='transactions=14884 commits=10423 pairs=4461 candidates=4461 fallbacks=0 paired_writes=119 forwarded_bytes=238 Qs=844 RAM=9216 results=2470 clocks=100'
        assert s['cache_coverage']=='fast=8885 staged=1538 prepare_edges=1538 resets=1'
        assert (d/'packet.sv').read_text()==packet((source/'packet.sv').read_text())
        assert (d/'caller.sv').read_text()==caller((source/'caller.sv').read_text())
        assert (d/'tb.sv').read_text()==bench((source/'tb.sv').read_text())
        for row in parent['source_files']:
            assert sha(d/row['path'])==sha(source/row['path'])
        assert sha(d/'forward.sv')==sha(source/'forward.sv')
        clocks=re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',(d/'test.log').read_text(),re.M)
        assert len(clocks)==100 and clocks==re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',(source/'test.log').read_text(),re.M)
        assert 'PACKET_GEOMETRY_REJECTIONS cases=403' in (d/'test.log').read_text()
        assert not s['native_integrated'] and not s['staged_fallback_integrated']
        peers.append(d)
    for name in ['packet.sv','caller.sv','tb.sv']:
        assert sha(peers[0]/name)==sha(peers[1]/name)
    negatives={}
    for mutation in ['early-write','drop-forward']:
        d=BUILD/f'external-packet-icarus-{mutation}-v3-20261025';s=read(d/'run-state.json')
        assert s['status']=='PASS_EXTERNAL_PACKET_COUNTEREXAMPLE'
        assert s['actions'][0]['returncode']==0 and s['actions'][1]['returncode']!=0
        log=(d/'test.log').read_text()
        if mutation=='early-write':
            assert 'PACKET invalid geometry committed bit=0' in log
        else:
            assert 'PACKET second result mismatch' in log and 'PACKET_DIFF last bit=' in log
        negatives[mutation]=[v for v in log.splitlines() if 'FATAL' in v or 'PACKET_DIFF' in v]
    d=BUILD/'external-packet-structure-v1-20261025';s=read(d/'run-state.json')
    assert s['status']=='PASS_EXTERNAL_PACKET_ONE_SRAM_AND_DUPLICATE_CONTROL'
    for row in s['cases']:
        case=d/('duplicate-control' if row['bad'] else 'positive')
        assert row['passed'] and row['returncode']==0
        assert sha(case/'hierarchy.xml')==row['XML_sha256'] and sha(case/'elaborate.log')==row['log_sha256']
        assert structure(case/'hierarchy.xml')==row['structure']
    retain(d,out/d.name)
    native=BUILD/'control-short-native-network-read-v2-20261025';n=read(native/'result.json')
    assert n['pid']==69736 and n['read_only_guest_inspection']
    assert 'Process 69736 detached' in (native/'lldb.log').read_text()
    assert n['counters']['network_tx_good']==1 and n['counters']['network_rx_bad']==0 and n['counters']['network_tx_bad']==0
    assert n['counters']['network_wire_rstate']==0 and n['counters']['network_rx_ready']==1
    assert n['counters']['network_controller_RxFifoEmpty']==1 and n['counters']['network_controller_RxFifoFull']==0
    words=n['network_buffer']['first32_words'][:n['counters']['network_rx_words']]
    assert len(words)==13 and words[1:4]==[0o1000,0o26,0o265]
    assert words[5]==0o76 and words[-1]==0xffff
    cedar=ROOT/'chm/dorado/CedarDorado.eb!6'
    assert (cedar.stat().st_size-512)%2==0
    expected_end=math.ceil(((cedar.stat().st_size-512)//2)/255)
    assert words[5]==expected_end
    translated=BUILD/'control-short-native-network-read-v3-20261025';tn=read(translated/'result.json')
    assert tn['pid']==69736 and tn['read_only_guest_inspection']
    assert 'Process 69736 detached' in (translated/'lldb.log').read_text()
    assert tn['counters']['host_ticks']>n['counters']['host_ticks']
    assert tn['binary_sha256']==n['binary_sha256'] and tn['header_sha256']==n['header_sha256']
    banks=tn['translated_front']['physical_9bit_banks']
    assert set(banks)=={'rm_h','rm_l','stk_h','stk_l'}
    assert all(len(v)==256 and all(0<=w<512 for w in v) for v in banks.values())
    assert len(tn['translated_front']['held_owner_hex'])==211
    for version in [1,2,3]:
        nd=BUILD/f'control-short-native-network-read-v{version}-20261025';retain(nd,out/nd.name)
    failed=BUILD/'external-packet-sram-review-v1-20261025'
    assert read(failed/'failure.json')['status']=='REVIEW_GLOB_INCLUDED_OWN_OUTPUT'
    target=out/failed.name;target.mkdir()
    # Its partial copies are byte-identical to the complete runs retained
    # above. Preserve the actual failed script and failure receipt explicitly.
    for name in ['failed-review-script.py','failure.json']:
        (target/name).write_bytes((failed/name).read_bytes())
    medium=BUILD/'cedar-work-native-hints.pack'
    assert medium.stat().st_size==63578624 and sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result=dict(status='REVIEWED_EXTERNAL_PACKET_SRAM_AND_UNCHANGED_PAIRED_PATH',scope=__doc__,runs=runs,
                full_original_replay_both_engines=True,transactions=14884,commits=10423,pairs=4461,
                paired_writes=119,forwarded_bytes=238,geometry_rejections=403,
                checked_result_bits=2470,checked_owner_bits=844,checked_SRAM_bits=9216,checked_clock_channels=100,
                processor_and_forwarding_source_byte_identical=True,cache_coverage_unchanged=True,
                reachable_SRAM_instances=1,reachable_banks=4,reachable_bits=9216,
                duplicate_control_SRAM_instances=2,duplicate_control_bits=18432,
                actual_negative_failures=negatives,all_preflight_compile_failures_retained=True,
                native_network_read=dict(pid=69736,host_ticks=n['counters']['host_ticks'],guest_ticks=n['counters']['guest_ticks'],
                    last_host_loaded_frame_octal=[format(w,'06o') for w in words],frame_type='Microcode boot reply265B',
                    last_frame_offset_octal='76',wire_idle=True,controller_FIFO_empty=True,
                    final_frame_sequence_matches_Cedar_file_length=True,Cedar_file_sha256=sha(cedar),
                    no_new_RX_or_TX_CRC_fault_count=True,not_proof_of_guest_consumption=True,
                    boot_pass=False,same_process_detached_and_resumed=True),
                protected_medium_unchanged=True,native_integrated=False,staged_and_paired_combined=False,
                raw_translated_front_read=dict(pid=69736,host_ticks=tn['counters']['host_ticks'],
                    guest_ticks=tn['counters']['guest_ticks'],physical_banks=4,physical_bits=9216,held_owner_bits=844,
                    same_binary_and_header=True,not_a_logical_register_decode_or_boot_pass=True),
                native16_visible_R_hookup_qualified=False,physical_timing_qualified=False,
                new_RBF_ready=False,hardware_changed=False,measured_speed_improvement=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label='external-packet-sram',extra_tools=TOOLS)
    print(r['status'])
