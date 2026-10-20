#!/usr/bin/env python3
"""Recheck Serial74 native source, complete state accounting and negative readers.

This verifies source-only integration. Native access, actual Main controls,
remaining RAM/DMadr timing, whole-machine snapshots and FPGA gates stay open.
"""
import argparse
import json
from pathlib import Path
import audit_native_phase_distribution as source
import inventory_native_phase_distribution as state


def verify(project,classification,reports,out):
    actual=source.audit(project,reports);actual['failure_controls']=source.controls(project,reports)
    assert actual==json.loads((project/'source-audit.json').read_text()),'source audit does not replay'
    record=json.loads((project/'phase-distribution-integration.json').read_text())
    assert len(record['source_hashes'])==663 and actual['verified_parent_inputs']==662
    for name,digest in record['source_hashes'].items():assert source.sha((project/name).read_bytes())==digest,name
    raw=(classification/'native.xml').read_bytes();data,normalized=state.read_inventory(raw)
    assert normalized==(classification/'native-normalized.xml').read_bytes(),'normalized XML differs'
    data['bindings']=state.prior.prior.prior.prior.bindings(normalized)
    data,review=state.prior.prior.prior.review_subroutines(data,normalized)
    data['phase_binding']=state.phase_binding(normalized);data['payload_bindings']=state.payload_bindings(normalized)
    expected=state.prior.prior.prior.expected_fields(reports)
    old=state.payload_bindings(source.raw_receipt(reports/source.PARENT,'qualification/classification/native-normalized.xml'))
    result=state.classify(data,expected,old);result['subroutine_review']=review
    result['failure_controls']=state.controls(data,expected,old)+state.alias_controls(raw);result['source_hashes']=record['source_hashes']
    assert result==json.loads((classification/'classification.json').read_text()),'state audit does not replay'
    assert data==json.loads((classification/'inventory.json').read_text()),'inventory does not replay'
    assert result['new_required_wrapper_bits']==19 and result['totals']['WRAPPER ACCESS PENDING']==132638
    assert len(result['failure_controls'])==34 and len(actual['failure_controls'])==13
    summary=dict(status='PASS_NATIVE_PHASE_DISTRIBUTION_SOURCE_AND_STATE',scope=__doc__,actual_inputs=663,
        verified_parent_inputs=662,changed_inputs=len(actual['changed_parent_inputs']),source_controls=13,state_controls=34,
        original_pin_connections=2761,new_required_bits=19,required_bits=sum(result['totals'].values()),
        phase_payload_helpers=len(result['payload_bindings']),schema_id=actual['schema_id'],
        native_state_access_complete=False,quartus_permitted=False,assembly_permitted=False,
        source_hashes=record['source_hashes'],state_totals=result['totals'])
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(summary,indent=2)+'\n')
    print(summary['status'],summary['actual_inputs'],'inputs;',summary['required_bits'],'required bits; access pending')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path);p.add_argument('classification',type=Path)
    p.add_argument('--reports',type=Path,default=source.ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();verify(a.project.resolve(),a.classification.resolve(),a.reports.resolve(),a.output.resolve())
