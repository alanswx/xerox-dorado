#!/usr/bin/env python3
"""Review separate original j19 data routes and bounded native startup.

Preserve the rejected earlier underflow transfer, its accidentally passing
forced-one control, both corrected data tests and specific epoch negatives.
Digital profile evidence does not qualify arbitrary arrivals or FPGA timing.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from generate_j19_capture import rewrite, route
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_cp_interior_stage import event_assertions
from review_cp_paired_read import observed
from review_late_hold_and_input_reset import retain
from review_native_r_capture import clock_outcomes
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'
ADAPTER = 'adapters/dorado_local_r_front_step.sv'


def read(path):
    return json.loads(path.read_text())


def frozen_rewrite(directory):
    # Execute only the retained repository helper; never substitute today's
    # generator when checking a historical experiment's exact source.
    namespace = {'__name__':'retained_j19_generator'}
    path = directory/'generate_j19_capture.py'
    exec(compile(path.read_text(),str(path),'exec'),namespace)
    return namespace['rewrite']


def profile(directory):
    gates = set()
    for path in (directory/'timed').glob('cell_*.v'):
        gates.update(map(int,re.findall(r'assign\s+#\((\d+)\)',path.read_text())))
    raw = (directory/'timed/run_control_original_ff.sv').read_text()
    phase = module(raw,'original_SE10231')
    ordinary = set(map(int,re.findall(r'<=#\((\d+)\)',raw.replace(phase,''))))
    phase_values = set(map(int,re.findall(r'<=#\((\d+)\)',phase)))
    assert len(gates)==len(ordinary)==len(phase_values)==1
    return dict(gate=gates.pop(),ordinary_FF=ordinary.pop(),SE10231=phase_values.pop())


def review(out, require_full=False):
    assert not out.exists()
    out.mkdir(parents=True)
    rows, pending, retained = [], [], set()
    base = BUILD/'native-R-single-verilator-v1-20261025'
    old_adapter = (base/ADAPTER).read_text()
    required_checks = event_assertions((base/'inputs/tb_r_front_local.sv').read_text())

    def collect(directory):
        state, row = observed(directory)
        rows.append(row)
        if row.get('verified_live'):
            pending.append(row)
        if directory not in retained:
            retain(directory,out/directory.name)
            retained.add(directory)
        return state

    replays = []
    for version in [1,2]:
        for mode in ['full','words']:
            peers = [BUILD/f'j19-{mode}-{engine}-v{version}-20261025'
                     for engine in ['icarus','verilator']]
            states = [collect(d) for d in peers]
            for d,s in zip(peers,states):
                generator = frozen_rewrite(d)
                assert (d/ADAPTER).read_text()==generator(old_adapter)
                if version==2:
                    assert generator(old_adapter)==rewrite(old_adapter)
                assert s['experimental_j19_ordered_epochs']
                assert s['actual_native_visible_R_cells']
                assert s['original_per_event_result_owner_RAM_and_clock_comparisons_retained']
            complete = all(s['status']=='PASS_COMBINED_CP_COHERENT_PACKETS' for s in states)
            if complete:
                assert clock_outcomes(peers[0])==clock_outcomes(peers[1])
                for name in ['candidate.sv','caller.sv','native_R.sv','native_MC10173.v',ADAPTER]:
                    assert sha(peers[0]/name)==sha(peers[1]/name)
                assert len(re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',
                                      (peers[0]/'test.log').read_text(),re.M))==100
                assert states[0]['summary']==states[1]['summary']
                if mode=='full':
                    assert 'paired_writes=119 forwarded_bytes=238' in states[0]['summary']
                else:
                    assert states[0]['staged_reads']==states[1]['staged_reads']==1
            replays.append(dict(version=version,mode=mode,complete_both_engines=complete,
                                summaries=[s.get('summary') for s in states]))

    # The corrected data rule is the only HDL change between these replay
    # versions. Check the complete retained stimuli/references too, rather
    # than accepting identical summary strings as proof of the same test.
    for mode in ['full','words']:
        for engine in ['icarus','verilator']:
            before=BUILD/f'j19-{mode}-{engine}-v1-20261025'
            after=BUILD/f'j19-{mode}-{engine}-v2-20261025'
            a=read(before/'run-state.json');b=read(after/'run-state.json')
            assert a['source_trace_sha256']==b['source_trace_sha256']
            assert a['source_initial_sha256']==b['source_initial_sha256']
            old_paths={r['path'] for r in a['source_files']}
            assert old_paths=={r['path'] for r in b['source_files']}
            for name in old_paths-{ADAPTER}:
                old=(before/name).read_text().replace(str(before),'REPLAY_DIRECTORY')
                new=(after/name).read_text().replace(str(after),'REPLAY_DIRECTORY')
                assert old==new,(mode,engine,name)

    tpc = []
    for engine in ['icarus','verilator']:
        positive=BUILD/f'full-tpc-j19-epochs-{engine}-v3-20261025'
        negative=BUILD/f'full-tpc-j19-epochs-late-{engine}-v3-20261025'
        p,n=collect(positive),collect(negative)
        assert p['returncode']==0 and p['original_program_passed']
        assert n['returncode']!=0 and not n['original_program_passed']
        for d in [positive,negative]:
            assert (d/ADAPTER).read_text()==rewrite(old_adapter)
            assert not required_checks-event_assertions((d/'inputs/tb_r_front_local.sv').read_text())
        good=(positive/'test.log').read_text()
        assert 'FULL_PROCESSOR_CP_BLOCK_PASS checks=204 Qs=844 RAM=9216 clocks=100' in good
        assert 'READ_OWNER_FULL_PROCESSOR_PASS reads=1' in good
        bad=(negative/'test.log').read_text()
        assert re.findall(r'^R_FRONT_DIFF bit=(\d+)',bad,re.M)==['74']
        assert 'R front state mismatch check=202' in bad
        actual=(negative/'candidate.sv').read_text()
        for family in ['control','capture']:
            wrong='module dorado_'+family+'_hold_atomic_step #(parameter integer WAVES=24,parameter integer CP_TPC_BLOCK_EARLY=0)('
            assert actual.count(wrong)==1
            actual=actual.replace(wrong,wrong.replace('CP_TPC_BLOCK_EARLY=0','CP_TPC_BLOCK_EARLY=1'),1)
        assert actual==(positive/'candidate.sv').read_text()
        tpc.append(dict(engine=engine,checks=204,original_reads=1,negative_bit=74,negative_check=202))

    directed = []
    for stack,rstk,mutation,bit in [(255,3,'old-overflow',734),(0,8,'late-underflow',735)]:
        peers = []
        for engine in ['icarus','verilator']:
            positive = BUILD/f'j19-aperture-sp{stack}-rstk{rstk}-{engine}-v2-20261025'
            negative = BUILD/f'j19-aperture-sp{stack}-rstk{rstk}-{mutation}-{engine}-v1-20261025'
            p,n = collect(positive),collect(negative)
            assert p['returncode']==0 and p['original_program_passed']
            assert n['returncode']!=0 and not n['original_program_passed']
            for d,s in [(positive,p),(negative,n)]:
                assert s['ordinary_stack_pointer_input']==stack and s['tail_RSTK_value']==rstk
                assert s['original_staged_read_case_retained_first']
                assert s['original_per_event_assertions_preserved']
                assert not required_checks-event_assertions((d/'inputs/tb_r_front_local.sv').read_text())
                assert profile(d)==dict(gate=3200,ordinary_FF=1500,SE10231=5000)
                for path in (positive/'timed').glob('*'):
                    if path.is_file():
                        assert sha(path)==sha(d/'timed'/path.name)
            good = (positive/'test.log').read_text()
            assert 'FULL_PROCESSOR_CP_BLOCK_PASS checks=339 Qs=844 RAM=9216 clocks=100' in good
            assert 'READ_OWNER_FULL_PROCESSOR_PASS reads=1' in good
            assert f'J19_STACK_INPUT value={stack} check=329' in good
            assert len(re.findall(r'^FULL_CP_CLOCK channel=\d+ edges=\d+$',good,re.M))==100
            arrivals=re.findall(r'^TPC_ORIGINAL_ARRIVAL t=([\d.]+) check=337 root=1 .*j19_C=([01]{2}) D=([01]{2}) Q=([0-9a-f]+)$',good,re.M)
            low=next(int(round(float(t)*1000)) for t,c,d,q in arrivals if c=='00')
            high=next(int(round(float(t)*1000)) for t,c,d,q in arrivals if c=='11' and int(round(float(t)*1000))>low)
            assert high-low==1700
            if bit==735:
                data=next(int(round(float(t)*1000)) for t,c,d,q in arrivals if d=='01')
                assert data-high==1500 and all(q=='3' for t,c,d,q in arrivals)
            else:
                data=next(int(round(float(t)*1000)) for t,c,d,q in arrivals if d=='10')
                assert high-data==1700 and arrivals[-1][-1]=='2'
            bad = (negative/'test.log').read_text()
            assert re.findall(r'^R_FRONT_DIFF bit=(\d+)',bad,re.M)==[str(bit)]
            assert 'R front state mismatch check=337' in bad
            assert (positive/ADAPTER).read_text()==rewrite(old_adapter)
            actual = (negative/ADAPTER).read_text()
            if mutation=='late-underflow':
                wrong='front_next[99]=(j19_legacy_edges[1] | j19_root_edges[1])'
                right='front_next[99]=j19_legacy_edges[1]'
            else:
                wrong='front_next[98]=j19_legacy_edges[0]'
                right='front_next[98]=(j19_legacy_edges[0] | j19_root_edges[0])'
            assert actual.count(wrong)==1
            assert actual.replace(wrong,right,1)==(positive/ADAPTER).read_text()
            for row in p['source_files']:
                name=row['path']
                if name==ADAPTER:continue
                a=(positive/name).read_text().replace(str(positive),'REPLAY_DIRECTORY')
                b=(negative/name).read_text().replace(str(negative),'REPLAY_DIRECTORY')
                assert a==b,name
            peers.append(positive)
            directed.append(dict(engine=engine,stack_pointer=stack,RSTK=rstk,events=339,
                                 retained_reads=1,negative=mutation,negative_bit=bit,negative_check=337,
                                 original_sink_arrivals=arrivals))
        for name in ['transactions.hex','initial.hex']:
            assert sha(peers[0]/name)==sha(peers[1]/name)

    # Retain the real counterexample to the former shared-data-epoch rule.
    historical = []
    for engine in ['icarus','verilator']:
        failed = BUILD/f'j19-aperture-sp0-rstk8-{engine}-v1-20261025'
        accidental = BUILD/f'j19-aperture-sp0-rstk8-bad-data-{engine}-v1-20261025'
        f,a = collect(failed),collect(accidental)
        assert f['returncode']!=0 and a['returncode']==0
        assert (failed/ADAPTER).read_text()==frozen_rewrite(failed)(old_adapter)
        assert 'R_FRONT_DIFF bit=735' in (failed/'test.log').read_text()
        assert 'FULL_PROCESSOR_CP_BLOCK_PASS checks=339' in (accidental/'test.log').read_text()
        historical.append(dict(engine=engine,earlier_helper_rejected=True,
                               forced_one_was_not_a_valid_negative=True))

    for pattern in ['j19-rstk*-20261025','j19-sp*-20261025','j19-aperture*-20261025']:
        for directory in sorted(BUILD.glob(pattern)):
            if directory not in retained:
                collect(directory)

    routes = {name:route(ROOT,g,f) for name,g,f in [('inherited',1750,400),('aperture',3200,1500)]}
    assert routes['aperture']['clock_LOW_ps']==1700
    assert routes['aperture']['underflow_D_after_clock_ps']==1500
    for board in ['BaseBd-Rev-Am','ContA-Rev-Cd','ProcL-Rev-Ci']:
        src=ROOT/'chm/sil'/(board+'.dm!1_')/(board+'.wl')
        dest=out/'wirelists'/src.name;dest.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(src,dest)

    native = BUILD/'native-cp-read-cells-prefix-v1-20261025'
    state=read(native/'run-state.json')
    prov=json.loads(gzip.decompress((native/'provenance.json.gz').read_bytes()))
    verify(native/'inputs',prov['inputs'])
    assert state['status']=='TERMINAL_NATIVE_MEMORY_DIAGNOSTIC' and state['reason']=='HOST_LIMIT'
    assert state['actions'][-1]['pid']==40369 and state['actions'][-1]['returncode']==0
    assert prov['host_limit']==int((native/'host-cycle-limit.txt').read_text())==250000000
    assert not prov['paired_mode_enabled'] and not prov['assembly_permitted']
    assert prov['original_ROM_bytes_unchanged'] and len(prov['used_hdl'])==194
    log=(native/'test.log').read_text()
    assert 'NATIVE_DIAGNOSTIC_STOP reason=  HOST_LIMIT host=250000000 guest=37059037' in log
    assert 'NATIVE_PROGRESS host=250000000 guest=37059037 initialized=1' in log
    assert 'fault=00000000:00000000 tx=0 crc=0 rx=0' in log
    assert 'NATIVE_CP_PAUSE host=250000000 guest=37059037 ready=1 commits=37059038 pauses=159 snapshots=159' in log
    assert 'if(phase==5)begin ready<=1;phase<=6;end' in (native/'inputs/src/fpga/mister/dorado_storage_selftest.sv').read_text()
    medium=BUILD/'cedar-work-native-hints.pack'
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    retain(native,out/native.name)
    inspection=BUILD/'native-cp-read-startup-inspection-v2-20261025'
    i=read(inspection/'result.json');c=i['counters']
    assert i['pid']==40369 and i['read_only_guest_inspection']
    assert i['binary_sha256']==sha(native/'obj/native-cedar')
    assert i['header_sha256']==sha(native/'obj/Vtb_native_cedar___024root.h')
    assert 'Process 40369 detached' in (inspection/'lldb.log').read_text()
    assert c['host_ticks']==198687918 and c['guest_ticks']==5009907
    for name in ['initialized','storage_ready','control_ready','alias_ok','entry_ready']:
        assert c['front_control_'+name]==1
    for name in ['unsettled','repeated','read_needed','shared_mismatch','read_phase']:
        assert c['front_control_'+name]==0
    retain(inspection,out/inspection.name)
    if require_full:
        assert not pending and all(r['complete_both_engines'] for r in replays)
    result=dict(status='REVIEWED_SEPARATE_J19_DATA_EPOCHS_AND_TERMINAL_NATIVE_PREFIX',
                scope=__doc__,runs=rows,pending_replays=pending,replays=replays,
                directed=directed,current_adapter_TPC_gate=tpc,rejected_historical_epoch_rule=historical,
                routes=routes,actual_stored_bits_per_event=844,SRAM_bits_per_event=9216,
                clock_channels=100,original_result_bits=2470,new_owner_bits=0,new_RAM_ports=0,
                replay_versions_share_exact_stimuli_and_all_other_HDL=True,
                original_staged_read_retained_first=True,
                new_j19_adapter_selected_in_native=False,
                native_prefix=dict(pid=40369,host_ticks=250000000,enabled_machine_ticks=37059037,
                                   reason='HOST_LIMIT',memory_selftest_passes=6,pauses=159,
                                   normal_boot_presses_reached=False,firmware_qualified=False),
                native_read_only_inspection=c,arbitrary_arrivals_qualified=False,
                full_processor_unknowns_qualified=False,physical_pulse_acceptance_qualified=False,
                native_paired_mode_qualified=False,physical_timing_qualified=False,
                original_firmware_Cedar_qualified=False,new_RBF_ready=False,
                hardware_changed=False,hardware_speed_gain_measured=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--archive',type=Path)
    parser.add_argument('--require-full',action='store_true')
    parser.add_argument('--label',default='j19-separate-data-epochs')
    args=parser.parse_args()
    result=review(args.output.resolve(),args.require_full)
    if args.archive:
        append_archive(args.archive.resolve(),args.output.resolve(),[],label=args.label,
                       extra_tools=['generate_j19_capture.py','probe_full_tpc_capture.py',
                                    'review_j19_data_epochs.py'])
    print(result['status'], 'pending='+str(len(result['pending_replays'])))
