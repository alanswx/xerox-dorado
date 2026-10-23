#!/usr/bin/env python3
"""Review original f14 event sharing and physical T-window experiments.

Separate source/physical-state comparisons from the isolated15ns budget and
the still-open native producer/phase/timer/SDRAM/full-boot acceptance gates.
Retain all functional observations; current physical parity is not a forecast
of the next transaction's future combinational result.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import fit_control_clock as fixture
from fit_r_front_control import FIELDS
from review_r_front_control_native import archive
from test_r_front_read_window_matrix import check as read_check
from test_r_front_task_event_matrix import check as task_check
from test_r_front_t_window_matrix import check as t_check
from test_r_front_current_memory_matrix import check as memory_check
from test_r_front_task_event import MODULES,adapter,candidate as task_candidate
from test_r_front_t_windows import candidate as t_candidate,t_file,t_parity,bench
from test_r_front_current_memory import memory

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def read(d):return json.loads((d/'run-state.json').read_text())


def timing(d,component):
    r=read(d);assert r['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [s['stage']for s in r['stages']]==['map','fit','sta','corners']and all(s['returncode']==0 for s in r['stages'])
    p=json.loads((d/'provenance.json').read_text());assert p['component']==component and p['clock_period_ns']==15 and not p['assembly_permitted']
    for n,h in p['files'].items():
        raw=(d/n).read_bytes()
        if n=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        assert hashlib.sha256(raw).hexdigest()==h,n
    assert(d/'clock.sdc').read_text()==fixture.SDC
    c=json.loads((d/'collected-input-check.json').read_text());assert c['status']=='TERMINAL_INPUT_READBACK'
    assert len(c['job_pids_absent'])==5 and c['changed_inputs']==['clock_fit.qsf']
    corners=re.findall(r'CONTROL_CLOCK_CORNER (\S+)',(d/'corners.log').read_text());assert len(corners)==len(set(corners))==4
    values=[]
    for corner in corners:
        row=dict(corner=corner)
        for mode in ['setup','hold','mpw','evaluator']:
            slacks=[float(x)for x in re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)',(d/f'clock-{mode}-{corner}.rpt').read_text())];assert slacks
            row[mode]=min(slacks)
        ucp=(d/f'clock-ucp-{corner}.rpt').read_text()
        for label in ['Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths']:
            assert re.search(r'; '+re.escape(label)+r'\s*; 0\s*; 0\s*;',ucp),(corner,label)
        values.append(row)
    fields=[(n,w)for n,w in FIELDS if n not in ['source_pins','fa_n']];assert sum(w for n,w in fields)==1400
    def path(mode,corner):
        report=(d/f'clock-{mode}-{corner}.rpt').read_text()
        start=re.search(r'^; From Node[ \t]+; ([^;\n]+);',report,re.M)[1].strip();end=re.search(r'^; To Node[ \t]+; ([^;\n]+);',report,re.M)[1].strip()
        launch=re.search(r'launch\[(\d+)\]',start);launch=int(launch[1])if launch else None
        result=re.search(r'result\[(\d+)\]',end);result=int(result[1])if result else None
        bit=0;endpoint=None
        for n,w in reversed(fields):
            if result is not None and bit<=result<bit+w:endpoint=dict(field=n,bit=result-bit)
            bit+=w
        return dict(corner=corner,from_node=start,to_node=end,launch_bit=launch,state_bit=launch-806 if launch is not None and launch>=806 else None,
                    result_bit=result,endpoint=endpoint,data_delay_ns=float(re.search(r'; Data Delay\s*;\s*(\d+\.\d+)\s*;',report)[1]),
                    logic_levels=int(re.search(r'; Number of Logic Levels\s*;\s*;\s*(\d+)\s*;',report)[1]))
    worst=path('setup',min(values,key=lambda v:v['setup'])['corner'])
    evaluator=path('evaluator',min(values,key=lambda v:v['evaluator'])['corner'])
    fit=(d/'clock_fit.fit.rpt').read_text(encoding='latin-1');assert re.search(r'; Total MLAB memory bits\s*; 9,216\s*;',fit)
    summary=(d/'clock_fit.fit.summary').read_text()
    area=int(re.search(r'Logic utilization \(in ALMs\) : ([\d,]+)',summary)[1].replace(',',''))
    regs=int(re.search(r'Total registers : (\d+)',summary)[1]);assert'Total block memory bits : 0 /'in summary
    return dict(directory=d.name,corners=values,setup_min=min(x['setup']for x in values),hold_min=min(x['hold']for x in values),mpw_min=min(x['mpw']for x in values),
                worst=worst,evaluator_worst=evaluator,
                ALMs=area,registers=regs,MLAB_bits=9216,M10K_bits=0,unconstrained_paths=0,constraints_unchanged=True,
                isolated_budget_pass=all(x[m]>=0 for x in values for m in ['setup','hold','mpw']),
                native_timing_qualified=False,ordinary60ns_qualified=False,speedup_qualified=False)


def verify_transform(d,kind):
    r=read(d);parent=Path(r['parent']);old=read(parent)
    assert sha(parent/'run-state.json')==r['parent_run_state_sha256']
    assert old['actions'][-1]['returncode']==0
    changed=0
    for row in r['source_files']:
        p=d/row['path'];q=parent/row['path'];assert sha(p)==row['output_sha256']and sha(q)==row['parent_sha256']
        raw=q.read_text();expected=raw
        if kind=='task':
            if p.name=='candidate.sv':expected=task_candidate(raw,r['mutation'])
            for name,module in MODULES.items():
                if p.name==module+'.sv':expected,count=adapter(raw,name)
        elif kind=='T':
            if p.name=='candidate.sv':expected=t_candidate(raw,r['mutation'])
            elif p.name=='dorado_local_t_file_step.sv':expected=t_file(raw)
            elif p.name=='dorado_local_t_parity_step.sv':expected=t_parity(raw)
            elif p.name=='tb_r_front_local.sv':expected=bench(raw)
        else:
            assert kind=='memory'
            if p.name=='dorado_local_r_memory_step.sv':expected=memory(raw,r['mutation'])
        assert p.read_text()==expected,('Unaccounted source change',p)
        assert row['changed']==(expected!=raw);changed+=row['changed']
    assert changed=={'task':10,'T':4,'memory':1}[kind]
    text=(d/'test.log').read_text()
    if r['mutation']:
        assert r['actions'][-1]['returncode']!=0
        if kind=='task':assert'R_FRONT_LOCAL_SOLVER check=17 shared=1'in text and'R front solver unsettled/repeated'in text
        elif kind=='T':assert'R front state mismatch check=7733'in text and'R_FRONT_DIFF bit=402'in text
        else:assert'R front storage mismatch check=203'in text and'R_FRONT_RAM_DIFF bit=2040'in text
    else:
        assert r['actions'][-1]['returncode']==0 and r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        assert len(re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M))==100
    return dict(directory=d.name,status=r['status'],mutation=r['mutation'],source_transform_exact=True,run_state_sha256=sha(d/'run-state.json'))


def matrix(d,checker,expected_status):
    r=read(d);assert r['status']==expected_status and len(r['cases'])==12 and not r['errors']
    for row in r['cases']:assert checker(ROOT/row['directory'])['run_state_sha256']==row['run_state_sha256']
    for n,h in r['source_hashes'].items():assert sha(ROOT/n)==h
    return r


def review(out,task_fit,t_fit,memory_fit):
    assert not out.exists();dirs=[]
    d=BUILD/'r-front-read-windows-matrix-v2-20261023';m=matrix(d,read_check,'PASS_R_FRONT_READ_WINDOW_PROFILE_MATRIX')
    dirs +=[ROOT/r['directory']for r in m['cases']]+[d]
    task=[];tw=[];mw=[]
    for engine in ['icarus','verilator']:
        for variant in ['', 'omit-']:
            d=BUILD/f'r-front-task-event-{variant}{engine}-v2-20261023';task.append(verify_transform(d,'task'));dirs.append(d)
        for variant in ['', 'omit_t_close-', 'hide_open_t-']:
            d=BUILD/f'r-front-t-windows-{variant}{engine}-v1-20261023';tw.append(verify_transform(d,'T'));dirs.append(d)
        for variant in ['', 'wrong-']:
            d=BUILD/f'r-front-current-memory-{variant}{engine}-v1-20261023';mw.append(verify_transform(d,'memory'));dirs.append(d)
    manifest=BUILD/'r-front-control-reference-v2-20261023/manifest.json';reference=json.loads(manifest.read_text())
    assert reference['boards'][0]['clock_taps'][12]=='@f14.ca'
    assert reference['boards'][0]['sequential']['f14']==[40,2]
    audit=BUILD/'r-front-control-clock-audit-v1-20261023/results.json';a=json.loads(audit.read_text());assert a['channels'][22]==dict(channel=22,board='ContA',tap='@f14.ca')
    complete=[];pending=[]
    for name,checker,status,kind in [('r-front-task-event-matrix-v1-20261023',task_check,'PASS_R_FRONT_TASK_EVENT_PROFILE_MATRIX','task'),
                                      ('r-front-t-windows-matrix-v1-20261023',t_check,'PASS_R_FRONT_T_WINDOW_PROFILE_MATRIX','T'),
                                      ('r-front-current-memory-matrix-v1-20261023',memory_check,'PASS_R_FRONT_CURRENT_MEMORY_PROFILE_MATRIX','memory')]:
        d=BUILD/name;r=read(d)
        if r['status']==status:
            matrix(d,checker,status)
            for row in r['cases']:
                case=ROOT/row['directory'];verify_transform(case,kind);dirs.append(case)
            dirs.append(d);complete.append(dict(directory=name,status=status,cases=r['cases']))
        else:
            assert r['status']=='RUNNING'and not r['errors']
            # A pending run is not archived or counted as qualified. The
            # caller must inspect its actual process before calling it live.
            pending.append(dict(directory=name,status='PENDING_NOT_QUALIFIED',recorded_driver=r['pid'],completed_cases=len(r['cases'])))
    tt=timing(task_fit,'r_front_control_canonical_task_event');wt=timing(t_fit,'r_front_control_T_windows')
    mt=timing(memory_fit,'r_front_control_current_memory');dirs +=[task_fit,t_fit,memory_fit]
    result=dict(status='PASS_TASK_EVENT_T_WINDOW_AND_CURRENT_MEMORY_COMPONENT_COMPARISONS',scope=__doc__,read_window_profile_matrix=m['cases'],task_cases=task,T_cases=tw,current_memory_cases=mw,
                f14_original_clock_alias=dict(joint_channel=12,child_channel=22,tap='@f14.ca',CA_state_bits=[40,41],processor_state_bits=[72,73],
                                             manifest_sha256=sha(manifest),clock_audit_sha256=sha(audit)),
                complete_matrices=complete,pending_not_qualified=pending,task_event_timing=tt,T_window_timing=wt,current_memory_timing=mt,
                physical_Qs=844,R_RAM_bits=9216,clock_channels=100,T_closed_encoding_bits=48,R_closed_encoding_bits=32,
                T_status_from_current_physical_Q=True,all_functional_observations_preserved=True,
                full_arrival_contract_qualified=False,native_firmware_qualified=False,ordinary60ns_qualified=False,hardware_speedup_claimed=False,
                next='Finish full profiles; follow actual failing capture path, qualify native live/closed ownership and original60ns phases, timers, SDRAM and full A-F acceptance.')
    out.mkdir(parents=True);(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,list(dict.fromkeys(dirs))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['output','task-fit','t-fit','memory-fit']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--archive',type=Path);a=p.parse_args();result,dirs=review(a.output.resolve(),a.task_fit.resolve(),a.t_fit.resolve(),a.memory_fit.resolve());print(result['status'])
    if a.archive:archive(a.archive.resolve(),a.output.resolve(),dirs,[Path(__file__).name,'test_r_front_task_event.py','test_r_front_task_event_matrix.py','fit_r_front_task_event.py','test_r_front_t_windows.py','test_r_front_t_window_matrix.py','fit_r_front_t_windows.py','test_r_front_current_memory.py','test_r_front_current_memory_matrix.py','fit_r_front_current_memory.py'])
