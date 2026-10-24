#!/usr/bin/env python3
"""Preserve full-native timing failure, ordered-clock proof and boot observations.

Keep component timing, actual native timing and firmware progress separate.
The new oscillator producer is not connected to display consumers. The native
fit fails; the next physical-fanout experiment changes no HDL or clock. Live
process observations and read-only debugger reads are not completed boot gates.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import subprocess

from review_late_hold_and_input_reset import first_path,retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(path):return json.loads(path.read_text())


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    fit=BUILD/'balanced-native-fit-terminal-v1-20261024';p=read(fit/'r-front-control-native.json')
    assert read(fit/'run-state.json')['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert all(s['returncode']==0 for s in read(fit/'run-state.json')['stages'])
    assert read(fit/'physical-report/run-state.json')['status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    for name,digest in p['source_hashes'].items():assert sha(fit/name)==digest,name
    assert p['frequency_hz']==57142857 and p['sysper']==4 and not p['assembly_permitted']
    corners=[]
    for name in ['7_slow_1100mv_100c','7_slow_1100mv_-40c','MIN_fast_1100mv_100c','MIN_fast_1100mv_-40c']:
        row=dict(condition=name)
        for label,prefix in [('setup','native59-setup'),('hold','native59-hold'),
                             ('owner_input','physical-native-input-setup'),('owner_internal','physical-native-internal-setup')]:
            row[label]=first_path(fit/'core'/(prefix+'-'+name+'.rpt'))
        corners.append(row)
    assert corners[0]['setup']['slack_ns']==-1.314 and corners[1]['setup']['slack_ns']==-1.053
    assert 'cell_MC1668_snapshot:u_a15|qa'in corners[0]['setup']['source']
    assert 'stk_h_rtl_0'in corners[0]['setup']['sink']
    map_report=(fit/'core/output_files/Dorado.map.rpt').read_text()
    loop_table=map_report[map_report.index('; Logic Cells Representing Combinational Loops'):]
    loop_table=loop_table[:loop_table.index('Note:')]
    assert 'Number of logic cells representing combinational loops'in loop_table
    assert re.search(r'Number of logic cells representing combinational loops\s*; 1\s*;',loop_table)
    assert 'DskEth_full_snapshot_experiment:b_DskEth|EccComputeErr_p_~0'in loop_table
    disk='src/fpga/mister/snapshot_native_dsketh/DskEth_full_snapshot.sv'
    baseline=BUILD/'native-cedar-5c65-input-reset-v3-20261023/inputs'
    assert sha(fit/disk)==sha(baseline/disk)
    body=re.search(r'cell_MC10113 u_c05 \(.*?\n  \);',(fit/disk).read_text(),re.S)[0]
    assert '.p10('not in body and '.p12('not in body
    for snippet in ['.p9(ComputeECC_p_)','.p11(EccData_32)','.p13(EccComputeErr_p_)',
                    '.p14(EccComputeErr_p___c05_14)','.p15(EccComputeErr_p___c05_15)']:assert snippet in body
    xor=(fit/'src/verilog/cells/cell_MC10113.v').read_text()
    for snippet in ['wire en = ~p9;','assign p14 = en & (p10 ^ p11);','assign p15 = en & (p12 ^ p13);']:assert snippet in xor
    coverage=re.findall(r'PHYSICAL_NATIVE_COVERAGE (\S+) keepers=(\d+) central=(\d+) primary=(\d+) duplicates=(\d+) history=(\d+)',
                        (fit/'physical-report/audit.log').read_text())
    assert len(coverage)==4
    retain(fit,out/fit.name)
    tests=BUILD/'oscillator-events-v3-20261024';t=read(tests/'run-state.json')
    assert t['status']=='PASS_ORDERED_OSCILLATOR_EVENTS_AND_NATIVE_RATE_COUNTEREXAMPLE'and len(t['cases'])==14
    for name,digest in t['inputs'].items():assert sha(tests/'inputs'/name)==digest,name
    for case in t['cases']:
        log=tests/(case['engine']+'-'+case['case']+'-test.log')
        assert sha(log)==case['log_sha256']
        assert (case['returncode']!=0)==case['expected_rejection']
    assert sha(tests/'inputs/dorado_oscillator_events.sv')==sha(ROOT/'verilog/rtl/dorado_oscillator_events.sv')
    assert sha(tests/'inputs/tb_oscillator_events.sv')==sha(ROOT/'verilog/verilator/tb_oscillator_events.sv')
    retain(tests,out/tests.name)
    failed=BUILD/'oscillator-events-v1-20261024';f=read(failed/'run-state.json')
    assert f['status']=='TERMINAL_OSCILLATOR_EVENT_TEST_FAILED'
    assert 'OSC_EVENTS endpoint coincidence'in(failed/'icarus-advance-while-held-test.log').read_text()
    history=BUILD/'oscillator-events-v2-20261024'
    assert read(history/'run-state.json')['status']=='PASS_ORDERED_OSCILLATOR_EVENTS_AND_NATIVE_RATE_COUNTEREXAMPLE'
    assert (failed/'inputs/positive.sv').read_bytes()==(history/'inputs/positive.sv').read_bytes()
    retain(history,out/history.name)
    retain(failed,out/failed.name)
    proofs=[]
    for variant in ['positive','drop-second','reverse-order','advance-while-held','late-offset']:
        source=BUILD/f'oscillator-events-{variant}-proof-v2-20261024';s=read(source/'run-state.json')
        assert s['status']==('PASS_ORDERED_50MHZ_EVENTS_INDUCTION'if variant=='positive'else'PASS_OSCILLATOR_EVENT_COUNTEREXAMPLE')
        assert s['returncode']==0 and not s['assumptions_added']
        assert s['unbounded_induction_proven']==(variant=='positive')
        assert s['tests_state_sha256']==sha(tests/'run-state.json')
        assert s['actual_source_sha256']==sha(tests/'inputs'/(variant+'.sv'))
        for name,digest in s['files'].items():assert sha(source/name)==digest,name
        assert sha(source/'proof.log')==s['proof_log_sha256'];retain(source,out/source.name)
        proofs.append(dict(variant=variant,status=s['status'],state_sha256=sha(source/'run-state.json')))
    debugger=[]
    for name in ['native-5c65-network-read-v1-20261024','native-queue-network-read-v1-20261024',
                 'native-5c65-network-read-v3-20261024']:
        source=BUILD/name;s=read(source/'result.json');run=Path(s['source'])
        assert s['read_only_guest_inspection'] and 'Process '+str(s['pid'])+' detached'in(source/'lldb.log').read_text()
        assert sha(run/'obj/native-cedar')==s['binary_sha256']
        assert sha(run/'obj/Vtb_native_cedar___024root.h')==s['header_sha256']
        assert s['network_buffer']['first32_words'][3]==0o265
        assert s['network_buffer']['first32_words'][5]==0o76
        retain(source,out/source.name);debugger.append(dict(directory=name,pid=s['pid'],counters=s['counters']))
    source=BUILD/'native-5c65-network-read-v2-20261024'
    assert read(source/'failure.json')['status']=='SOURCE_BINDING_REJECTED_BEFORE_DEBUG_ATTACH';retain(source,out/source.name)
    s=read(BUILD/'native-5c65-network-read-v3-20261024/result.json')
    words=[int(f'{value:016b}'[::-1],2)for value in s['register_file']['wire_order_words']]
    symbols=BUILD/'native-initial-symbols-v1-20261024'
    listing=subprocess.check_output([str(symbols/'registers')],cwd=ROOT,text=True)
    (out/'initial-register-symbols.log').write_text(listing)
    addresses={name:int(address)for name,address in re.findall(r'^(\w+) memory=RM .*address_decimal=(\d+)$',listing,re.M)}
    anchors={'R400':0o400,'BOOTDATAPTR':0o1000,'LHOST':0o42,'BFN':0o113}
    for name,value in anchors.items():assert words[addresses[name]]==value,(name,words[addresses[name]])
    assert words[addresses['SEQNO']]==0 and words[addresses['RHOST']]==0o177400
    for name in ['symbols.c','registers.c']:shutil.copy2(symbols/name,out/name)
    for name in ['dorado/src/mb.c','dorado/include/mb.h','chm/dorado/expanded/bootstrap.dm!20_/Initial.mb',
                 'chm/dorado/expanded/BootstrapSources.dm/InitialEther.mc']:
        dest=out/'symbol-and-protocol-inputs'/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/name,dest)
    fanout=BUILD/'memc-address-fanout-native-stage-v1-20261024';n=read(fanout/'r-front-control-native.json')
    assert n['only_QSF_physical_assignments_changed_after_relocation']and not n['HDL_or_clock_changes']
    assert n['memc_address_fanout_experiment']['source_registers']==16
    for name,digest in n['source_hashes'].items():assert sha(fanout/name)==digest,name
    changed=[name for name in p['source_hashes']if (fanout/name).read_bytes().replace(n['project'].encode(),p['project'].encode())!=(fit/name).read_bytes()]
    assert changed==['core/Dorado.qsf']
    retain(fanout,out/fanout.name)
    retain(BUILD/'memc-address-fanout-preparation-v1-20261024',out/'memc-address-fanout-preparation-v1-20261024')
    terminal=BUILD/'memc-address-fanout-fit-terminal-v1-20261024'
    collection_path=BUILD/'memc-address-fanout-native-v1-20261024-terminal.tgz.json'
    collection=read(collection_path)
    assert sha(Path(str(collection_path)[:-5]))==collection['archive_sha256']
    for row in collection['members']:
        file=terminal/row['path'];assert sha(file)==row['sha256']and file.stat().st_size==row['bytes']
    assert read(terminal/'r-front-control-native.json')==n
    assert read(terminal/'physical-native-input-readback.json')['actual']==n['source_hashes']
    assert all(s['returncode']==0 for s in read(terminal/'run-state.json')['stages'])
    assert read(terminal/'physical-report/run-state.json')['status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    fanout_corners=[]
    for row in corners:
        condition=row['condition']
        fanout_corners.append(dict(condition=condition,**{label:first_path(terminal/'core'/(prefix+'-'+condition+'.rpt'))
            for label,prefix in [('setup','native59-setup'),('hold','native59-hold'),
                                 ('owner_input','physical-native-input-setup'),('owner_internal','physical-native-internal-setup')]}))
    assert [r['setup']['slack_ns']for r in fanout_corners]==[-0.995,-0.979,3.684,4.505]
    assert min(r['hold']['slack_ns']for r in fanout_corners)==0.053
    assert 'u_a14|qb~SynDup'in fanout_corners[0]['setup']['source']
    assert 'r_front_upper_q[4]'in fanout_corners[0]['setup']['sink']
    critical=(terminal/'core/native59-setup-7_slow_1100mv_100c.rpt').read_text()
    critical=critical[critical.index('Path #1:'):critical.index('Path #2:')]
    assert re.search(r';\s*321\s*;[^\n]*u_a14\|qb~SynDup\|q\s*;',critical)
    assert re.search(r';\s*320\s*;[^\n]*u_a14\|qb~SynDup_wirecell\|combout\s*;',critical)
    map_log=(terminal/'map.log').read_text()
    assert map_log.count('Warning (18060)')==5 and map_log.count('Info (18058)')==9
    retain(terminal,out/terminal.name);shutil.copy2(collection_path,out/collection_path.name)
    next_stage=BUILD/'memc-address-fanout-native-stage-v2-20261024'
    next_manifest=read(next_stage/'r-front-control-native.json')
    assert next_manifest['memc_address_fanout_experiment']['physical_register_duplication_enabled']
    for name,digest in next_manifest['source_hashes'].items():assert sha(next_stage/name)==digest,name
    changed=[]
    for name in n['source_hashes']:
        raw=(next_stage/name).read_bytes().replace(next_manifest['project'].encode(),n['project'].encode())
        old=(fanout/name).read_bytes()
        if raw!=old:
            changed.append(name)
            assert raw==old.replace(b'PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION OFF',b'PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION ON',1)
    assert changed==['core/Dorado.qsf'];retain(next_stage,out/next_stage.name)
    remote=BUILD/'memc-address-fanout-live-v2-20261024.json';live=read(remote)
    assert live['run_state']['status']=='RUNNING'and 'quartus'in live['stage_command']
    assert live['manifest_sha256']==sha(next_stage/'r-front-control-native.json');shutil.copy2(remote,out/remote.name)
    native=[]
    for name in ['cp-prefix-balanced-native-boot-v1-20261024','cp-prefix-native-owned-cold-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023','native-cedar-queue-input-reset-v3-20261023']:
        run=BUILD/name;dest=out/('observed-'+name);native.append(observation(run,dest))
        for file in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:shutil.copy2(run/file,dest/file)
    result=dict(status='REVIEWED_NATIVE_TIMING_AND_ORDERED_CLOCK_PRODUCER',scope=__doc__,native_fit_corners=corners,
        physical_native_coverage=coverage,native_fit_accepted=False,native_source_count=533,
        physical_logic_loop_count=1,physical_loop='DskEth EccComputeErr_p_',
        ECC_source_byte_identical_to_baseline=True,
        ECC_equation='err = !ComputeECC_n && (EccData32 || err), with original unused ECL inputs pulled low',
        ECC_classification='Intentional error retention: disabled clears, enabled data1 sets, enabled data0 retains. Analog/pulse/fitted latch timing remains unqualified.',
        packed_processor_Q_loops_not_listed_in_this_fitted_netlist=True,
        native_clock_hz=57142857,sysper=4,oscillator_cases=t['cases'],oscillator_proofs=proofs,
        oscillator_exact_edge_offsets_proven=True,
        active_pixel_counterexample='Four15ns samples produce4000/6000 required half-period transitions; current eight-sample ratio passes.',
        display_crystal_output_unused_in_current_wrapper=True,ordered_events_consumed_by_native_display=False,
        network_read_observations=debugger,baseline_sequence_number=0,baseline_remote_host=0o177400,
        software_register_anchors=anchors,protocol_root_cause_established=False,matched_firmware_speed_qualified=False,
        fanout_fit_corners=fanout_corners,fanout_fit_accepted=False,
        fanout_ignored_assignment_warnings=5,fanout_inserted_logic_assignment_groups=9,
        fanout_worst_source_loads=321,fanout_worst_wirecell_loads=320,
        next_fanout_experiment=live,native_live_observations=native,
        hardware_changed=False,assembly_permitted=False,speedup_qualified=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--label',default='native-clock-timestamps')
    a=p.parse_args();r=review(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),[],label=a.label,extra_tools=[
        'test_oscillator_events.py','prove_oscillator_events.py','inspect_native_baseboard_process.py',
        'stage_memc_address_fanout.py','collect_r_front_physical_native.py',Path(__file__).name])
    print(r['status'],flush=True)
