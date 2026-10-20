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


def r_storage_memory(directory,qualified,processor_bits):
    assert processor_bits in (596,636,736) and qualified['physical_bits']==processor_bits
    assert qualified['ram_bits']==9216 and qualified['total_physical_bits']==9216+processor_bits
    if processor_bits>=636:
        assert qualified['R_final_address_stage_qualified'] and qualified['all_R_memory_bits_compared']
    assert qualified['r_read_write_nonoverlap_checked'] and qualified['post_commit_source_epoch_checked']
    assert qualified['ram_arrays_flattened'] is False
    text=(directory/'clock_fit.map.rpt').read_text()
    match=re.search(r';\s*Total MLAB memory bits\s*;\s*([\d,]+)\s*;',text)
    assert match and int(match[1].replace(',',''))==9216,'Missing/wrong MLAB bit count'
    rows=[]
    for line in text.splitlines():
        if 'ALTDPRAM_INSTANCE' not in line or ';'not in line:continue
        fields=[x.strip()for x in line.split(';')][1:-1]
        if len(fields)==9:rows.append(fields)
    assert len(rows)==4,('Missing/extra R memory bank',len(rows))
    names=[]
    for row in rows:
        match=re.search(r'altdpram:(rm_h|stk_h|rm_l|stk_l)_rtl_0\|',row[0])
        assert match and row[1:]==['MLAB','Simple Dual Port','256','9','256','9','2304','None'],row
        names.append(match[1])
    assert set(names)=={'rm_h','stk_h','rm_l','stk_l'} and len(set(names))==4
    # With real address muxes Quartus cannot absorb the launch flip-flop into
    # the read-address port. The new component requires a truly async read;
    # retain the older externally registered-address fixture's exact contract.
    parameters={'INDATA_REG':'INCLOCK','WRADDRESS_REG':'INCLOCK','WRCONTROL_REG':'INCLOCK',
        'RDADDRESS_REG':'UNREGISTERED'if processor_bits>=636 else'OUTCLOCK',
        'RDCONTROL_REG':'UNREGISTERED','OUTDATA_REG':'UNREGISTERED',
        'RAM_BLOCK_TYPE':'LUTRAM','READ_DURING_WRITE_MODE_MIXED_PORTS':'DONT_CARE'}
    for name in names:
        match=re.search(r'; Parameter Settings for Inferred Entity Instance: [^\n]*altdpram:'+name+r'_rtl_0 ;\n(.*?)(?=\n\n)',text,re.S)
        assert match,('Missing R memory parameter table',name)
        for key,value in parameters.items():
            found=re.search(r';\s*'+key+r'\s*;\s*([^;]+?)\s*;',match[1])
            assert found and found[1]==value,(name,key,found[1]if found else None)
    return dict(logical_banks=sorted(names),bits=9216,depth=256,width=9,type='MLAB',
        initialization='None',inferred_parameters=parameters,
        collision_scope='no_rw_check: architecture reads outside write windows; coupled nonoverlap/source-epoch checks required. No same-edge raw-output value guarantee.')


