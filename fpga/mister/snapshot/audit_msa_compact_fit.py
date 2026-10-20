#!/usr/bin/env python3
"""Review matched original/compact full-MSA area, every corner and exact sources.

An isolated saving is not a legal complete-core fit or a deployed speed result.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
from audit_storage_factored_fit import CORNERS
import audit_msa_compact as source_audit
from generate_serial_boards import raw_receipt

ROOT=Path(__file__).resolve().parents[3]


def sha(raw):return hashlib.sha256(raw).hexdigest()


def timing(read,label):
    result=[]
    for corner in CORNERS:
        for kind in ('setup','hold','mpw'):
            text=read(f'{label}/bank-{kind}-{corner}.rpt')
            match=re.search(r'Found (\d+) (?:\w+ )*\((\d+) violated\)\.\s+Worst case slack is\s+([-+\d.]+)',text)
            assert match and int(match[1])>0 and int(match[2])==0 and float(match[3])>=0,'timing violation/missing report'
            result.append(dict(corner=corner,kind=kind,slack_ns=float(match[3])))
        for kind in ('recovery','removal'):assert 'Nothing to report.'in read(f'{label}/bank-{kind}-{corner}.rpt')
        coverage=read(f'{label}/bank-ucp-{corner}.rpt')
        for prop in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
            match=re.search(r';\s*'+prop+r'\s*;\s*(\d+)\s*;\s*(\d+)\s*;',coverage)
            assert match and match.groups()==('0','0'),'unconstrained fixture path'
        clocks=read(f'{label}/bank-clocks-{corner}.rpt')
        assert re.findall(r'(?m)^;\s*(\S+)\s*;\s*(Base|Generated)\s*;',clocks)==[('host_clock','Base')]
        assert re.search(r';\s*host_clock\s*;\s*Base\s*;\s*20\.000\s*;\s*50\.0 MHz\s*;',clocks),'changed clock period'
    return result


def audit(root,reports,overrides=None):
    overrides=overrides or {};raw=lambda n:overrides.get(n,(root/n).read_bytes());read=lambda n:raw(n).decode('latin1')
    record=json.loads(read('provenance.json'))
    assert 'Matched compact MSA fit terminal'in read('driver.log'),'not terminal'
    for name,digest in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert sha(raw(name))==digest,'compiled input differs: '+name
    receipt_root=reports/'2026-10-19-snapshot-msa-clocked-rejected'
    previous=json.loads(raw_receipt(receipt_root,'fit-terminal/matched/provenance.json'))
    top=previous['expected_top']
    gate=''' wire ff_clock,ff_clock_request;
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),.intended_device_family("Cyclone V")) ff_clock_control(
 .inclk({3'b000,host_clk}),.clkselect(2'b00),.ena(advance_r || ff_clock_request),.outclk(ff_clock));
'''
    ports='.state_ff_clk(ff_clock),\n.state_ff_clock_request(ff_clock_request),\n'
    assert top.count(gate)==1 and top.count(ports)==1
    top=top.replace(gate,'',1).replace(ports,'',1)
    assert record['expected_top']==top and 'altclkctrl'not in top,'unapproved shell change'
    for key in ('expected_qsf','expected_sdc','expected_audit'):assert record[key]==previous[key],'fixture/policy changed'
    qualified=reports/'2026-10-19-snapshot-msa-transport';ref=lambda n:raw_receipt(qualified,'simulation-v5/candidate/'+n)
    assert raw('baseline/msa_board.sv')==ref('msa_board.sv'),'baseline changed'
    for name in record['reference_manifest']['cloned_sources']:
        assert raw('baseline/'+name)==ref(name),'baseline helper changed'
    manifest=record['candidate_manifest']
    source_audit.audit(root/'compact',manifest,reports,{n:raw('compact/'+n)for n in ['msa_board.sv',*manifest['cloned_sources']]})
    results={}
    for label in ('baseline','compact'):
        suffix='\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
        qsf=read(label+'/snapshot_fit.qsf');expected=previous['expected_qsf']
        assert qsf in (expected,expected+suffix,expected+suffix+'\n'),'unapproved fitter policy'
        for name,key in [('top.sv','expected_top'),('bank.sdc','expected_sdc'),('audit.tcl','expected_audit')]:
            assert read(label+'/'+name)==record[key],'actual fixture/timing changed'
        summary=read(label+'/snapshot_fit.fit.summary');mapping=read(label+'/snapshot_fit.map.summary')
        assert 'Fitter Status : Successful'in summary and 'Device : 5CSEBA6U23I7'in summary
        assert 'Analysis & Synthesis Status : Successful'in mapping
        for text in ('Total virtual pins : 991','Total block memory bits : 0 /','Total DSP Blocks : 0 /'):assert text in summary,'resource/coverage changed'
        area=re.search(r'Logic utilization \(in ALMs\) : ([\d,]+) / 41,910',summary)
        regs=re.search(r'Total registers : (\d+)',summary);mapregs=re.search(r'Total registers : (\d+)',mapping)
        assert area and regs and mapregs
        t=timing(read,label)
        results[label]=dict(alms=int(area[1].replace(',','')),registers=int(regs[1]),synthesis_registers=int(mapregs[1]),
                            timing=t,minimum_slack_ns=min(x['slack_ns']for x in t))
    saving=results['baseline']['alms']-results['compact']['alms']
    return dict(status='PASS_MATCHED_COMPONENT_SAVING'if saving>0 else'REJECTED_NO_AREA_BENEFIT',scope=__doc__,
        results=results,saved_alms=saving,actual_physical_bits=dict(baseline=2690,compact=1650),hardware_changed=False,
        limits='No combined fit, native catalog/barrier, complete FPGA snapshot or speed claim.')


def controls(root,reports):
    cases=[('loose_clock','compact/bank.sdc','20.000','40.000'),
           ('policy_change','compact/snapshot_fit.qsf','Aggressive Area','HIGH PERFORMANCE EFFORT'),
           ('bad_hold','compact/bank-hold-7_slow_1100mv_100c.rpt','(0 violated)','(1 violated)'),
           ('missing_setup','compact/bank-setup-MIN_fast_1100mv_-40c.rpt','Worst case slack is','Missing slack was'),
           ('cross_group','compact/msa_board.sv','.addr(dram_addr_0),.addr_will_change(dram_change_0),','.addr(dram_addr_0),.addr_will_change(dram_change_1),')]
    results=[]
    for name,path,before,after in cases:
        source=(root/path).read_bytes();assert before.encode()in source;damaged=source.replace(before.encode(),after.encode(),1)
        record=copy.deepcopy(json.loads((root/'provenance.json').read_text()))
        if path in record['compiled_files']:record['compiled_files'][path]=sha(damaged)
        if path.endswith('bank.sdc'):record['expected_sdc']=damaged.decode()
        if path=='compact/msa_board.sv':record['candidate_manifest']['generated_sha256']=sha(damaged)
        try:audit(root,reports,{path:damaged,'provenance.json':json.dumps(record).encode()})
        except AssertionError as error:results.append(dict(case=name,rejected=True,diagnostic=str(error),damaged_metadata_rehashed=True))
        else:raise AssertionError('damaged compact fit accepted: '+name)
    return results


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path)
    p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=audit(a.directory,a.reports);result['failure_controls']=controls(a.directory,a.reports)
    result['input_hashes']={str(x.relative_to(a.directory)):sha(x.read_bytes())for x in a.directory.rglob('*')if x.is_file()}
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],saved_alms=result['saved_alms'],minimum_slack_ns=min(r['minimum_slack_ns']for r in result['results'].values()),failure_controls=len(result['failure_controls']))))
