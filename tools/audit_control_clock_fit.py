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
    assert processor_bits in (596,636,736,752,786,792) and qualified['physical_bits']==processor_bits
    assert qualified['ram_bits']==9216 and qualified['total_physical_bits']==9216+processor_bits
    if processor_bits>=636:
        assert qualified['all_R_memory_bits_compared']
        if processor_bits in (786,792):
            assert qualified['bounded_original_graph_checks']
            assert qualified['full_producer_arrival_contract_qualified'] is False
        else:
            assert qualified['R_final_address_stage_qualified']
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


def stack_adder_bridge(qualified, record):
    """Bind the arithmetic-only rewrite to its executed binary proof and parent."""
    parent_sha='60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'
    cone_sha='6db485f25d397639fe381c4b3390bc0b7338a2e659ed17a654517b4f9c3c7ea6'
    assert record['arrival_qualification_method']=='binary_combinational_equivalence_bridge'
    assert record['parent_cone_sha256']==parent_sha and record['candidate_cone_sha256']==cone_sha
    files={'inherited-mask-gate.json':'inherited_mask_gate_sha256',
           'adder-transform.json':'adder_transform_sha256',
           'adder-equivalence/results.json':'adder_equivalence_sha256'}
    for name,key in files.items():
        assert sha((qualified/name).read_bytes())==record[key],('changed bridge evidence',name)
    inherited=json.loads((qualified/'inherited-mask-gate.json').read_text())
    assert inherited['status']=='PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    assert record['inherited_arrivals']==inherited['arrivals'] and len(inherited['arrivals'])==12
    assert record['inherited_negative_cases']==inherited['negative_cases'] and len(inherited['negative_cases'])==2
    assert record['negative_cases']==[]
    assert len(record['arrivals'])==2 and {r['engine']for r in record['arrivals']}=={'icarus','verilator'}
    assert all(r['summary']=='checks=13515 writes=328 reads=5608 epochs=797'for r in record['arrivals'])
    assert inherited['candidate_sha256']==record['candidate_sha256']
    assert inherited['source_bound_properties_sha256']==record['source_bound_properties_sha256']
    before=(qualified/'original-cone.sv.txt').read_text()
    after=(qualified/'dorado_r_front_cone.sv').read_text()
    assert sha(before.encode())==parent_sha and sha(after.encode())==cone_sha
    transform=json.loads((qualified/'adder-transform.json').read_text())
    assert transform==json.loads((qualified/'adder-equivalence/transform.json').read_text())
    assert transform['parent_sha256']==parent_sha and transform['output_sha256']==cone_sha
    assert transform['connected_outputs_preserved'] and transform['non_arithmetic_source_unchanged']
    assert transform['architectural_state_added'] is False and transform['native_integrated'] is False
    assert len(transform['original_instances'])==3
    for index,line in enumerate(transform['original_instances']):
        assert before.count(line)==1
        before=before.replace(line,transform['inserted']if index==0 else'')
    assert before==after,'Rewrite changes more than the three arithmetic instances'
    proof=json.loads((qualified/'adder-equivalence/results.json').read_text())
    assert proof['status']=='PASS_STACK_ADDER_BINARY_EQUIVALENCE'
    assert proof['cone_sha256']==cone_sha
    assert sha((qualified/'adder-equivalence/rewritten-cone.sv').read_bytes())==cone_sha
    assert proof['native_integrated'] is False and proof['physical_timing_qualified'] is False
    assert proof['source_sha256']==inherited['inputs']['verilog/cells/cell_MC10182.v']
    assert sha((qualified/'adder-equivalence/inputs/cell_MC10182.v').read_bytes())==proof['source_sha256']
    assert len(proof['cases'])==6 and len(proof['actions'])==12
    assert {(r['engine'],r['mutation'])for r in proof['cases']}=={
        (engine,mutation)for engine in ['icarus','verilator']for mutation in [None,'drop2','drop4']}
    for i,r in enumerate(proof['cases']):
        compile_action,test_action=proof['actions'][2*i:2*i+2]
        assert compile_action['returncode']==0 and 'finished'in compile_action and 'finished'in test_action
        if r['mutation'] is None:
            assert r['checks']==4096 and r['physical_wiring_checks']==512 and r['result']=='PASS'
            assert test_action['returncode']==0
        else:
            assert r['result']=='REJECTED_CARRY_ERROR' and test_action['returncode']==1


