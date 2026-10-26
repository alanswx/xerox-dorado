#!/usr/bin/env python3
"""Retain actual-native-source66.7MHz SDRAM protocol evidence and limitations."""
import argparse
import json
from pathlib import Path
import shutil

from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]


def review(tests,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((tests/'run-state.json').read_text());verify(tests,r['files'])
    assert r['status']=='PASS_NATIVE_SOURCE_SDRAM_66MHZ_PIN_MODEL'
    stage=Path(r['stage']);m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert r['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and m['frequency_hz']==66666667
    assert sha(stage/'src/fpga/mister/dorado_storage_sdram_burst.sv')==r['native_backend_sha256']==sha(tests/'inputs/burst.sv')
    assert r['native_options']=={'RESPONSE_OVERLAP':1,'CAPTURE_READ_WORD':1,'EARLY_READ_RESPONSE':1,'CACHE_LINES':4}
    assert r['native_address_bits']==23 and r['pin_bench_address_bits']==26
    assert len(r['cases'])==5 and all(c['passed']for c in r['cases'])
    assert [(c['clock_hz'],c['clock_delay_ns'])for c in r['cases'][:3]]==[(50000000,0),(66666667,0),(66666667,6)]
    results=[]
    for case in r['cases']:
        d=tests/case['name']
        for action in case['actions']:
            assert sha(d/(action['label']+'.log'))==action['log_sha256']
            if action['label']=='compile':assert action['returncode']==0
        log=(d/'test.log').read_text()
        if case['negative']is None:
            assert case['returncode']==0 and case['metrics']==['STORAGE-SDRAM-BURST PASS use_burst=1 lines=4 checks=1197 reads=636 writes=561 physical_reads=88 physical_writes=562 cache_hits=557']
            assert sha(d/'burst.sv')==r['native_backend_sha256']
        elif case['negative']=='early':
            assert case['returncode']!=0 and 'read 00000000 got 0000 expected a57b'in log
        else:
            assert case['negative']=='frequency'and case['returncode']!=0
            assert 'command PRECHARGE issued at 150075.000 ns before 200 us power-up delay'in log
        results.append({k:case[k]for k in ['name','clock_hz','clock_delay_ns','negative','returncode','metrics']})
    retain(tests,out/'retained'/tests.name)
    result=dict(status='REVIEWED_NATIVE_SOURCE_66MHZ_SDRAM_PIN_PROTOCOL',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                native_backend_sha256=r['native_backend_sha256'],source_tests_state_sha256=sha(tests/'run-state.json'),
                cases=results,all_original_pin_bench_assertions_retained=r['original_assertions_retained'],
                protocol_checks='200us power-up, both ranks/banks, corrected CAS2 sampling, reset during every burst beat, writes/reset, held responses, refresh under backpressure, pin mapping and original tOH glitch check',
                address_width_scope='Exact frozen native controller source/options,26-bit pin fixture covers both physical ranks; native23-bit connection is also exercised by the separate full native startup.',
                full_native_CDC_or_board_flight_or_four_corner_IO_timing_or_hardware_qualified=False,
                full60ns_capture_or_Cedar_or_measured_hardware_gain_qualified=False,
                FPGA_or_existing_runs_or_protected_media_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['tests','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.tests.resolve(),a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-sdram-66mhz-pin',extra_tools=[Path(__file__).name,'test_native_sdram_66mhz.py'])
    print(r['status'],flush=True)
