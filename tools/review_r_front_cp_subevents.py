#!/usr/bin/env python3
"""Review bounded CP ordering passes and preserve actual live profile handles.

This is component evidence, not native timing, boot or speed qualification.
Keep rejected earlier candidates and negative controls. Archive terminal source
trees only; copy immutable observations for jobs which are actually live.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def source_check(d,r):
    for row in r['source_files']:
        assert sha(d/row['path'])==row['output_sha256'],(d,row['path'])
        if row['path']=='inputs/tb_r_front_local.sv':assert not row['changed']
        if row['path'].startswith('timed/'):
            assert row['output_sha256']==(row.get('profile_source_sha256') or row['parent_sha256'])
    assert not r['extra_RAM_or_guest_cycle'] and not r['extra_architectural_state']
    assert not r['qualifying_native_timing'] and not r['assembly_permitted']


def observation(d,dest):
    r=read(d/'run-state.json');assert r['status']=='RUNNING'
    action=r['actions'][-1] if r.get('actions') else dict(pid=r['pid'])
    # A matrix may currently be reusing its live balanced case; inspect its
    # driver instead if its most recent recorded subprocess is terminal.
    pid=r['pid'] if action.get('returncode') is not None else action['pid']
    ps=subprocess.check_output(['ps','-p',str(pid),'-o','pid=,etime=,state=,command='],text=True)
    assert d.name in ps,(d,pid,ps)
    dest.mkdir(parents=True)
    (dest/'run-state.json').write_text(json.dumps(r,indent=2)+'\n')
    (dest/'ps.txt').write_text(ps)
    if (d/'test.log').exists():
        p=d/'test.log'
        with p.open('rb')as stream:
            stream.seek(max(0,p.stat().st_size-30000));raw=stream.read()
        if raw and not raw.endswith(b'\n'):raw=raw[:raw.rfind(b'\n')+1]
        (dest/'test-prefix-tail.log').write_bytes(raw)
    return dict(directory=d.name,pid=pid,ps=ps.strip(),observed=time.time(),state_sha256=sha(dest/'run-state.json'))


def review(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];passes=[];rejects=[];live=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-front-cp-subevents-{engine}-v3-20261023';r=read(d/'run-state.json');source_check(d,r)
        assert r['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS' and r['actions'][-1]['returncode']==0
        assert r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
        assert r['original_reference_and_checks_unchanged'] and r['task_CP_subevent']
        assert r['processor_CP_subevent'] and r['canonical_task_clock_stage_ownership']
        log=(d/'test.log').read_text()
        audit=re.findall(r'ALL_ADDRESS_PIN_AUDIT group=(\d+) package=(\S+) edges=(\d+) distinct=(\d+)',log)
        assert len(audit)==8 and sum(int(e)for i,p,e,n in audit)==28700
        assert all(int(n)==0 for i,p,e,n in audit)
        passes.append(dict(engine=engine,directory=d.name,summary=r['summary'],address_pin_captures=audit,
                           state_sha256=sha(d/'run-state.json')));dirs.append(d)
    for engine,version in [('icarus',1),('verilator',1),('verilator',2)]:
        d=BUILD/f'r-front-cp-subevents-{engine}-v{version}-20261023';r=read(d/'run-state.json');source_check(d,r)
        assert r['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED' and r['actions'][0]['returncode']==0
        assert r['actions'][-1]['returncode']!=0
        assert 'R_FRONT_LOCAL_SOLVER check=12839 shared=0' in (d/'test.log').read_text()
        rejects.append(dict(directory=d.name,event=12839,scope='Earlier incomplete subevent composition',state_sha256=sha(d/'run-state.json')));dirs.append(d)
    for engine in ['icarus','verilator']:
        for mutation,event in [('root-first',11327),('task-root-first',12839)]:
            d=BUILD/f'r-front-cp-subevents-{mutation}-{engine}-v3-20261023';r=read(d/'run-state.json');source_check(d,r)
            if r['status']=='RUNNING':
                live.append(observation(d,out/('observed-'+d.name)));continue
            assert r['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED' and r['actions'][0]['returncode']==0
            assert r['actions'][-1]['returncode']!=0
            assert f'R_FRONT_LOCAL_SOLVER check={event} ' in (d/'test.log').read_text()
            rejects.append(dict(directory=d.name,event=event,scope='Ordering negative control',state_sha256=sha(d/'run-state.json')));dirs.append(d)
    matrix=BUILD/'r-front-cp-subevents-matrix-v1-20261023';m=read(matrix/'run-state.json')
    for n,h in m['source_hashes'].items():assert sha(ROOT/n)==h,('Live matrix driver changed',n)
    assert not m['errors']
    for row in m['cases']:
        d=ROOT/row['directory'];r=read(d/'run-state.json');source_check(d,r)
        assert r['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS' and r['actions'][-1]['returncode']==0
        if d not in dirs:dirs.append(d)
    if m['status']=='RUNNING':live.append(observation(matrix,out/'observed-profile-matrix'))
    else:
        assert m['status']=='PASS_CP_SUBEVENT_PROFILE_MATRIX' and len(m['cases'])==12
        dirs.append(matrix)

    native=[]
    for kind in ['5c65','queue']:
        d=BUILD/f'native-cedar-{kind}-skip-wait-v1-20261023';dest=out/('observed-native-'+kind)
        row=observation(d,dest)
        assert int((d/'host-cycle-limit.txt').read_text())==5000000000
        for name in ['provenance.json.gz','host-cycle-limit.txt','window-control.txt']:
            (dest/name).write_bytes((d/name).read_bytes())
        r=json.loads(gzip.decompress((d/'provenance.json.gz').read_bytes()))
        for name,h in r['inputs'].items():assert sha(d/'inputs'/name)==h,(kind,name)
        tail=(dest/'test-prefix-tail.log').read_text(errors='replace')
        row['FinishCP_observations']=re.findall(r'^NATIVE_CP_WRITE[^\n]*pc6502=f8a[5-9ab][^\n]*',tail,re.M)
        row.update(kind=kind,bound=5000000000)
        native.append(row)
    assert native[1]['FinishCP_observations'],'Queued original-ROM firmware CP progress not observed'
    result=dict(status='REVIEWED_CP_SUBEVENT_COMPONENT_PASSES',scope=__doc__,balanced_passes=passes,
                rejected_experiments=rejects,profile_matrix_status=m['status'],profile_cases_at_observation=m['cases'],
                live_component_observations=live,native_observations=native,distinct_address_epoch_samples=0,
                complete_profile_matrix_qualified=m['status']=='PASS_CP_SUBEVENT_PROFILE_MATRIX',
                native_integrated=False,native_CP_root_qualified=False,physical_timing_qualified=False,
                firmware_qualified=False,assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


def finish_negatives(out):
    assert not out.exists();out.mkdir(parents=True);rows=[];dirs=[]
    for engine in ['icarus','verilator']:
        for mutation,event in [('root-first',11327),('task-root-first',12839)]:
            d=BUILD/f'r-front-cp-subevents-{mutation}-{engine}-v3-20261023';r=read(d/'run-state.json');source_check(d,r)
            assert r['status']=='CP_BEFORE_ROOT_SUBEVENTS_FAILED' and r['actions'][0]['returncode']==0
            assert r['actions'][-1]['returncode']!=0
            assert f'R_FRONT_LOCAL_SOLVER check={event} ' in (d/'test.log').read_text()
            rows.append(dict(engine=engine,mutation=mutation,event=event,directory=d.name,
                             state_sha256=sha(d/'run-state.json'),test_sha256=sha(d/'test.log'),
                             candidate_sha256=sha(d/'candidate.sv')))
            # Verilator trees are already in the primary subevent receipt.
            if engine=='icarus':dirs.append(d)
    for mutation in ['root-first','task-root-first']:
        pair=[r for r in rows if r['mutation']==mutation]
        assert pair[0]['candidate_sha256']==pair[1]['candidate_sha256']
    result=dict(status='REVIEWED_CP_SUBEVENT_NEGATIVE_CONTROLS_BOTH_ENGINES',
                negative_controls=rows,original_checks_unchanged=True,native_timing_qualified=False,
                assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


def finish_matrix(out):
    assert not out.exists();out.mkdir(parents=True)
    d=BUILD/'r-front-cp-subevents-matrix-v1-20261023';r=read(d/'run-state.json')
    assert r['status']=='CP_SUBEVENT_PROFILE_MATRIX_FAILED' and len(r['cases'])==7
    assert r['errors']==[dict(engine='icarus',error="AssertionError(('icarus', 'fast_gates', 1))")]
    assert all('returncode' in a for a in r['actions'])
    for n,h in r['source_hashes'].items():assert sha(ROOT/n)==h
    failure=d/'fast_gates-icarus';f=read(failure/'run-state.json');source_check(failure,f)
    assert f['actions'][0]['returncode']==0 and f['actions'][-1]['returncode']!=0
    log=(failure/'test.log').read_text()
    assert 'R_FRONT_DIFF bit=88 old=0 expected=0 original=1' in log
    assert 'R front state mismatch check=12839' in log
    good=d/'fast_gates-verilator';g=read(good/'run-state.json');source_check(good,g)
    assert g['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS' and g['actions'][-1]['returncode']==0
    assert {x['path']:x['output_sha256']for x in f['source_files']}=={x['path']:x['output_sha256']for x in g['source_files']}
    pids={r['pid'],*(a['pid']for a in r['actions'])}
    for sub in d.iterdir():
        if (sub/'run-state.json').is_file():
            s=read(sub/'run-state.json');assert s['status']!='RUNNING'
            assert all('returncode'in a for a in s['actions'])
            pids.add(s['pid']);pids.update(a['pid']for a in s['actions'])
    ps=subprocess.run(['ps','-p',','.join(map(str,sorted(pids))),'-o','pid=,command='],capture_output=True,text=True)
    assert ps.returncode==1 and not ps.stdout.strip(),('Recorded matrix process remains',ps.stdout)
    (out/'recorded-processes-absent.txt').write_text(ps.stdout+ps.stderr)
    live=[];diagnostics=[];dirs=[d]
    for engine,version in [('icarus',1),('verilator',1),('verilator',2)]:
        diag=BUILD/f'r-front-cp-flag-arrival-{engine}-v{version}-20261023';s=read(diag/'run-state.json')
        if s['status']=='RUNNING':
            live.append(observation(diag,out/f'observed-flag-arrival-{engine}-v{version}'))
        else:
            assert all('returncode'in a for a in s['actions'])
            diagnostics.append(dict(directory=diag.name,status=s['status'],actions=s['actions'],
                                    state_sha256=sha(diag/'run-state.json')));dirs.append(diag)
    result=dict(status='REVIEWED_CP_PROFILE_MATRIX_REJECTION',matrix_state_sha256=sha(d/'run-state.json'),
                passed_cases=r['cases'],failed_profile='fast_gates-icarus',failed_event=12839,
                failed_physical_Q_bit=88,expected_Q=0,original_Q=1,
                paired_fast_gate_RTL_sources_byte_identical=True,paired_Verilator_case_passed=True,
                original_clock_and_state_assertions_retained=True,arrival_cause_established=False,
                live_arrival_diagnostics=live,terminal_arrival_diagnostics=diagnostics,
                native_integrated=False,physical_timing_qualified=False,
                assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


def review_arrivals(out):
    assert not out.exists();out.mkdir(parents=True);dirs=[];rows=[];live=[]
    source=BUILD/'cp-g18-setup-source-v1-20261023'
    assert (source/'page-number.txt').read_text().strip()=='129'
    text=(source/'mc10176-page.txt').read_text()
    assert 'Setup Time' in text and '2.5' in text and 'Hold Time' in text and '1.5' in text
    book=ROOT/'DoradoDocs/datasheets/Motorola_MECL_DataBook_1978.pdf'
    spec=dict(source=str(book.relative_to(ROOT)),source_sha256=sha(book),pdf_page=129,printed_page='3-82',
              part='MC10176/MC10576',minimum_setup_ns=2.5,minimum_hold_ns=1.5,
              page_image_visually_reviewed=True,extracted_files={p.name:sha(p)for p in source.iterdir()if p.is_file()})
    (out/'original-part-timing.json').write_text(json.dumps(spec,indent=2)+'\n');dirs.append(source)
    for engine,version in [('icarus',1),('verilator',2)]:
        d=BUILD/f'r-front-cp-flag-arrival-{engine}-v{version}-20261023';r=read(d/'run-state.json')
        if r['status']=='RUNNING':
            live.append(observation(d,out/('observed-arrival-'+engine)));continue
        assert r['status']=='DIAGNOSTIC_CP_FLAG_ARRIVAL_OBSERVED' and r['actions'][0]['returncode']==0
        assert r['qualifying_gate'] is False and r['all_original_fatals_retained']
        for row in r['source_files']:
            assert sha(d/row['path'])==row['output_sha256']
            assert row['changed']==(Path(row['path']).name=='tb_r_front_local.sv')
        clocks=re.findall(r'CP_FLAG_CLOCK t=([\d.]+) check=12839 D=(\d) Q=(\d)',(d/'test.log').read_text())
        assert len(clocks)==1
        simultaneous=[s for s in r['arrival_lines']if f't={clocks[0][0]} 'in s and 'check=12839 'in s and 'D=0 C=1'in s]
        assert simultaneous,('No observed data/clock coincidence',engine)
        rows.append(dict(engine=engine,directory=d.name,state_sha256=sha(d/'run-state.json'),
                         clock=clocks[0],arrival_lines=r['arrival_lines'],
                         zero_setup_margin_at_trace_precision=True,test_returncode=r['observed_test_returncode'],
                         outcome_changed_by_observation=r['outcome_changed_by_observation']));dirs.append(d)
    assert any(r['engine']=='verilator'for r in rows)
    result=dict(status='REVIEWED_CP_FLAG_ZERO_SETUP_DIAGNOSTIC',observed_arrivals=rows,
                live_arrival_diagnostics=live,original_part_timing=spec,
                original_matrix_rejection_retained=True,reference_model_changed=False,
                valid_margin_replay_qualified=False,native_producer_setup_qualified=False,
                physical_timing_qualified=False,assembly_permitted=False,hardware_changed=False,speedup_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--finish-negatives',action='store_true',help='Append the now-terminal Icarus negative trees and compare both engines')
    p.add_argument('--finish-matrix',action='store_true',help='Preserve the terminal fast-gate rejection and actual arrival diagnostic handles')
    p.add_argument('--arrivals',action='store_true',help='Preserve observed flag arrivals and the original part setup/hold requirements')
    a=p.parse_args();assert sum([a.finish_negatives,a.finish_matrix,a.arrivals])<=1
    reviewer=review_arrivals if a.arrivals else finish_matrix if a.finish_matrix else finish_negatives if a.finish_negatives else review
    label='cp-flag-arrival'if a.arrivals else'cp-subevents-matrix'if a.finish_matrix else'cp-subevents-negatives'if a.finish_negatives else'cp-subevents'
    r,dirs=reviewer(a.output.resolve())
    append_archive(a.archive.resolve(),a.output.resolve(),dirs,label=label,extra_tools=[
        'probe_r_front_cp_subevents.py','test_r_front_cp_subevents_matrix.py','diagnose_r_front_cp_flag_arrival.py',Path(__file__).name])
    print(r['status'])