def visible_bridge(qualified,record):
    """Check the source-bound complementary-window proof and exact RTL delta."""
    assert record['arrival_qualification_method']=='binary_visible_nonoverlap_equivalence_bridge'
    assert record['parent_operand_sha256']=='d4d39beb23d07dd54d978b56bb7fded9703b667291fff22707f5f438d73f9f30'
    assert record['visible_operand_sha256']=='2e760e63e1a4332b4f0af567b22044fff5f12e8e6ee04e672030a15a2733724b'
    assert sha((qualified/'visible-proof/results.json').read_bytes())==record['visible_proof_sha256']
    assert sha((qualified/'inherited-mask-gate.json').read_bytes())==record['inherited_mask_gate_sha256']
    inherited=json.loads((qualified/'inherited-mask-gate.json').read_text())
    assert inherited['status']=='PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    assert record['inherited_arrivals']==inherited['arrivals'] and len(record['inherited_arrivals'])==12
    assert record['inherited_negative_cases']==inherited['negative_cases'] and len(record['inherited_negative_cases'])==2
    assert record['negative_cases']==[]
    assert len(record['arrivals'])==2 and {r['engine']for r in record['arrivals']}=={'icarus','verilator'}
    assert all(r['summary']=='checks=13515 writes=328 reads=5608 epochs=797'for r in record['arrivals'])
    assert inherited['candidate_sha256']==record['candidate_sha256']
    assert inherited['source_bound_properties_sha256']==record['source_bound_properties_sha256']
    proof=json.loads((qualified/'visible-proof/results.json').read_text())
    assert proof['status']=='PASS_BINARY_R_VISIBLE_NONOVERLAP'
    assert [r['result']for r in proof['cases']]==['unsat','sat','sat']
    assert all(r['returncode']==0 for r in proof['cases'])
    assert proof['full_next_state_equivalence_proven'] is False and proof['analog_skew_qualified'] is False
    assert proof['native_integrated'] is False
    assert {r['board']for r in proof['original_clock_wiring']}=={'ProcH-Rev-Ce','ProcL-Rev-Ci'}
    transform=proof['transform']
    assert transform['parent_sha256']==record['parent_operand_sha256']
    assert transform['output_sha256']==record['visible_operand_sha256']
    assert transform['extra_architectural_state'] is False and transform['native_integrated'] is False
    before=(qualified/'original-operand.sv.txt').read_text()
    assert sha(before.encode())==record['parent_operand_sha256']
    assert len(transform['changes'])==2
    for old,new in transform['changes']:
        assert before.count(old)==1;before=before.replace(old,new)
    after=(qualified/'executed-inputs/verilog/rtl/dorado_local_r_operand_step.sv').read_text()
    assert before==after and sha(after.encode())==record['visible_operand_sha256']
    assert (qualified/'visible-proof/candidate.sv').read_text()==after
    changed=[n for n in record['compiled_rtl_files']
             if record['inputs']['verilog/rtl/'+n]!=inherited['inputs']['verilog/rtl/'+n]]
    assert changed==['dorado_local_r_operand_step.sv'],changed
    assert sha((qualified/'dorado_r_front_cone.sv').read_bytes())=='60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'


