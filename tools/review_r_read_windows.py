#!/usr/bin/env python3
"""Review task-clock reset/profile gates and transparent R latch ownership.

Keep unit proofs separate from the full844-Q integration experiment and the
still-failed15ns fit. A full-graph failure is evidence of an unfinished owner
contract, never permission to omit physical Qs, clocks, RAM or functional ports.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from review_r_front_control_native import archive
from test_r_front_control_extended_matrix import check
import fit_control_clock as fixture

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def implicit_generate(raw):
    return raw.replace(' generate for(genvar',' for(genvar',1).replace(' end endgenerate',' end',1)


def read(d):return json.loads((d/'run-state.json').read_text())


def terminal(d,success,summary=None,failure=None):
    r=read(d);assert r['status']!='RUNNING',d
    assert r['actions'][-1]['returncode']==(0 if success else 1),(d,r['actions'][-1])
    if len(r['actions'])>1:assert r['actions'][0]['returncode']==0,d
    text=(d/'test.log').read_text()
    if summary:assert summary in text,(d,summary)
    if failure:assert failure in text,(d,failure)
    return dict(directory=d.name,status=r['status'],run_state_sha256=sha(d/'run-state.json'),
                test_log_sha256=sha(d/'test.log'),summary=r.get('summary'))


def timing(d):
    r=read(d);assert r['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [s['stage']for s in r['stages']]==['map','fit','sta','corners'] and all(s['returncode']==0 for s in r['stages'])
    source=json.loads((d/'provenance.json').read_text());assert source['clock_period_ns']==15 and not source['assembly_permitted']
    assert(d/'clock.sdc').read_text()==fixture.SDC
    for n,h in source['files'].items():
        raw=(d/n).read_bytes()
        if n=='clock_fit.qsf':raw=re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$',b'',raw)
        assert hashlib.sha256(raw).hexdigest()==h,n
    syntax=source['quartus_genvar_syntax'];raw=(d/'dorado_r_read_window_step.sv').read_text()
    lowered=' genvar byte_no;\n generate for(byte_no=0;';original=' generate for(genvar byte_no=0;'
    assert raw.count(lowered)==1 and raw.replace(lowered,original,1)==(ROOT/'verilog/rtl/dorado_r_read_window_step.sv').read_text()
    assert syntax['inverse_byte_identical'] and all(a['returncode']==0 for row in syntax['comparisons']for a in row['actions'])
    for engine in ['icarus','verilator']:assert 'R_READ_WINDOW PASS checks=2304'in(d/f'unit-proof-{engine}-1.log').read_text()
    collected=json.loads((d/'collected-input-check.json').read_text());assert collected['status']=='TERMINAL_INPUT_READBACK'
    assert len(collected['job_pids_absent'])==5 and collected['changed_inputs']==['clock_fit.qsf']
    corners=re.findall(r'CONTROL_CLOCK_CORNER (\S+)',(d/'corners.log').read_text());assert len(corners)==len(set(corners))==4
    data=[]
    for c in corners:
        row=dict(corner=c)
        for mode in ['setup','hold','mpw','evaluator']:
            values=[float(x)for x in re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)',(d/f'clock-{mode}-{c}.rpt').read_text())];assert values
            row[mode]=min(values)
        ucp=(d/f'clock-ucp-{c}.rpt').read_text()
        for label in ['Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths']:
            assert re.search(r'; '+re.escape(label)+r'\s*; 0\s*; 0\s*;',ucp),(c,label)
        data.append(row)
    fit=(d/'clock_fit.fit.rpt').read_text(encoding='latin-1');assert re.search(r'; Total MLAB memory bits\s*; 9,216\s*;',fit)
    path=(d/'clock-evaluator-7_slow_1100mv_-40c.rpt').read_text()
    for text in ['launch[835]~DUPLICATE','result[339]~reg0','17.941','q[41]~11','state_next[72]~2','fault_requests~1']:assert text in path
    assert min(x['setup']for x in data)==-3.557
    return dict(directory=d.name,corners=data,setup_min=min(x['setup']for x in data),hold_min=min(x['hold']for x in data),mpw_min=min(x['mpw']for x in data),
                MLAB_bits=9216,constraints_unchanged=True,unconstrained_paths=0,
                worst=dict(launch_bit=835,launch_state_bit=29,result_bit=339,result_signal='faults[0]',logic_levels=21,data_delay_ns=17.941,
                           path='control/MIR/task-clock -> joint Switchb Q41 -> child task Q72 -> T input/visible latch -> parity fault'),
                isolated_budget_pass=all(x[m]>=0 for x in data for m in ['setup','hold','mpw']),native_timing_qualified=False,speedup_qualified=False)


def review(out,coupled,fit=None):
    assert not out.exists();dirs=[]
    matrix=BUILD/'r-front-control-tasking-mask-matrix-v1-20261023';m=read(matrix)
    assert m['status']=='PASS_TASKING_CLOCK_MASK_PROFILE_MATRIX' and len(m['cases'])==14 and not m['errors']
    for row in m['cases']:
        d=ROOT/row['directory'];assert check(d,row['extended'],True)['run_state_sha256']==row['run_state_sha256'];dirs.append(d)
    dirs.append(matrix)
    audit=BUILD/'baseboard-cp-source-audit-v1-20261023';a=json.loads((audit/'results.json').read_text())
    assert a['status']=='PASS_ORIGINAL_BASEBOARD_CP_SOURCE_AUDIT' and not a['timing_exception_justified']
    for n,h in a['inputs'].items():assert sha(audit/'inputs'/n)==h==sha(ROOT/n)
    assert a['firmware_address_write_precedes_strobe_RMW'] and len(a['nets'])==13
    dirs.append(audit)
    reset=[];negative=[]
    for engine in ['icarus','verilator']:
        names=[f'tasking-mask-reset-{engine}-v'+('2'if engine=='icarus'else'1')+'-20261023']
        names +=[f'tasking-mask-reset-{profile}-{engine}-v3-20261023'for profile in ['fast_gates','later_control','ca_later_control']]
        for name in names:
            d=BUILD/name;r=read(d);dirs.append(d)
            assert r['initial_component_fixture_only'] and not r['native_state_imported'] and not r['frozen_NoDispatch_view']
            full=(d/'full844.sv').read_text();assert hashlib.sha256(full.encode()).hexdigest()=='ef0abf3eed4497350a1ada19ab8a00c0ddc5732ed81390722844eac1ddb0efea'
            start=full.index('// Experimental original ContA/ContB/MemC phase and Hold composition.')
            assert (d/'candidate.sv').read_text()==full[start:]
            row=terminal(d,True,'CONTROL_HOLD PASS checks=266 physical=156 clocks=56 reset_cases=2')
            text=(d/'test.log').read_text()
            for held in [0,1]:assert f'TASKING_MASK_RESET held={held} LOW_root=1 Switch_before=11 Switch_after=00 actual_clock_edges=1'in text
            reset.append(row)
        for name in [f'tasking-mask-reset-frozen-{engine}-v2-20261023',f'tasking-mask-reset-frozen-later_control-{engine}-v3-20261023']:
            d=BUILD/name;r=read(d);dirs.append(d);assert r['frozen_NoDispatch_view']
            full=(d/'full844.sv').read_text();start=full.index('// Experimental original ContA/ContB/MemC phase and Hold composition.')
            assert (d/'candidate.sv').read_text()==full[start:].replace('tasking_view=0;','tasking_view=tasking_view;',1)
            negative.append(terminal(d,False,failure='CONTROL_HOLD_CLOCK_DIFF check=261 channel=12 actual=1 predicted=0'))
    units=[];wrong=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-read-window-{engine}-v1-20261023';dirs.append(d)
        units.append(terminal(d,True,'R_READ_WINDOW PASS checks=2304 closing_halves=1024 unclocked_updates=768 closed_changes=768'))
        assert (d/'candidate.sv').read_text()==implicit_generate((ROOT/'verilog/rtl/dorado_r_read_window_step.sv').read_text())
        for mutation,kind in [('new_data_at_close','visible mismatch check=1'),('omit_close','visible mismatch check=1'),('hide_open_view','commit view mismatch check=0')]:
            d=BUILD/f'r-read-window-{mutation}-{engine}-v1-20261023';dirs.append(d)
            assert read(d)['mutation']==mutation
            wrong.append(terminal(d,False,failure='R_READ_WINDOW '+kind))
        d=BUILD/f'r-operand-latches-{engine}-v3-20261023';dirs.append(d)
        units.append(terminal(d,True,'R_OPERAND_LATCH PASS checks=6145 handoffs=1024 late_updates=1024 held_edges=1024 Qs=48 RAM_read_addresses=1'))
        assert (d/'candidate.sv').read_text()==implicit_generate((ROOT/'verilog/rtl/dorado_r_operand_latches.sv').read_text())
        tb=(d/'tb.sv').read_text();assert 'always @(posedge host)if(advance)begin'in tb and "(read_gate | vis_gate)!==2'b11"in tb
        for mutation,check_no in [('registered_visible',3),('wrong_visible_close',5)]:
            d=BUILD/f'r-operand-latches-{mutation}-{engine}-v3-20261023';dirs.append(d)
            assert read(d)['mutation']==mutation
            wrong.append(terminal(d,False,failure=f'R_OPERAND_LATCH mismatch check={check_no}'))
        # Explicit generate/endgenerate is required by Quartus17. Keep the
        # old runtime proofs above and prove that syntax-only change exactly;
        # these current-source tests also compile and run in both engines.
        for component,rtl,summary in [
            ('r-read-window','dorado_r_read_window_step.sv','R_READ_WINDOW PASS checks=2304 closing_halves=1024 unclocked_updates=768 closed_changes=768'),
            ('r-operand-latches','dorado_r_operand_latches.sv','R_OPERAND_LATCH PASS checks=6145 handoffs=1024 late_updates=1024 held_edges=1024 Qs=48 RAM_read_addresses=1')]:
            d=BUILD/f'{component}-{engine}-v4-20261023';dirs.append(d)
            assert sha(d/'candidate.sv')==sha(ROOT/'verilog/rtl'/rtl)
            units.append(terminal(d,True,summary))
    # Retain the actual first failures; these are not negative controls.
    fixture=[]
    for engine in ['icarus','verilator']:
        d=BUILD/f'r-operand-latches-{engine}-v1-20261023';dirs.append(d)
        fixture.append(terminal(d,False,failure='Invalid overlapping read/visible latch gates'))
    d=BUILD/'r-operand-latches-verilator-v2-20261023';dirs.append(d)
    fixture.append(terminal(d,False,failure='R_OPERAND_LATCH mismatch check=3'))
    assert 'INITIALDLY'in(d/'compile.log').read_text()
    d=BUILD/'r-operand-latches-icarus-v2-20261023';dirs.append(d)
    fixture.append(terminal(d,True,'R_OPERAND_LATCH PASS checks=6145'))
    # Wrong-owner tests against that raced Verilator fixture cannot prove the
    # intended mutation; keep them explicitly superseded by the clocked v3.
    for d in BUILD.glob('r-operand-latches-*-v2-20261023'):
        if d not in dirs:dirs.append(d)
    compile_failure=BUILD/'tasking-mask-reset-icarus-v1-20261023';dirs.append(compile_failure)
    assert read(compile_failure)['actions'][0]['returncode']!=0
    integration=[]
    for d in coupled:
        r=read(d);assert r['status']!='RUNNING'
        assert r['full_state_comparison']=='all844 physical Qs after owner commit'
        assert not r['assembly_permitted'] and r['RAM_bits']==9216 and r['clock_channels']==100
        assert sum(x['changed']for x in r['source_files'])==4
        for row in r['source_files']:assert sha(d/row['path'])==row['output_sha256']
        text=(d/'test.log').read_text()if(d/'test.log').exists()else''
        if r.get('mutation')=='omit_close':
            assert r['status']=='R_FRONT_READ_WINDOWS_FAILED' and r['actions'][-1]['returncode']!=0
            assert 'R front state mismatch check=15'in text and 'R_FRONT_DIFF bit=556'in text
        else:
            assert r['status']=='PASS_R_FRONT_READ_WINDOWS' and r['actions'][-1]['returncode']==0
            assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
            clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
        integration.append(dict(directory=d.name,status=r['status'],mutation=r.get('mutation'),run_state_sha256=sha(d/'run-state.json'),last_log_lines=text.splitlines()[-12:]))
        dirs.append(d)
    positive=[x for x in integration if not x['mutation']]
    assert len(positive)>=2 and {'icarus','verilator'}<={read(BUILD/x['directory'])['engine']for x in positive}
    measured=timing(fit)if fit else None
    if fit:dirs.append(fit)
    for version in [1,2]:
        d=BUILD/f'r-front-read-windows-fit-failed-v{version}-20261023'
        if d.exists():
            r=read(d);assert r['status']=='TERMINAL_FAILED' and r['stages'][0]['stage']=='map' and r['stages'][0]['returncode']!=0
            assert 'Verilog HDL syntax error at dorado_r_read_window_step.sv'in(d/'map.log').read_text();dirs.append(d)
    result=dict(status='PASS_TASK_CLOCK_RESET_AND_TRANSPARENT_LATCH_UNITS',scope=__doc__,
                masked_profile_matrix=m['cases'],baseboard_CP_source_audit_sha256=sha(audit/'results.json'),reset_cases=reset,reset_negative_controls=negative,
                latch_units=units,latch_negative_controls=wrong,superseded_fixture_results=fixture,
                full_graph_experiments=integration,balanced_full_graph_compared=True,full_latch_integration_qualified=False,timing=measured,
                ordinary60ns_qualified=False,physical_timing_qualified=False,native_firmware_qualified=False,
                hardware_speedup_claimed=False,next='Complete source-bound current SRAM port/closed versus live latch ownership in full graph; retain full oracle,15ns and native firmware/60ns/SDRAM/A-F gates.')
    out.mkdir(parents=True);(out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result,list(dict.fromkeys(dirs))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    p.add_argument('--coupled',type=Path,action='append',default=[]);p.add_argument('--timing-fit',type=Path);a=p.parse_args()
    r,dirs=review(a.output.resolve(),[d.resolve()for d in a.coupled],a.timing_fit.resolve()if a.timing_fit else None);print(r['status'])
    if a.archive:archive(a.archive.resolve(),a.output.resolve(),dirs,[Path(__file__).name,'test_r_front_read_windows.py','test_r_front_read_window_matrix.py','test_r_read_window.py','test_r_operand_latches.py','test_tasking_clock_mask.py','test_r_front_control_extended_matrix.py','fit_r_front_read_windows.py','audit_baseboard_cp_source.py'])
