#!/usr/bin/env python3
"""Review actual terminal chip-width nineteen-board capacity, separately from timing.

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
    assert source['status']=='PASS_CHIPWIDTH_NATIVE_SOURCE_ONLY'and source['boards']==19
    assert source['original_pin_connections']==2761 and len(source['failure_controls'])==23
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
    records=rows(read('core/output_files/Dorado.fit.rpt'))
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
        reduction_from_serial63_compact=42449-needed,fit_registers=int(registers[1]),block_memory_bits=int(memory[1].replace(',','')),
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
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--source-audit',type=Path,required=True);p.add_argument('--classification',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    source=json.loads(a.source_audit.read_text());classification=json.loads(a.classification.read_text())
    result=review(a.directory,source,classification);result['failure_controls']=controls(a.directory,source,classification)
    result['input_hashes']={str(p.relative_to(a.directory)):hashlib.sha256(p.read_bytes()).hexdigest()for p in a.directory.rglob('*')if p.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k]for k in ('status','alms','available_alms','excess_alms','reduction_from_serial63_compact')}))