def flags_bridge(qualified,record):
    """Bind the flag-only output rewrite to the prior visible-R qualification."""
    assert record['arrival_qualification_method']=='binary_flag_factorization_equivalence_bridge'
    assert record['parent_flags_sha256']=='d4809afa1d091ce88e5456db7980dc2616a42368cae829abccb0840de1fac719'
    assert record['flags_sha256']=='233f668d60cf89481469b02a8ccd4e800a06c597faf9102eb760b6401db68d2a'
    assert sha((qualified/'inherited-visible-gate.json').read_bytes())==record['inherited_visible_gate_sha256']
    parent=json.loads((qualified/'inherited-visible-gate.json').read_text())
    assert parent['status']=='PASS_BOUNDED_R_FRONT_VISIBLE_BUDGET_GATE'
    visible_bridge(qualified,parent)
    assert record['previous_visible_arrivals']==parent['arrivals']
    assert record['inherited_arrivals']==parent['inherited_arrivals']
    assert record['inherited_negative_cases']==parent['inherited_negative_cases']
    assert len(record['arrivals'])==2 and {r['engine']for r in record['arrivals']}=={'icarus','verilator'}
    assert all(r['summary']=='checks=13515 writes=328 reads=5608 epochs=797'for r in record['arrivals'])
    assert record['compiled_rtl_files']==parent['compiled_rtl_files']
    changed=[n for n in record['compiled_rtl_files']
             if record['inputs']['verilog/rtl/'+n]!=parent['inputs']['verilog/rtl/'+n]]
    assert changed==['dorado_local_r_flags_step.sv'],changed
    assert sha((qualified/'flags-proof/results.json').read_bytes())==record['flags_proof_sha256']
    proof=json.loads((qualified/'flags-proof/results.json').read_text())
    assert proof['status']=='PASS_BINARY_R_FLAG_FACTORIZATION'
    assert [r['case']for r in proof['cases']]==['original','stale_ram','swap_md']
    assert [r['result']for r in proof['cases']]==['unsat','sat','sat']
    for row in proof['cases']:
        assert row['returncode']==0
        assert sha((qualified/'flags-proof'/(row['case']+'.smt2')).read_bytes())==row['smt_sha256']
    assert proof['full_next_state_equivalence_proven'] is False and proof['analog_skew_qualified'] is False
    assert proof['native_integrated'] is False
    t=proof['transform']
    assert t['parent_sha256']==record['parent_flags_sha256'] and t['output_sha256']==record['flags_sha256']
    assert t['extra_architectural_state'] is False and t['physical_latch_updates_unchanged']
    before=(qualified/'original-flags.sv.txt').read_text()
    assert sha(before.encode())==record['parent_flags_sha256']
    assert before.count(t['old'])==1
    after=(qualified/'executed-inputs/verilog/rtl/dorado_local_r_flags_step.sv').read_text()
    assert before.replace(t['old'],t['new'])==after and sha(after.encode())==record['flags_sha256']
    assert (qualified/'flags-proof/candidate.sv').read_text()==after


