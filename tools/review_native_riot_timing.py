#!/usr/bin/env python3
"""Retain the first routed CE core, remaining real setup failure and SDRAM paths.

All report stages and580 source hashes must have completed. No assembly,
timing exception, clock reduction or readiness bypass follows from fitting.
"""
import argparse
import json
from pathlib import Path
import re

from review_native_cp_read_timing import paths
from review_baseboard_clock_consumer import verify
from review_native_alu_capture import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def review(source,out):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((source/'results.json').read_text())
    assert r['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'and r['native_RIOT_block_RAM']
    assert r['source_files_verified']==580 and r['terminal_state']['builder_pid']==1029546
    assert len(r['terminal_state']['stages'])==9 and all(a['returncode']==0 for a in r['terminal_state']['stages'])
    terminal=source/'terminal';o=json.loads((terminal/'collection.json').read_text());verify(terminal,o['collected_hashes'])
    stage=BUILD/'native-riot-blockram-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert o['source_readback']==m['source_hashes']and r['exact_stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    fit=(terminal/'core/output_files/Dorado.fit.summary').read_text()
    assert '35,880 / 41,910 ( 86 % )'in fit and 'Total registers : 50770'in fit
    assert '335 / 553'in fit and '2,511,686 / 5,662,720'in fit
    core=terminal/'core';groups={}
    expected={'native59-hold-*.rpt':0.085,'native59-sdram-input-setup-*.rpt':0.442,
              'native59-sdram-input-hold-*.rpt':4.263,'native59-sdram-output-setup-*.rpt':3.248,
              'native59-sdram-output-hold-*.rpt':3.856}
    for pattern,worst in expected.items():
        files=list(core.glob(pattern));assert len(files)==4
        rows=[p for f in files for p in paths(f.read_text(errors='replace'))]
        assert rows and min(v['slack_ns']for v in rows)==worst
        groups[pattern]=dict(files=4,path_count=len(rows),minimum_slack_ns=worst)
    setup={name:min(v['slack_ns']for v in rows)for name,rows in r['setup_paths_by_corner'].items()}
    assert setup=={'7_slow_1100mv_-40c':-8.734,'7_slow_1100mv_100c':-9.117,
                   'MIN_fast_1100mv_-40c':3.563,'MIN_fast_1100mv_100c':1.996}
    worst=r['worst_setup_path']
    assert worst['From Node'].endswith('r_front_upper_q[254]')and worst['To Node'].endswith('u_network_wire|rstate[0]')
    full=(core/'native59-setup-7_slow_1100mv_100c.rpt').read_text().split('Path #1:')[1].split('Path #2:')[0]
    for token in ['b_MemC|NewRef','b_MemC|dVA_11','r_front_pins[386]',
                  'direct_arrival|state_next[65]','phase|root_arrival|shortened|captures',
                  'clock_view|l|i18|p14','network_step~CLKENA0','rstate[0]|sload']:
        assert token in full,token
    assert re.search(r'; Number of Logic Levels\s*;\s*;\s*15\s*;',full)
    assert re.search(r';\s*IC\s*;\s*;\s*16\s*;\s*17.390\s*;\s*74\s*;',full)
    ucp=(core/'native-unconstrained.rpt').read_text()
    for label,number in [('Illegal Clocks',0),('Unconstrained Clocks',0),('Unconstrained Input Ports',3),('Unconstrained Output Ports',50)]:
        assert re.search(r'; '+label+r'\s*;\s*'+str(number)+r'\s*;',ucp),label
    board=stage/'src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv';text=board.read_text()
    assert 'assign r_front_pins[386]=MemC15_sil_pl_2;'in text
    assert 'assign MemC15_sil_pl_2 = MemC15_sil_pl_2__e15_3 | MemC15_sil_pl_2__e15_14 | MemC15_sil_pl_2__e16_3 | MemC15_sil_pl_2__e16_14 | MemC15_sil_pl_2__e17_3;'in text
    retain(source,out/'retained'/source.name)
    result=dict(status='REVIEWED_ROUTED_NATIVE_CE_CORE_WITH_REAL_SETUP_FAILURE',source_review_sha256=sha(source/'results.json'),
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),builder_pid=1029546,terminal_all_nine_stages_zero=True,
                fitted_ALMs=35880,fitted_percent=86,fitted_registers=50770,RAM_blocks=335,memory_bits=2511686,
                setup_slacks_by_corner_ns=setup,worst_setup_path=worst,
                worst_data_delay_ns=23.551,worst_logic_levels=15,worst_data_interconnect_ns=17.390,worst_interconnect_percent=74,
                remaining_path='Held request owner -> MemC NewRef/dVA/cache conditional Hold producer(pin386) -> control capture/derived phase -> ready/grant -> global G3 distribution -> Ethernet receiver state synchronous load',
                hold_and_SDRAM_corner_checks=groups,illegal_clocks=0,unconstrained_clocks=0,
                unconstrained_inputs=3,unconstrained_outputs=50,all_IO_or_full_timing_qualified=False,
                conditional_Hold_source_sha256=sha(board),nominal_ordinary_ns=120,target_ordinary_ns=60,
                assembly_or_load_permitted=False,new_hardware_speed_gain_measured=False,full_goal_active=True,
                next_action='Trace actual MemC conditional-Hold/cache-compare producer and control capture phases; prepare/capture stable producer values before their original consumer edge. Prove real MIR/Ready/Hold/previous-instruction completion and X/Z fallback. Do not bypass readiness, add blanket exceptions, lower the target clock or merely retime Ethernet without its source path. Then same-source fit/full native/physical SDRAM and all hardware/Cedar/Rollback/persistence/three-boot/stress requirements.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.source.resolve(),a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-riot-blockram-terminal-timing',extra_tools=[Path(__file__).name,'review_native_cp_read_timing.py'])
    print(r['status'],flush=True)
