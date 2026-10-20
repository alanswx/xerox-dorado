#!/usr/bin/env python3
"""Review terminal nineteen-board capacity, including real memory-pause integration.

A failed-fit estimate is diagnostic; a successful fit still requires timing,
real service barriers, full snapshots and FPGA qualification. No deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from audit_enabled_capacity import rows

MAIN='ContA ContB ProcH ProcL IFU MemC MemD MemX DskEth DispY BaseBd'.split()
MSA='b_msa b_msaOdd b_msa1 b_msa1Odd b_msa2 b_msa2Odd b_msa3 b_msa3Odd'.split()


def review(root,source,classification,overrides=None):
    overrides=overrides or {};read=lambda n:overrides.get(n,(root/n).read_text(encoding='latin1'))
    summary=read('core/output_files/Dorado.fit.summary');log=read('fit.log');driver=read('driver.log')
    state=json.loads(read('terminal-state.json'))
    assert state['status'].startswith('TERMINAL')and not state['builder_live']and not state['assembly_started']and not state['rbfs']
    assert 'Analysis & Synthesis Status : Successful'in read('core/output_files/Dorado.map.summary')
    assert 'Device : 5CSEBA6U23I7'in summary and 'Total virtual pins : 7'in summary
    assert source['original_pin_connections']==2761
    memory_fanout=source['status']=='PASS_NATIVE_MEMORY_FANOUT_SOURCE_ONLY'
    memory_pause=source['status']=='PASS_NATIVE_MEMORY_PAUSE_SOURCE_ONLY'or memory_fanout
    if memory_pause:
        assert source['verified_parent_inputs']==659 and source['actual_inputs']==660
        assert state['prepared_inputs']==660 and state['prepared_inputs_changed']==[]
        assert len(source['failure_controls'])==(6 if memory_fanout else 10) and source['constraints_unchanged']
        if memory_fanout:
            assert source['verified_policy_parent_inputs']==660 and source['new_policy_state_bits']==0
            assert source['changed_policy_files']==['core/Dorado.qsf']
            assert(source['target'],source['max_fanout'])==('emu|network_step',256)
            assert re.search(r';\s*emu:emu\|network_step\s*;\s*256\s*;\s*4\s*;',read('core/output_files/Dorado.map.rpt')),'actual fanout setting/replication differs'
            assert 'Inserted 4 logic cells for Maximum Fan-Out assignment on "emu:emu|network_step"'in read('map.log'),'mapper did not apply targeted replication'
        assert set(source['changed_parent_inputs'])=={
            'core/Dorado.qsf','core/Dorado.sv',
            'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v',
            'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv',
            'src/fpga/mister/dorado_storage_arb.sv','src/fpga/mister/dorado_storage_sdram.sv',
            'src/fpga/mister/snapshot_serial/assets.json','src/fpga/mister/snapshot_serial/MSA__msa_board.sv'}
        assert (source['new_state_bits'],source['new_clocks'],source['new_ports'])==(2,0,0)
        assert source['schema_id']=='af0b647cb43a3a62'
        assert source['real_memory_pause'] and not source['real_pause_barriers']
        assert classification['status']=='PASS_NATIVE_MEMORY_PAUSE_STATE_ACCESS_PENDING'
        assert len(classification['failure_controls'])==15 and not classification['sequential_exclusions']
        assert classification['totals']=={'PHYSICAL BOARD':1957048,'PRIVATE BOARD':4578,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132606}
    elif source['status']in ('PASS_READY_PATH_SOURCE_ONLY','PASS_TARGETED_FANOUT_SOURCE_ONLY','PASS_GLOBAL_SIGNAL_SOURCE_ONLY','PASS_PLACEMENT_SEED_SOURCE_ONLY'):
        assert source['verified_parent_inputs']==659 and len(source['failure_controls'])==6
        assert source['constraints_unchanged']
        if source['status']=='PASS_READY_PATH_SOURCE_ONLY':assert source['changed_logic_files']==['core/Dorado.sv']
        else:
            assert source['changed_logic_files']==[]and source['changed_policy_files']==['core/Dorado.qsf']
            if source['status']=='PASS_TARGETED_FANOUT_SOURCE_ONLY':
                assert (source['target'],source['max_fanout'])==('emu|network_step',64)
            elif source['status']=='PASS_GLOBAL_SIGNAL_SOURCE_ONLY':
                assert (source['target'],source['global_signal'])==('emu|network_step','Global Clock')
            else:assert source['seed']==2
        assert (source['new_state_bits'],source['new_clocks'],source['new_ports'])==(0,0,0)
        assert source['schema_id']=='af0b647cb43a3a62'and not source['real_pause_barriers']
    else:
        assert source['status']=='PASS_CHIPWIDTH_NATIVE_SOURCE_ONLY'and source['boards']==19
        assert len(source['failure_controls'])==23
    if not memory_pause:
        assert classification['status']=='PASS_BOARD_GEOMETRY_WRAPPER_ACCESS_PENDING'and len(classification['failure_controls'])==14
        assert classification['totals']=={'PHYSICAL BOARD':1957048,'PRIVATE BOARD':4578,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132484}
    match=re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / ([\d,]+)',summary);assert match
    needed,available=(int(x.replace(',',''))for x in match.groups());assert available==41910
    successful='Fitter Status : Successful'in summary
    if successful:
        assert needed<=available and 'Quartus Prime Fitter was successful'in log
        assert state['status']!='TERMINAL_FAILED_FIT'
        status='PASS_CAPACITY_TIMING_AND_BARRIERS_PENDING'
    else:
        assert 'Fitter Status : Failed'in summary and needed>available
        assert 'Error (11802): Can\'t fit design in device.'in log
        assert 'returned non-zero exit status 3'in driver
        assert state['status']=='TERMINAL_FAILED_FIT'and not state['sta_started']
        labs=re.search(r'Error \(170012\): Fitter requires (\d+) LABs.*only (\d+) LABs',log)
        assert labs and int(labs[1])>int(labs[2])==4191
        status='REJECTED_CAPACITY'
    report=read('core/output_files/Dorado.fit.rpt')
    if source['status']=='PASS_PLACEMENT_SEED_SOURCE_ONLY':
        assert re.search(r';\s*Fitter Initial Placement Seed\s*;\s*2\s*;\s*1\s*;',report),'fitter seed not applied'
    records=rows(report)
    boards=[r for r in records if r['Entity Name']in {'dorado_serial_'+n for n in MAIN}|{'dorado_serial_MSA'}]
    wanted={f'|dorado_serial_{n}:b_{n}|'for n in MAIN}|{f'|dorado_serial_MSA:{n}|'for n in MSA}
    assert len(boards)==19 and {r['Compilation Hierarchy Node']for r in boards}==wanted,'nineteen-board hierarchy missing/duplicated'
    transport={n:[r for r in records if r['Entity Name']==n]for n in ('dorado_board_state_fabric_msa','dorado_board_state_uio_msa','dorado_msa_state_group_chipwidth')}
    assert all(len(v)==1 for v in transport.values()),'enabled owner/UIO/router missing'
    estimate=lambda r:float(r['ALMs needed [=A-B+C]'].split()[0])
    own=lambda r:float(r['ALMs needed [=A-B+C]'].split()[1].strip('()'))
    registers=re.search(r'Total registers : (\d+)',summary);memory=re.search(r'Total block memory bits : ([\d,]+) /',summary)
    assert registers and memory
    return dict(status=status,scope=__doc__,alms=needed,available_alms=available,excess_alms=max(0,needed-available),
        reduction_from_serial63_compact=42449-needed,increase_from_serial64_chipwidth=needed-41381,
        fit_registers=int(registers[1]),block_memory_bits=int(memory[1].replace(',','')),
        boards=[dict(instance=r['Full Hierarchy Name'],module=r['Entity Name'],fitter_alm_attribution=estimate(r))for r in boards],
        eight_msa_fitter_attribution_alms=round(sum(estimate(r)for r in boards if r['Entity Name']=='dorado_serial_MSA'),1),
        transport={n:estimate(v[0])for n,v in transport.items()},
        largest_exclusive_attribution=[dict(instance=r['Full Hierarchy Name'],module=r['Entity Name'],self_attribution_alms=own(r))for r in sorted(records,key=own,reverse=True)[:24]],
        legal_capacity=successful,timing_qualified=False,assembly_started=False,hardware_changed=False,
        limits='Failed-fit zero RAM-block count is not physical RAM qualification. No all-device timing, real service barrier, complete FPGA snapshot or speed claim.')


def controls(root,source,classification):
    cases=[('wrong_capacity','core/output_files/Dorado.fit.summary','/ 41,910','/ 50,000'),
        ('missing_msa','core/output_files/Dorado.fit.rpt','dorado_serial_MSA:b_msa3Odd','dorado_serial_MSA:b_other'),
        ('missing_router','core/output_files/Dorado.fit.rpt','dorado_msa_state_group_chipwidth','missing_msa_state_group')]
    result=[]
    for name,path,before,after in cases:
        raw=(root/path).read_text(encoding='latin1');assert before in raw
        try:review(root,source,classification,{path:raw.replace(before,after)})
        except AssertionError as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged capacity report accepted: '+name)
    damaged=json.loads((root/'terminal-state.json').read_text());damaged['builder_live']=True
    try:review(root,source,classification,{'terminal-state.json':json.dumps(damaged)})
    except AssertionError as error:result.append(dict(case='live_builder',rejected=True,diagnostic=str(error)))
    else:raise AssertionError('live builder reported terminal')
    if source['status']=='PASS_PLACEMENT_SEED_SOURCE_ONLY':
        path='core/output_files/Dorado.fit.rpt';raw=(root/path).read_text(encoding='latin1')
        damaged,count=re.subn(r'(;\s*Fitter Initial Placement Seed\s*;\s*)2(\s*;\s*1\s*;)',
                             lambda m:m[1]+'3'+m[2],raw,count=1)
        assert count==1
        try:review(root,source,classification,{path:damaged})
        except AssertionError as error:result.append(dict(case='wrong_actual_seed',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unapplied placement seed accepted')
    if source['status']in('PASS_NATIVE_MEMORY_PAUSE_SOURCE_ONLY','PASS_NATIVE_MEMORY_FANOUT_SOURCE_ONLY'):
        cases=[('wrong_actual_input_count','actual_inputs',659),
                               ('fake_memory_pause','real_memory_pause',False),
                               ('unearned_full_barrier','real_pause_barriers',True)]
        if source['status']=='PASS_NATIVE_MEMORY_FANOUT_SOURCE_ONLY':cases.append(('wrong_policy_limit','max_fanout',64))
        for name,key,value in cases:
            damaged=dict(source);damaged[key]=value
            try:review(root,damaged,classification)
            except AssertionError as error:result.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged memory pause source accepted: '+name)
        damaged=dict(classification);damaged['totals']=dict(classification['totals'])
        damaged['totals']['WRAPPER ACCESS PENDING']-=120
        try:review(root,source,damaged)
        except AssertionError as error:result.append(dict(case='omitted_task_state',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('omitted SDRAM task state accepted')
    if source['status']=='PASS_NATIVE_MEMORY_FANOUT_SOURCE_ONLY':
        path='core/output_files/Dorado.map.rpt';raw=(root/path).read_text(encoding='latin1')
        damaged,count=re.subn(r'(;\s*emu:emu\|network_step\s*;\s*)256(\s*;\s*4\s*;)',lambda m:m[1]+'128'+m[2],raw,count=1)
        assert count==1
        try:review(root,source,classification,{path:damaged})
        except AssertionError as error:result.append(dict(case='wrong_applied_fanout',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('wrong applied fanout accepted')
        raw=(root/'map.log').read_text(encoding='latin1')
        damaged=raw.replace('Inserted 4 logic cells for Maximum Fan-Out assignment','Inserted 0 logic cells for Maximum Fan-Out assignment')
        assert damaged!=raw
        try:review(root,source,classification,{'map.log':damaged})
        except AssertionError as error:result.append(dict(case='missing_actual_replication',rejected=True,diagnostic=str(error)))
        else:raise AssertionError('unapplied replication accepted')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--source-audit',type=Path,required=True);p.add_argument('--classification',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    source=json.loads(a.source_audit.read_text());classification=json.loads(a.classification.read_text())
    result=review(a.directory,source,classification);result['failure_controls']=controls(a.directory,source,classification)
    result['input_hashes']={str(p.relative_to(a.directory)):hashlib.sha256(p.read_bytes()).hexdigest()for p in a.directory.rglob('*')if p.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k]for k in ('status','alms','available_alms','excess_alms','reduction_from_serial63_compact')}))
