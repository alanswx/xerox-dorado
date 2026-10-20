#!/usr/bin/env python3
"""Check a terminal nineteen-board capacity rejection; never implies legal timing.

Failed-fit hierarchy attribution is diagnostic. It cannot predict another fit,
and the failed summary's zero RAM-block count is not a physical RAM result.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from audit_enabled_capacity import rows

MAIN='ContA ContB ProcH ProcL IFU MemC MemD MemX DskEth DispY BaseBd'.split()
MSA='b_msa b_msaOdd b_msa1 b_msa1Odd b_msa2 b_msa2Odd b_msa3 b_msa3Odd'.split()


def review(directory,source,classification,overrides=None):
    overrides=overrides or {}
    def read(name):return overrides.get(name,(directory/name).read_text(encoding='latin1'))
    summary=read('core/output_files/Dorado.fit.summary');log=read('fit.log')
    assert 'Fitter Status : Failed'in summary and 'Device : 5CSEBA6U23I7'in summary
    assert 'Quartus Prime Fitter was unsuccessful. 2 errors'in log
    assert 'Error (11802): Can\'t fit design in device.'in log
    assert 'returned non-zero exit status 3'in read('driver.log')
    assert 'Analysis & Synthesis Status : Successful'in read('core/output_files/Dorado.map.summary')
    state=json.loads(read('terminal-state.json'))
    assert state['status']=='TERMINAL_FAILED_FIT'and not state['sta_started']and not state['assembly_started']and not state['rbfs']
    alms=re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / ([\d,]+)',summary)
    labs=re.search(r'Error \(170012\): Fitter requires (\d+) LABs.*only (\d+) LABs',log)
    assert alms and labs
    needed,available=(int(x.replace(',',''))for x in alms.groups())
    assert needed>available==41910 and int(labs[1])>int(labs[2])==4191
    assert 'Total virtual pins : 7'in summary
    assert source['status']=='PASS_SOURCE_ONLY'and source['boards']==19 and len(source['failure_controls'])==23
    assert classification['status']=='PASS_BOARD_GEOMETRY_WRAPPER_ACCESS_PENDING'
    assert classification['totals']['PHYSICAL BOARD']==1966264 and classification['totals']['WRAPPER ACCESS PENDING']==132484
    records=rows(read('core/output_files/Dorado.fit.rpt'))
    boards=[r for r in records if r['Entity Name']in {'dorado_serial_'+n for n in MAIN}|{'dorado_serial_MSA'}]
    wanted={f'|dorado_serial_{n}:b_{n}|'for n in MAIN}|{f'|dorado_serial_MSA:{n}|'for n in MSA}
    assert len(boards)==19 and {r['Compilation Hierarchy Node']for r in boards}==wanted,'nineteen-board fitted attribution missing/duplicated'
    transport={n:[r for r in records if r['Entity Name']==n]for n in ('dorado_board_state_fabric_msa','dorado_board_state_uio_msa','dorado_msa_state_group')}
    assert all(len(v)==1 for v in transport.values()),'enabled owner/UIO/router missing'
    def estimate(r):return float(r['ALMs needed [=A-B+C]'].split()[0])
    def own(r):return float(r['ALMs needed [=A-B+C]'].split()[1].strip('()'))
    top=[dict(instance=r['Full Hierarchy Name'],module=r['Entity Name'],self_attribution_alms=own(r))for r in sorted(records,key=own,reverse=True)[:24]]
    attribution=[dict(instance=r['Full Hierarchy Name'],module=r['Entity Name'],failed_fit_alm_estimate=estimate(r),registers=int(r['Dedicated Logic Registers'].split()[0]))for r in boards]
    return dict(status='REJECTED_CAPACITY',scope=__doc__,alms_needed=needed,available_alms=available,excess_alms=needed-available,
                labs_needed=int(labs[1]),available_labs=int(labs[2]),boards=attribution,
                eight_msa_failed_fit_attribution_alms=round(sum(estimate(r)for r in boards if r['Entity Name']=='dorado_serial_MSA'),1),
                transport={n:estimate(v[0])for n,v in transport.items()},largest_exclusive_attribution=top,
                source_audit='PASS_SOURCE_ONLY',geometry_audit='PASS_BOARD_GEOMETRY_WRAPPER_ACCESS_PENDING',
                sta_started=False,assembly_started=False,hardware_changed=False,
                comparison_limit='Earlier 45,228-ALM experiment lacked the eight enabled MSA adapters. Coverage differs; this is not an equal-coverage serial/parallel comparison.')


def controls(directory,source,classification):
    cases=[('false_success','core/output_files/Dorado.fit.summary','Fitter Status : Failed','Fitter Status : Successful'),
           ('wrong_capacity','core/output_files/Dorado.fit.summary','/ 41,910','/ 50,000'),
           ('missing_msa','core/output_files/Dorado.fit.rpt','dorado_serial_MSA:b_msa3Odd','dorado_serial_MSA:b_other'),
           ('missing_owner','core/output_files/Dorado.fit.rpt','dorado_msa_state_group','missing_msa_state_group')]
    results=[]
    for name,path,before,after in cases:
        raw=(directory/path).read_text(encoding='latin1');assert before in raw
        try:review(directory,source,classification,{path:raw.replace(before,after)})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged capacity report accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--source-audit',type=Path,required=True);p.add_argument('--classification',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();source=json.loads(a.source_audit.read_text());classification=json.loads(a.classification.read_text())
    result=review(a.directory,source,classification);result['failure_controls']=controls(a.directory,source,classification)
    inputs=[a.source_audit,a.classification,Path(__file__),*[x for x in a.directory.rglob('*')if x.is_file()]]
    result['input_hashes']={str(x):hashlib.sha256(x.read_bytes()).hexdigest()for x in inputs if x!=a.output}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k]for k in ('status','alms_needed','available_alms','excess_alms','eight_msa_failed_fit_attribution_alms')}))
