#!/usr/bin/env python3
"""Read terminal matched compact/chip-width full-MSA area and all timing corners.

All actual inputs are checked against the qualified parent fixture and source
receipts. Component savings do not establish complete-core capacity or speed.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
from generate_serial_boards import raw_receipt
from audit_msa_compact_fit import timing
import audit_msa_chipwidth as source_audit

ROOT=Path(__file__).resolve().parents[3]


def sha(raw):return hashlib.sha256(raw).hexdigest()


def audit(root,reports,overrides=None):
    overrides=overrides or {};raw=lambda n:overrides[n]if n in overrides else(root/n).read_bytes();read=lambda n:raw(n).decode('latin1')
    record=json.loads(read('provenance.json'));assert 'Matched chipwidth MSA fit terminal'in read('driver.log')
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert sha(raw(name))==digest,'compiled input changed: '+name
    parent=reports/'2026-10-19-snapshot-msa-compact'
    previous=json.loads(raw_receipt(parent,'fit-terminal/provenance.json'))
    for key in ('expected_top','expected_qsf','expected_sdc','expected_audit'):assert record[key]==previous[key],'unapproved fixture/policy change'
    for name in ['msa_board.sv',*record['reference_manifest']['cloned_sources']]:
        assert raw('baseline/'+name)==raw_receipt(parent,'msa-v2/candidate/'+name),'baseline differs from qualified compact source'
    manifest=record['candidate_manifest'];source_audit.audit(root/'chipwidth',manifest,reports,
        {n:raw('chipwidth/'+n)for n in ['msa_board.sv',*manifest['cloned_sources']]})
    results={}
    for label in ('baseline','chipwidth'):
        qsf=read(label+'/snapshot_fit.qsf');expected=previous['expected_qsf'];suffix='\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        assert qsf in (expected,expected+suffix,expected+suffix+'\n'),'unapproved fitter policy'
        for name,key in [('top.sv','expected_top'),('bank.sdc','expected_sdc'),('audit.tcl','expected_audit')]:
            assert read(label+'/'+name)==previous[key],'actual fixture/timing differs'
        summary=read(label+'/snapshot_fit.fit.summary');mapping=read(label+'/snapshot_fit.map.summary')
        assert 'Fitter Status : Successful'in summary and 'Device : 5CSEBA6U23I7'in summary
        assert 'Analysis & Synthesis Status : Successful'in mapping
        for text in ('Total virtual pins : 991','Total block memory bits : 0 /','Total DSP Blocks : 0 /'):assert text in summary,'resource/coverage changed'
        area=re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / 41,910',summary);regs=re.search(r'Total registers : (\d+)',summary)
        assert area and regs;t=timing(read,label)
        results[label]=dict(alms=int(area[1].replace(',','')),registers=int(regs[1]),timing=t,minimum_slack_ns=min(x['slack_ns']for x in t))
    saving=results['baseline']['alms']-results['chipwidth']['alms']
    return dict(status='PASS_MATCHED_COMPONENT_SAVING'if saving>0 else'REJECTED_NO_AREA_BENEFIT',scope=__doc__,results=results,
        saved_alms=saving,actual_physical_bits=dict(baseline=1650,chipwidth=1538),hardware_changed=False,
        limits='No combined fit, native schema/barrier, complete FPGA snapshot or speed claim.')


def controls(root,reports):
    cases=[('loose_clock','chipwidth/bank.sdc','20.000','40.000'),
        ('area_policy','chipwidth/snapshot_fit.qsf','Aggressive Area','HIGH PERFORMANCE EFFORT'),
        ('bad_hold','chipwidth/bank-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
        ('missing_setup','chipwidth/bank-setup-MIN_fast_1100mv_-40c.rpt','Worst case slack is','Missing slack was'),
        ('wrong_width','chipwidth/storage_group.sv','reg [WIDTH-1:0] cache_data=0;','reg [WIDTH-2:0] cache_data=0;')]
    results=[]
    for name,path,before,after in cases:
        source=(root/path).read_bytes();assert before.encode()in source;damaged=source.replace(before.encode(),after.encode(),1)
        record=copy.deepcopy(json.loads((root/'provenance.json').read_text()))
        if path in record['compiled_files']:record['compiled_files'][path]=sha(damaged)
        if path.endswith('bank.sdc'):record['expected_sdc']=damaged.decode()
        if path=='chipwidth/storage_group.sv':record['candidate_manifest']['cloned_sources']['storage_group.sv']=sha(damaged)
        try:audit(root,reports,{path:damaged,'provenance.json':json.dumps(record).encode()})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged chip-width fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory,a.reports);result['failure_controls']=controls(a.directory,a.reports)
    result['input_hashes']={str(p.relative_to(a.directory)):sha(p.read_bytes())for p in a.directory.rglob('*')if p.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],saved_alms=result['saved_alms'],minimum_slack_ns=min(r['minimum_slack_ns']for r in result['results'].values()),failure_controls=len(result['failure_controls']))))