def audit(directory,qualified,allow_missing):
    provenance=json.loads((directory/'provenance.json').read_text())
    profiles={None:(52,51,'control_clock','PASS_COUPLED_ORIGINAL_CONTROL_CLOCK_COMPONENT','dorado_control_clock_step.sv'),
              'processor_capture':(122,109,'processor_capture','PASS_COUPLED_PROCESSOR_CAPTURE_COMPONENT','dorado_processor_capture_step.sv'),
              'task_capture':(242,139,'task_capture','PASS_TASK_CAPTURE_ARRIVAL_CONTRACT','dorado_task_capture_step.sv'),
              'task_producers':(98,66,'task_producers','PASS_COUPLED_TASK_PRODUCERS','dorado_task_producers_step.sv'),
              'mir_ready':(232,109,'mir_ready','PASS_MIR_CP_READY_COUPLED','dorado_mir_ready_step.sv'),
              'conta_mir':(356,186,'conta_mir','PASS_CONTA_MIR_COUPLING','dorado_conta_mir_step.sv'),
              'task_ready_capture':(278,189,'task_ready_capture','PASS_TASK_READY_CAPTURE','dorado_task_ready_capture_step.sv'),
              't_file':(660,463,'t_file','PASS_T_FILE_ARRIVAL_CONTRACT','dorado_t_file_step.sv'),
              't_parity':(846,607,'t_parity','PASS_T_PARITY_ARRIVAL_CONTRACT','dorado_t_parity_step.sv'),
              'r_operand':(898,675,'r_operand','PASS_R_OPERAND_ARRIVAL_CONTRACT','dorado_r_operand_step.sv'),
              'r_flags':(902,689,'r_flags','PASS_R_FLAGS_ARRIVAL_CONTRACT','dorado_r_flags_step.sv'),
              'r_address':(1042,791,'r_address','PASS_R_ADDRESS_ARRIVAL_CONTRACT','dorado_r_address_step.sv'),
              'r_memory':(1043,847,'r_memory','PASS_R_MEMORY_ARRIVAL_CONTRACT','dorado_r_memory_step.sv'),
              'r_front':(1211,1181,'r_front','PASS_R_FRONT_ARRIVAL_CONTRACT','dorado_r_front_step.sv'),
              'r_front_functional':(1211,971,'r_front_functional','PASS_R_FRONT_ARRIVAL_CONTRACT','dorado_r_front_step.sv'),
              'r_front_ready':(1247,1021,'r_front_ready','PASS_R_FRONT_READY_ARRIVAL_CONTRACT','dorado_r_front_ready_step.sv'),
              'r_front_mir':(1409,1170,'r_front_mir','PASS_BOUNDED_R_FRONT_MIR_BUDGET_GATE','dorado_r_front_mir_step.sv'),
              'r_front_mir_mask':(1429,1184,'r_front_mir_mask','PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE','dorado_r_front_commands_step.sv'),
              'r_front_stack_adder':(1429,1184,'r_front_stack_adder','PASS_BOUNDED_STACK_ADDER_BUDGET_GATE','dorado_r_front_commands_step.sv'),
              'r_front_visible':(1429,1184,'r_front_visible','PASS_BOUNDED_R_FRONT_VISIBLE_BUDGET_GATE','dorado_r_front_commands_step.sv'),
              'r_front_flags':(1429,1184,'r_front_flags','PASS_BOUNDED_R_FRONT_FLAGS_BUDGET_GATE','dorado_r_front_commands_step.sv'),
              'r_storage':(939,709,'r_storage','PASS_R_STORAGE_ARRIVAL_CONTRACT','dorado_r_storage_step.sv')}
    assert provenance.get('component')in profiles,'Unknown fixture profile'
    expected_launch,expected_result,component,expected_status,evaluator=profiles[provenance.get('component')]
    assert provenance['clock_period_ns']==15 and provenance['launch_bits']==expected_launch and provenance['result_bits']==expected_result
    if component=='conta_mir':
        assert sha((directory/'top.sv').read_bytes())=='4af1a8411de7b9ea5c49ec4e70788d974579beed92abc4772a6c5fa40dd9b488','Unexpected Control-A/MIR fixture wiring'
        assert len(json.loads((directory/'functional-gate.json').read_text())['rejected_mutations'])==7
    if component=='mir_ready':
        assert sha((directory/'top.sv').read_bytes())=='21c4aaedf8cba23facc358fb5c3ca9c3d83bae99db0a8a99f439bb2bc1591777','Unexpected MIR/Ready fixture wiring'
        assert len(json.loads((directory/'functional-gate.json').read_text())['rejected_mutations'])==6
    if component=='r_front_ready':
        # The fixed 752-Q shell captures every functional result/invariant.
        # Hash identity also rejects a rehashed top that drops/permutates them
        # while leaving plausible interface dimensions and old reports.
        assert sha((directory/'top.sv').read_bytes())=='aedc73ed7ce97324d253e83dd5165bd3a7895da0f02dec46d67073829c55e897','Unexpected Ready/R-front fixture wiring'
    if component=='r_front_mir':
        assert sha((directory/'top.sv').read_bytes())=='0ee3ad83d661d041c387324bd13c33b01fbf62f7db8cdbcd198f64c4f27799b6','Unexpected R-front/MIR fixture wiring'
        assert provenance['full_producer_arrival_contract_qualified'] is False
    if component in ('r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
        assert sha((directory/'top.sv').read_bytes())=='70dfecbde9d5cd3cad02b9d7670bebd5305c645600f66a06aa470bbdcf4b774b','Unexpected masked-MIR fixture wiring'
        assert provenance['full_producer_arrival_contract_qualified'] is False
    if component in ('r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
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
    if component=='r_front_mir':
        assert (directory/'functional-gate.json').read_bytes()==(qualified/'results.json').read_bytes()
        assert qualified_record['bounded_original_graph_checks']
        assert qualified_record['full_producer_arrival_contract_qualified'] is False
        assert len(qualified_record['arrivals'])==6
    if component in ('r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
        assert (directory/'functional-gate.json').read_bytes()==(qualified/'results.json').read_bytes()
        assert qualified_record['bounded_original_graph_checks']
        assert qualified_record['full_producer_arrival_contract_qualified'] is False
        if component=='r_front_stack_adder':
            stack_adder_bridge(qualified,qualified_record)
        elif component=='r_front_visible':
            visible_bridge(qualified,qualified_record)
        elif component=='r_front_flags':
            flags_bridge(qualified,qualified_record)
        else:
            assert len(qualified_record['arrivals'])==12 and len(qualified_record['negative_cases'])==2
        assert qualified_record['candidate_sha256']=='e695318064d80a9e308ff5e76e3fd57d86bb55412a448b163da135856883dd0e'
        properties=qualified/'mask-properties/proof.json'
        assert sha(properties.read_bytes())==qualified_record['source_bound_properties_sha256']
        property_record=json.loads(properties.read_text())
        assert property_record['status']=='PASS_SOURCE_BOUND_MIR_CLOCK_MASK_PROPERTIES'
        assert property_record['full_next_state_equivalence_proven'] is False
    evaluator_digest=qualified_record['inputs']['verilog/rtl/'+evaluator]
    assert sha((directory/evaluator).read_bytes())==evaluator_digest
    for name in provenance['files']:
        if name.endswith('.sv')and name!='top.sv':
            if component in ('r_front','r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags')and name=='dorado_r_front_cone.sv':
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
    if component in ('r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
        # Bind the complete project source list to the executed functional
        # gate; a rehashed QSF cannot silently substitute another module.
        sources=re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE (\S+)$',qsf,re.M)
        assert sources==['top.sv',*qualified_record['compiled_rtl_files'],'dorado_r_front_cone.sv'],'Unexpected masked-MIR source list'
        assert not re.search(r'-name (?:VERILOG_FILE|VHDL_FILE)',qsf)
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
    memory_bits={'r_storage':596,'r_memory':636,'r_front':736,'r_front_functional':736,'r_front_ready':752,'r_front_mir':786,'r_front_mir_mask':792,'r_front_stack_adder':792,'r_front_visible':792,'r_front_flags':792}
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
    if component in ('r_front_functional','r_front_ready','r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
        result['observation_outputs']=dict(registered=False,names=['source_pins','fa_n'],
            scope='Post-state test observations are retained in simulation. Every state bit, clock event, functional port and unsettled check remains in the registered budget.')
    if component in ('r_front_mir','r_front_mir_mask','r_front_stack_adder','r_front_visible','r_front_flags'):
        result['full_producer_arrival_contract_qualified']=False
        result['scope']='Bounded R-front/MIR implementation budget only; complete producers, native IO/ROM/timing and60ns remain open.'
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--qualified',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--allow-missing-targeted-reports',action='store_true');a=p.parse_args()
    record=audit(a.directory.resolve(),a.qualified.resolve(),a.allow_missing_targeted_reports)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],record['minimum_slack_ns'],record['resources'])