def audit(directory,qualified,allow_missing):
    provenance=json.loads((directory/'provenance.json').read_text())
    profiles={None:(52,51,'control_clock','PASS_COUPLED_ORIGINAL_CONTROL_CLOCK_COMPONENT','dorado_control_clock_step.sv'),
              'processor_capture':(122,109,'processor_capture','PASS_COUPLED_PROCESSOR_CAPTURE_COMPONENT','dorado_processor_capture_step.sv'),
              'task_capture':(242,139,'task_capture','PASS_TASK_CAPTURE_ARRIVAL_CONTRACT','dorado_task_capture_step.sv'),
              't_file':(660,463,'t_file','PASS_T_FILE_ARRIVAL_CONTRACT','dorado_t_file_step.sv'),
              't_parity':(846,607,'t_parity','PASS_T_PARITY_ARRIVAL_CONTRACT','dorado_t_parity_step.sv'),
              'r_operand':(898,675,'r_operand','PASS_R_OPERAND_ARRIVAL_CONTRACT','dorado_r_operand_step.sv'),
              'r_flags':(902,689,'r_flags','PASS_R_FLAGS_ARRIVAL_CONTRACT','dorado_r_flags_step.sv'),
              'r_address':(1042,791,'r_address','PASS_R_ADDRESS_ARRIVAL_CONTRACT','dorado_r_address_step.sv'),
              'r_memory':(1043,847,'r_memory','PASS_R_MEMORY_ARRIVAL_CONTRACT','dorado_r_memory_step.sv'),
              'r_front':(1211,1181,'r_front','PASS_R_FRONT_ARRIVAL_CONTRACT','dorado_r_front_step.sv'),
              'r_front_functional':(1211,971,'r_front_functional','PASS_R_FRONT_ARRIVAL_CONTRACT','dorado_r_front_step.sv'),
              'r_storage':(939,709,'r_storage','PASS_R_STORAGE_ARRIVAL_CONTRACT','dorado_r_storage_step.sv')}
    assert provenance.get('component')in profiles,'Unknown fixture profile'
    expected_launch,expected_result,component,expected_status,evaluator=profiles[provenance.get('component')]
    assert provenance['clock_period_ns']==15 and provenance['launch_bits']==expected_launch and provenance['result_bits']==expected_result
    if component=='r_front_functional':
        assert provenance['observation_outputs_registered'] is False
        assert provenance['observation_only_outputs']==['source_pins','fa_n']
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
            if component in ('r_front','r_front_functional')and name=='dorado_r_front_cone.sv':
                from fit_r_front import quartus_cone
                cone=json.loads((qualified/'dorado_r_front_cone.json').read_text())
                assert (directory/name).read_text()==quartus_cone((qualified/name).read_text())
                assert sha((directory/'qualified-cone.sv.txt').read_bytes())==cone['output_sha256']
                assert sha((qualified/name).read_bytes())==cone['output_sha256']
                for source,digest in cone['source_cells'].items():
                    assert qualified_record['inputs'][source]==digest,source
            else:
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
    memory_bits={'r_storage':596,'r_memory':636,'r_front':736,'r_front_functional':736}
    memory=r_storage_memory(directory,qualified_record,memory_bits[component])if component in memory_bits else None
    if memory:resources['mlab_memory_bits']=memory['bits']
    minimum={mode:min(r[mode]for r in records)for mode in ['setup','hold','mpw']}
    passing=(targeted_complete and all(v>=0 for v in minimum.values()) and
             all(r['evaluator_setup'] is not None and r['evaluator_setup']>=0 for r in records))
    result=dict(status='PASS_COMPONENT_TIMING'if passing else'REJECTED_COMPONENT_TIMING',
        scope=__doc__,component=component,functional_qualification_status=qualified_record['status'],period_ns=15,resources=resources,minimum_slack_ns=minimum,conditions=records,
        input_staging_bits=provenance.get('input_staging_bits',0),
        targeted_coverage_complete=targeted_complete,evaluator_sha256=evaluator_digest,
        permitted_qsf_delta='Quartus17.0.2 LAST_QUARTUS_VERSION annotation only',
        native_qualified=False,hardware_speedup_claimed=False)
    if memory:result['memory_contract']=memory
    if component=='r_front_functional':
        result['observation_outputs']=dict(registered=False,names=['source_pins','fa_n'],
            scope='Post-state test observations are retained in simulation. Every state bit, clock event, functional port and unsettled check remains in the registered budget.')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--allow-missing-targeted-reports',action='store_true');a=p.parse_args()
    record=audit(a.directory.resolve(),a.qualified.resolve(),a.allow_missing_targeted_reports)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],record['minimum_slack_ns'],record['resources'])
