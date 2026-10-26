#!/usr/bin/env python3
"""Launch one proved combinational Ready comparison diagnostic; no assembly."""
import argparse
import json
from pathlib import Path
import shutil

from review_baseboard_clock_consumer import verify
from start_native_phase_guard_timing import launch_pinned_stage
from test_external_r_packet import sha


def start(stage,audit,parent_audit,parent_timing,publication,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['parallel_Ready_target_comparison_staged'] and len(m['source_hashes'])==582
    assert not m['assembly_permitted'] and m['additional_host_or_guest_cycles']==0
    parent=Path(m['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
    assert sha(parent/'r-front-control-native.json')==m['parent_manifest_sha256']
    a=json.loads((audit/'results.json').read_text());pa=json.loads((parent_audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_READY_MODE_STRUCTURAL_AUDIT' and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['unchanged_original_sequential_processes']==3814 and a['added_sequential_processes']==0
    assert pa['status']=='PASS_NATIVE_PREPARED_HOLD_STRUCTURAL_AUDIT' and pa['stage_manifest_sha256']==m['parent_manifest_sha256']
    t=json.loads((parent_timing/'results.json').read_text())
    assert t['status']=='REJECTED_PREPARED_HOLD_CORE_TIMING_WITH_MIR_READY_PATH_IDENTIFIED'
    assert t['stage_manifest_sha256']==m['parent_manifest_sha256'] and t['all_nine_report_stages_zero']
    p=json.loads((publication/'run-state.json').read_text())
    assert p['status']=='PASS_COMPLETE_NATIVE_PREPARED_HOLD_PUBLICATION' and p['stage_manifest_sha256']==m['parent_manifest_sha256']
    assert p['assumptions']==0 and all(v['passed']for v in p['actions'])
    receipts={}
    for name,receipt in m['parallel_Ready_target_receipts'].items():
        d=Path(receipt['directory']);f=d/('results.json'if name=='component'else'run-state.json')
        assert sha(f)==receipt['state_sha256'];value=json.loads(f.read_text());receipts[name]=value
        if 'files'in value:verify(d,value['files'])
        assert name=='component' or all(v['passed']for v in value['actions'])
        if name in ['proof','native']:
            for v in value['actions']:
                verify(d/v['label'],v['files']);assert sha(d/v['label']/'proof.log')==v['log_sha256']
    n=receipts['native']
    assert n['status']=='PASS_COMPLETE_NATIVE_READY_MODE_COMPARISON' and n['stage_manifest_sha256']==m['parent_manifest_sha256']
    assert n['assumptions']==0 and n['compared_public_outputs']==2970 and n['arbitrary_binary_input_bits']==1982
    assert receipts['proof']['status']=='PASS_SOURCE_BOUND_READY_MODE_AND_COMPARISON'
    assert receipts['tests']['status']=='PASS_DUAL_ENGINE_READY_MODE_COMPARISON_AND_XZ_FALLBACK'
    assert receipts['witness']['status']=='PASS_DUAL_ENGINE_READY_SPARSE_WITNESS_AND_FIX'
    assert sha(stage/'build_r_front_ready_native.py')==sha(parent/'build_r_front_ready_native.py')
    out.mkdir(parents=True);shutil.copy2(Path(__file__),out/Path(__file__).name)
    preflight=dict(Ready_target_receipts=m['parallel_Ready_target_receipts'],
        structural_audit_sha256=sha(audit/'results.json'),parent_audit_sha256=sha(parent_audit/'results.json'),
        parent_timing_review_sha256=sha(parent_timing/'results.json'),
        inherited_Hold_publication_sha256=sha(publication/'run-state.json'),diagnostic_fit_only=True,
        additional_host_or_guest_cycles=0,added_sequential_processes=0,
        original60ns_phase_integration_and_hardware_speed_still_unqualified=True)
    return launch_pinned_stage(stage,out,m,preflight)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','audit','parent-audit','parent-timing','publication','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();start(a.stage.resolve(),a.audit.resolve(),a.parent_audit.resolve(),a.parent_timing.resolve(),a.publication.resolve(),a.output.resolve())
