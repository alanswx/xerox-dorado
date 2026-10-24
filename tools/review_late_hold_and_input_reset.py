#!/usr/bin/env python3
"""Preserve rejected component timing and the native boot-input reset repair.

No timing exception, result field removal or FPGA deployment is authorized.
Native tests are observations of specific live processes, not boot passes.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil

from fit_r_front_late_hold import gate
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(path):return json.loads(path.read_text())


def retain(source,dest):
    """Copy complete text/source receipts, excluding build databases/binaries."""
    for path in source.rglob('*'):
        rel=path.relative_to(source)
        if not path.is_file()or any(x in rel.parts for x in ['obj','db','incremental_db']):continue
        if path.suffix in ['.o','.a','.vvp','.rbf','.sof','.pack']or path.name in ['test','layout','symbols','inspect']:continue
        raw=path.read_bytes()
        assert not raw.startswith((b'\x7fELF',b'\xcf\xfa\xed\xfe')),path
        target=dest/rel;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)


def first_path(path):
    text=path.read_text()
    slack=float(re.search(r'Worst case slack is ([+-]?[\d.]+)',text)[1])
    def field(name):
        return re.search(r'^; '+name+r'\s*; ([^;]+);',text,re.M)[1].strip()
    return dict(slack_ns=slack,source=field('From Node'),sink=field('To Node'),
                arrival_ns=float(field('Data Arrival Time')),required_ns=float(field('Data Required Time')),
                report_sha256=sha(path))


def review(out):
    assert not out.exists();out.mkdir(parents=True)
    gates=[gate(BUILD/f'r-front-late-hold-all-{e}-v3-20261023',e)[1]for e in ['icarus','verilator']]
    fit=BUILD/'r-front-late-hold-fit-terminal-v2-20261023';state=read(fit/'run-state.json')
    assert [(v['stage'],v['returncode'])for v in state['stages']]==[('map',0),('fit',0),('sta',0),('corners',3)]
    prov=read(fit/'provenance.json')
    for name,h in prov['files'].items():
        raw=(fit/name).read_bytes()
        if name=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        assert hashlib.sha256(raw).hexdigest()==h,name
    assert prov['result_bits']==1400 and prov['physical_Q_bits']==844 and prov['clock_period_ns']==15
    query=BUILD/'r-front-late-hold-flat-query-v5-20261023'
    assert read(query/'flat-v5-run-state.json')['status']=='TERMINAL_REPORT_PASS'
    coverage=re.findall(r'LATE_HOLD_COVERAGE launch=(\d+) results=(\d+) late=(\d+) owners=(\d+)',(query/'flat-v5-report.log').read_text())
    assert coverage==[('1400','1193','1','844')]*4
    corners=[]
    reports=query/'late-hold-flat-v5-reports'
    for condition in ['7_slow_1100mv_100c','7_slow_1100mv_-40c','MIN_fast_1100mv_100c','MIN_fast_1100mv_-40c']:
        row=dict(condition=condition)
        for key,prefix in [('global','clock-setup'),('owners','clock-owner-results'),('late_owners','clock-late-hold-owners'),('hold','clock-hold')]:
            row[key]=first_path(reports/(prefix+'-'+condition+'.rpt'))
        ucp=(reports/('clock-ucp-'+condition+'.rpt')).read_text()
        counts=re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)',ucp)
        assert len(counts)==5 and all(pair==('0','0')for pair in counts)
        assert row['late_owners']['slack_ns']>0 and row['hold']['slack_ns']>0
        corners.append(row)
    assert min(r['global']['slack_ns']for r in corners)==-17.177
    assert min(r['owners']['slack_ns']for r in corners)==-16.283
    assert min(r['late_owners']['slack_ns']for r in corners)==7.047
    assert corners[0]['global']['source']=='launch[686]'
    assert corners[0]['global']['sink']=='result[355]~reg0'
    assert corners[0]['owners']['source']=='launch[686]'
    assert corners[0]['owners']['sink']=='result[1150]~reg0'
    assert all(r['late_owners']['source']=='launch[388]'for r in corners)
    retain(fit,out/fit.name);retain(query,out/query.name)
    history=[]
    for name in ['r-front-late-hold-fit-failed-v1-20261023','r-front-late-hold-flat-query-v3-20261023','r-front-late-hold-flat-query-v4-20261023']:
        source=BUILD/name;retain(source,out/name)
        history.append(dict(directory=name,scope='Preserved terminal parser/report-query failure'))
    proof=BUILD/'keyboard-stopped-reset-v4-20261023';p=read(proof/'run-state.json')
    assert p['status']=='PASS_STOPPED_INPUT_RESET_WITH_NEGATIVE_CONTROLS'and len(p['cases'])==16
    for name,h in p['inputs'].items():assert sha(proof/'inputs'/name)==h,name
    for name in ['dorado_keyboard.v','dorado_terminal.v']:
        assert sha(ROOT/'verilog/cells'/name)==p['inputs'][name]
    for row in p['cases']:
        log=proof/(row['engine']+'-'+row['mode']+'-test.log')
        assert sha(log)==row['log_sha256']
        assert (row['test_returncode']!=0)==row['expected_rejection']
    retain(proof,out/proof.name)
    for version in [1,2,3]:
        source=BUILD/f'keyboard-stopped-reset-v{version}-20261023';retain(source,out/source.name)
    inspection=BUILD/'native-cedar-queue-keyboard-read-v1-20261023';r=read(inspection/'result.json')
    assert r['pid']==29618 and r['read_only_guest_inspection']
    assert 'Process 29618 detached'in(inspection/'lldb.log').read_text()
    c=r['counters'];assert c['keyboard_ps2_key']==c['keyboard_machine_key']==0x621
    assert all(c['keyboard_word'+str(i)]==0 for i in range(4))
    retain(inspection,out/inspection.name)
    live=[]
    for kind in ['5c65','queue']:
        failed=BUILD/f'native-cedar-{kind}-input-reset-v1-20261023';f=read(failed/'run-state.json')
        assert f['status']=='TERMINAL_FAILED'and f['actions'][-1]['returncode']!=0
        assert 'Native input reset matrix 0000 0000 0000 0000'in(failed/'test.log').read_text()
        retain(failed,out/failed.name)
        for suffix in ['skip-wait-v1','input-reset-v2']:
            source=BUILD/f'native-cedar-{kind}-{suffix}-20261023'
            obs=out/('observed-'+source.name);record=observation(source,obs)
            for name in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
                shutil.copy2(source/name,obs/name)
            assert int((source/'host-cycle-limit.txt').read_text())==5000000000
            n=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
            for name,h in n['inputs'].items():assert sha(source/'inputs'/name)==h,name
            lines=(source/'test.log').read_text().splitlines()
            special=[s for s in lines if any(v in s for v in ['NATIVE_BOOT_REQUEST','NATIVE_C_RELEASED','NATIVE_INPUT_RESET_PASS'])]
            (obs/'boot-and-reset-events.log').write_text('\n'.join(special)+'\n')
            record['boot_and_reset_events']=special
            if suffix=='input-reset-v2':
                assert any('NATIVE_INPUT_RESET_PASS'in s for s in special)
                # Freeze the new compiled input source/provenance; no simulator
                # objects or mutable process state are copied as a snapshot.
                retain(source/'inputs',obs/'inputs')
            live.append(record)
    result=dict(status='REVIEWED_LATE_HOLD_TIMING_REJECTED_AND_INPUT_RESET_REPAIRED',scope=__doc__,
                component_gates=gates,fit_source_provenance_sha256=sha(fit/'provenance.json'),
                corners=corners,fit_parser_and_query_history=history,
                full_result_fields=1400,synthesized_result_registers=1193,all_owner_registers=844,
                timing_exceptions_added=False,fields_removed=False,fit_or_native_timing_qualified=False,
                input_reset_proof_sha256=sha(proof/'run-state.json'),input_reset_cases=p['cases'],
                observed_original_keyboard=dict(inspection=inspection.name,pid=29618,counters=c),
                native_observations=live,original_jobs_or_bounds_changed=False,
                new_native_tests_are_fresh_changed_source_experiments=True,
                firmware_qualified=False,desktop_qualified=False,hardware_changed=False,speedup_qualified=False,
                full_goal_active=True,full_FPGA_snapshots_deferred=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


def correct_metadata(source,out):
    """Correct the first receipt's column-header matches; no experiment reruns."""
    assert not out.exists();out.mkdir(parents=True)
    original=source/'results.json';result=read(original)
    reports=BUILD/'r-front-late-hold-flat-query-v5-20261023/late-hold-flat-v5-reports'
    for row in result['corners']:
        for key,prefix in [('global','clock-setup'),('owners','clock-owner-results'),('late_owners','clock-late-hold-owners'),('hold','clock-hold')]:
            prior=row[key];actual=first_path(reports/(prefix+'-'+row['condition']+'.rpt'))
            assert prior['source']=='To Node'and prior['sink']=='Launch Clock'
            assert all(prior[v]==actual[v]for v in ['slack_ns','arrival_ns','required_ns','report_sha256'])
            assert actual['source'].startswith('launch[')
            row[key]=actual
    assert result['corners'][0]['global']['sink']=='result[355]~reg0'
    assert result['corners'][0]['owners']['sink']=='result[1150]~reg0'
    assert all(r['late_owners']['source']=='launch[388]'for r in result['corners'])
    result['path_metadata_correction']=dict(superseded_results_sha256=sha(original),
        reason='Anchor metadata fields at the start of a detail row; the original parser matched the summary table column labels.',
        report_source_slack_or_qualification_changed=False,experiments_rerun=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


def final_input(out):
    """Review the final synthesizable reset implementation and fresh handles."""
    assert not out.exists();out.mkdir(parents=True)
    proof=BUILD/'keyboard-stopped-reset-v5-20261023';p=read(proof/'run-state.json')
    assert p['status']=='PASS_STOPPED_INPUT_RESET_WITH_NEGATIVE_CONTROLS'and len(p['cases'])==16
    for name,h in p['inputs'].items():assert sha(proof/'inputs'/name)==h,name
    for case in p['cases']:
        log=proof/(case['engine']+'-'+case['mode']+'-test.log')
        assert sha(log)==case['log_sha256']
        assert (case['test_returncode']!=0)==case['expected_rejection']
    synthesis=BUILD/'input-reset-quartus-map-v2-20261023';s=read(synthesis/'run-state.json')
    assert s['status']=='PASS_INPUT_ADAPTER_SYNTHESIS'and s['returncode']==0
    for name,h in read(synthesis/'inputs.json').items():assert sha(synthesis/name)==h,name
    for name in ['dorado_keyboard.v','dorado_terminal.v']:
        assert sha(ROOT/'verilog/cells'/name)==p['inputs'][name]==sha(synthesis/name)
    log=(synthesis/'map.log').read_text()
    assert 'Analysis & Synthesis was successful. 0 errors, 9 warnings'in log
    assert "can't infer register"not in log
    failed=BUILD/'input-reset-quartus-map-v1-20261023'
    assert read(failed/'run-state.json')['status']=='TERMINAL_SYNTHESIS_FAILED'
    assert 'Error (10818)'in(failed/'map.log').read_text()
    for source in [proof,synthesis,failed,BUILD/'native-input-reset-superseded-v2-20261023']:
        retain(source,out/source.name)
    live=[];superseded=[]
    for kind in ['5c65','queue']:
        prior=BUILD/f'native-cedar-{kind}-input-reset-v2-20261023';r=read(prior/'run-state.json')
        assert r['status']=='TERMINAL_FAILED'and r['actions'][-1]['returncode']==-15
        retain(prior,out/prior.name)
        superseded.append(dict(directory=prior.name,test_pid=r['actions'][-1]['pid'],returncode=-15,
                               reason='Known Quartus-rejected input-adapter source; no observation timeout or bound extension'))
        source=BUILD/f'native-cedar-{kind}-input-reset-v3-20261023';obs=out/('observed-'+source.name)
        record=observation(source,obs)
        assert int((source/'host-cycle-limit.txt').read_text())==5000000000
        prov=json.loads(gzip.decompress((source/'provenance.json.gz').read_bytes()))
        for name,h in prov['inputs'].items():assert sha(source/'inputs'/name)==h,name
        assert prov['input_reset_proof_sha256']==sha(proof/'run-state.json')
        assert prov['input_reset_synthesis_sha256']==sha(synthesis/'run-state.json')
        for name in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
            shutil.copy2(source/name,obs/name)
        retain(source/'inputs',obs/'inputs')
        logs=(source/'test.log').read_text()
        assert 'NATIVE_INPUT_RESET_PASS host=6 guest=0'in logs
        record['cold_reset_assertion_passed']=True;live.append(record)
    originals=[]
    for kind in ['5c65','queue']:
        source=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023'
        originals.append(observation(source,out/('observed-original-'+kind)))
    result=dict(status='REVIEWED_SYNTHESIZABLE_STOPPED_INPUT_RESET_AND_FRESH_NATIVE_TESTS',scope=__doc__,
        proof_sha256=sha(proof/'run-state.json'),cases=p['cases'],synthesis_state_sha256=sha(synthesis/'run-state.json'),
        final_adapter_hashes={name:sha(synthesis/name)for name in ['dorado_keyboard.v','dorado_terminal.v']},
        synthesis_scope='Isolated Cyclone V adapters only; no place/route, timing or full core qualification',
        final_register_implementation='Clock strobe history separately; constant asynchronous reset branches including mouse_taken',
        superseded_native_tests=superseded,native_observations=live,original_native_observations=originals,
        firmware_qualified=False,desktop_qualified=False,physical_timing_qualified=False,
        hardware_changed=False,speedup_qualified=False,original_bounded_jobs_changed=False,full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['output','archive']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--correct-metadata',type=Path)
    parser.add_argument('--final-input-synthesis',action='store_true')
    args=parser.parse_args()
    assert not(args.correct_metadata and args.final_input_synthesis)
    if args.final_input_synthesis:
        result=final_input(args.output.resolve());label='input-reset-synthesis'
    elif args.correct_metadata:
        result=correct_metadata(args.correct_metadata.resolve(),args.output.resolve());label='late-hold-timing-input-reset-paths'
    else:
        result=review(args.output.resolve());label='late-hold-timing-input-reset'
    append_archive(args.archive.resolve(),args.output.resolve(),[],label=label,extra_tools=[
        'probe_r_front_late_hold.py','fit_r_front_late_hold.py','report_r_front_late_hold_fit.py',
        'inspect_native_baseboard_process.py','test_keyboard_stopped_reset.py','prepare_native_input_reset.py',Path(__file__).name])
    print(result['status'],flush=True)
