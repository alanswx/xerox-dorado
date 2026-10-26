#!/usr/bin/env python3
"""Retain terminal prepared-Hold timing failure and its new MIR/Ready path."""
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
    assert not out.exists()
    r=json.loads((source/'results.json').read_text());assert r['status']=='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'
    assert r['prepared_MemC_Hold']and r['source_files_verified']==581 and r['functional_HDL_source_union_count']==50
    state=r['terminal_state'];assert state['builder_pid']==1458513 and len(state['stages'])==9 and all(a['returncode']==0 for a in state['stages'])
    terminal=source/'terminal';c=json.loads((terminal/'collection.json').read_text());verify(terminal,c['collected_hashes'])
    stage=BUILD/'native-prepared-hold-stage-v1-20261025';m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert c['source_readback']==m['source_hashes']and r['exact_stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    fit=r['fit_summary'];assert'35,380 / 41,910 ( 84 % )'in fit and'Total registers : 50995'in fit and'335 / 553'in fit
    slacks={n:min(v['slack_ns']for v in rows)for n,rows in r['setup_paths_by_corner'].items()}
    assert slacks=={'7_slow_1100mv_-40c':-9.922,'7_slow_1100mv_100c':-10.251,
                    'MIN_fast_1100mv_-40c':3.288,'MIN_fast_1100mv_100c':1.394}
    worst=r['worst_setup_path'];assert worst['From Node'].endswith('r_front_upper_q[179]')
    assert 'b_MemX'in worst['To Node']and worst['To Node'].endswith('porta_we_reg')
    full=(terminal/'core/native59-setup-7_slow_1100mv_100c.rpt').read_text().split('Path #1:')[1].split('Path #2:')[0]
    for token in ['FA_eq_0_p_','DoCBr__drv','dRSTK_0','selector_preview|low_hold|direct_arrival|using_sparse',
                  'control|state_next[154]','control|state_next[50]','ready_comparison|capture_mismatch',
                  'network_step~CLKENA0','portawe']:
        assert token in full,token
    assert re.search(r'; Number of Logic Levels\s*;\s*;\s*18\s*;',full)
    assert re.search(r';\s*IC\s*;\s*;\s*19\s*;\s*18.032\s*;\s*71\s*;',full)
    assert 'assign FF_0__drv=r_front_q[767];'in(stage/'src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv').read_text()
    groups={}
    for pattern,minimum in [('native59-hold-*.rpt',0.067),('native59-sdram-input-setup-*.rpt',0.339),
                            ('native59-sdram-input-hold-*.rpt',4.259),('native59-sdram-output-setup-*.rpt',3.233),
                            ('native59-sdram-output-hold-*.rpt',3.855)]:
        files=list((terminal/'core').glob(pattern));rows=[v for p in files for v in paths(p.read_text())]
        assert len(files)==4 and min(v['slack_ns']for v in rows)==minimum
        groups[pattern]=dict(corners=4,minimum_slack_ns=minimum)
    unconstrained=(terminal/'core/native-unconstrained.rpt').read_text()
    for label,n in [('Illegal Clocks',0),('Unconstrained Clocks',0),('Unconstrained Input Ports',3),('Unconstrained Output Ports',50)]:
        assert re.search(r'; '+label+r'\s*;\s*'+str(n)+r'\s*;',unconstrained)
    out.mkdir(parents=True);retain(source,out/'retained'/source.name)
    result=dict(status='REJECTED_PREPARED_HOLD_CORE_TIMING_WITH_MIR_READY_PATH_IDENTIFIED',
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_review_sha256=sha(source/'results.json'),
                source_files_verified=581,terminal_builder_pid=1458513,all_nine_report_stages_zero=True,
                fitted_ALMs=35380,fitted_percent=84,fitted_registers=50995,RAM_blocks=335,memory_bits=2511686,
                setup_slacks_by_corner_ns=slacks,worst_path=worst,worst_data_delay_ns=25.536,
                worst_logic_levels=18,worst_interconnect_ns=18.032,worst_interconnect_percent=71,
                source_owner_held_bit=767,source_owner_name='ContA FF.0 (MIR field)',
                critical_cone='MIR FF.0 -> FA decode/DoCBr -> ContB dRSTK -> selector sparse eligibility -> joint control/MiscHold/Ready target -> duplicate Ready comparison -> global G3 execution grant -> MemX RAM write enable',
                hold_and_SDRAM_corner_checks=groups,illegal_clocks=0,unconstrained_clocks=0,unconstrained_input_ports=3,unconstrained_output_ports=50,
                all_IO_or_full_timing_qualified=False,prepared_Hold_component_proofs_not_invalidated_by_timing=True,
                standalone_Hold_cut_not_sufficient_for_CPU_timing=True,candidate_selected_for_faster_core=False,
                assembly_or_FPGA_load_permitted=False,new_hardware_speed_gain_measured=False,
                nominal_ordinary_ns=120,target_ordinary_ns=60,full_goal_active=True,
                next_action='Keep this completed job terminal. Split actual MIR/control/capture phases and qualify authoritative Ready/Hold updates against the original source. Inspect the duplicate Ready validation in the execution-enable cone; do not bypass actual readiness/storage barriers. Avoid stacking another unqualified late-Ready buffer or calling serialized120ns success. Finish pending caller/native runs and post-initialization wait attribution, then original60ns clocks, hardware/Cedar/Rollback/persistence/three-boots/stress.')
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();r=review(a.source.resolve(),a.output.resolve())
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='native-prepared-hold-terminal-timing',
        extra_tools=[Path(__file__).name,'review_native_cp_read_timing.py','collect_native_clock_enable_fit.py'])
    print(r['status'],flush=True)
