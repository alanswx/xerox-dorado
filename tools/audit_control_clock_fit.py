#!/usr/bin/env python3
"""Read isolated control-clock timing evidence; never infer native speed.

Requires unchanged prepared inputs (only Quartus's version annotation allowed),
simulation-qualified evaluator bytes, four timing conditions, the 15 ns clock
and fully constrained virtual I/O. Rejects missing targeted path coverage unless
explicitly reviewing the historical rejected experiment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

CORNERS={'7_slow_1100mv_100c':'Slow 1100mV 100C Model',
         '7_slow_1100mv_-40c':'Slow 1100mV -40C Model',
         'MIN_fast_1100mv_100c':'Fast 1100mV 100C Model',
         'MIN_fast_1100mv_-40c':'Fast 1100mV -40C Model'}


def sha(raw):return hashlib.sha256(raw).hexdigest()


def slack(path,model):
    text=path.read_text(errors='strict');assert model in text,(path,'wrong model')
    match=re.search(r'Worst case slack is (-?\d+\.\d+)',text,re.I)
    assert match,(path,'missing slack')
    value=float(match[1])
    summaries=[float(x)for x in re.findall(r';\s*Slack\s*;\s*(-?\d+\.\d+)\b',text)]
    assert summaries and min(summaries)==value,(path,'header/path slack disagreement')
    return value


def audit(directory,qualified,allow_missing):
    provenance=json.loads((directory/'provenance.json').read_text())
    profiles={None:(52,51,'control_clock','PASS_COUPLED_ORIGINAL_CONTROL_CLOCK_COMPONENT','dorado_control_clock_step.sv'),
              'processor_capture':(122,109,'processor_capture','PASS_COUPLED_PROCESSOR_CAPTURE_COMPONENT','dorado_processor_capture_step.sv'),
              'task_capture':(242,139,'task_capture','PASS_TASK_CAPTURE_ARRIVAL_CONTRACT','dorado_task_capture_step.sv'),
              't_file':(660,463,'t_file','PASS_T_FILE_ARRIVAL_CONTRACT','dorado_t_file_step.sv'),
              't_parity':(846,607,'t_parity','PASS_T_PARITY_ARRIVAL_CONTRACT','dorado_t_parity_step.sv')}
    assert provenance.get('component')in profiles,'Unknown fixture profile'
    expected_launch,expected_result,component,expected_status,evaluator=profiles[provenance.get('component')]
    assert provenance['clock_period_ns']==15 and provenance['launch_bits']==expected_launch and provenance['result_bits']==expected_result
    assert not provenance['assembly_permitted']and not provenance['native_qualified']
    annotation=b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
    for name,digest in provenance['files'].items():
        raw=(directory/name).read_bytes()
        if name=='clock_fit.qsf'and sha(raw)!=digest:
            assert raw.endswith(annotation)and raw.count(annotation)==1
            raw=raw[:-len(annotation)]
        assert sha(raw)==digest,('changed input',name)
    qualified_record=json.loads((qualified/'results.json').read_text())
    assert qualified_record['status']==expected_status
    evaluator_digest=qualified_record['inputs']['verilog/rtl/'+evaluator]
    assert sha((directory/evaluator).read_bytes())==evaluator_digest
    for name in provenance['files']:
        if name.endswith('.sv')and name!='top.sv':
            assert sha((directory/name).read_bytes())==qualified_record['inputs']['verilog/rtl/'+name],('unqualified RTL dependency',name)
    sdc=(directory/'clock.sdc').read_text();qsf=(directory/'clock_fit.qsf').read_text()
    assert sdc.splitlines()==[
        'create_clock -name host_clock -period 15.000 [get_ports host_clk]',
        'derive_clock_uncertainty',
        'set_input_delay -clock host_clock -max 2.000 [get_ports {pins[*]}]',
        'set_input_delay -clock host_clock -min 0.000 [get_ports {pins[*]}]',
        'set_output_delay -clock host_clock -max 2.000 [get_ports {result[*]}]',
        'set_output_delay -clock host_clock -min 0.000 [get_ports {result[*]}]',
    ],'Unexpected timing constraints'
    assert 'create_clock -name host_clock -period 15.000 [get_ports host_clk]'in sdc
    assert not re.search(r'set_(?:false_path|multicycle_path|max_delay|min_delay|disable_timing)',sdc)
    assert '5CSEBA6U23I7'in qsf and 'REGISTER_RETIMING OFF'in qsf and 'GENERATE_RBF_FILE OFF'in qsf
    state=json.loads((directory/'run-state.json').read_text());assert state['status']=='TERMINAL_REVIEW_REQUIRED'
    assert [r['stage']for r in state['stages']]==['map','fit','sta','corners']
    assert all(r['returncode']==0 for r in state['stages'])
    readback=json.loads((directory/'collected-input-check.json').read_text())
    assert readback['status']=='TERMINAL_INPUT_READBACK'and len(readback['job_pids_absent'])==5
    assert set(readback['changed_inputs'])<= {'clock_fit.qsf'}
    for name,digest in readback['actual'].items():assert sha((directory/name).read_bytes())==digest
    records=[];targeted_complete=True
    for corner,model in CORNERS.items():
        values={mode:slack(directory/('clock-'+mode+'-'+corner+'.rpt'),model)for mode in ['setup','hold','mpw']}
        for mode in ['recovery','removal']:
            assert 'Nothing to report.'in(directory/('clock-'+mode+'-'+corner+'.rpt')).read_text()
        clock_text=(directory/('clock-clocks-'+corner+'.rpt')).read_text()
        assert len(re.findall(r';\s*host_clock\s*;\s*Base\s*;\s*15\.000\s*;',clock_text))==1
        ucp=(directory/('clock-ucp-'+corner+'.rpt')).read_text()
        for prop in ['Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports',
                     'Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths']:
            match=re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',ucp)
            assert match and match.groups()==('0','0'),(corner,prop)
        path=directory/('clock-evaluator-'+corner+'.rpt')
        if path.is_file():
            values['evaluator_setup']=slack(path,model)
            text=path.read_text();assert 'launch['in text and 'result['in text
        else:
            assert allow_missing,'Missing launch/result path report'
            targeted_complete=False;values['evaluator_setup']=None
        records.append(dict(corner=corner,**values))
    summary=(directory/'clock_fit.fit.summary').read_text()
    assert 'Fitter Status : Successful'in summary and 'Device : 5CSEBA6U23I7'in summary and 'Timing Models : Final'in summary
    resources={}
    for key,label in [('alms','Logic utilization (in ALMs)'),('registers','Total registers'),
                      ('memory_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks'),
                      ('dsp','Total DSP Blocks'),('plls','Total PLLs')]:
        match=re.search(re.escape(label)+r'\s*:\s*([\d,]+)',summary);assert match,label
        resources[key]=int(match[1].replace(',',''))
    assert resources['memory_bits']==resources['ram_blocks']==resources['dsp']==resources['plls']==0
    minimum={mode:min(r[mode]for r in records)for mode in ['setup','hold','mpw']}
    passing=targeted_complete and all(v>=0 for v in minimum.values())
    return dict(status='PASS_COMPONENT_TIMING'if passing else'REJECTED_COMPONENT_TIMING',
        scope=__doc__,component=component,functional_qualification_status=qualified_record['status'],period_ns=15,resources=resources,minimum_slack_ns=minimum,conditions=records,
        input_staging_bits=provenance.get('input_staging_bits',0),
        targeted_coverage_complete=targeted_complete,evaluator_sha256=evaluator_digest,
        permitted_qsf_delta='Quartus17.0.2 LAST_QUARTUS_VERSION annotation only',
        native_qualified=False,hardware_speedup_claimed=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--allow-missing-targeted-reports',action='store_true');a=p.parse_args()
    record=audit(a.directory.resolve(),a.qualified.resolve(),a.allow_missing_targeted_reports)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],record['minimum_slack_ns'],record['resources'])
