#!/usr/bin/env python3
"""Start one source-qualified prepared-Hold diagnostic fit; never assemble/load."""
import argparse
import json
from pathlib import Path
import shutil

from start_native_phase_guard_timing import launch_pinned_stage
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def start(stage,publication,audit,parent_audit,parent_timing,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['prepared_MemC_Hold_staged']and len(m['source_hashes'])==581 and not m['assembly_permitted']
    parent=Path(m['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
    assert sha(parent/'r-front-control-native.json')==m['parent_manifest_sha256']
    a=json.loads((audit/'results.json').read_text());pa=json.loads((parent_audit/'results.json').read_text())
    assert a['status']=='PASS_NATIVE_PREPARED_HOLD_STRUCTURAL_AUDIT'and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert a['unchanged_original_sequential_processes']==3813 and a['added_host_preparation_processes']==1
    assert pa['status']=='PASS_NATIVE_RIOT_BLOCK_RAM_STRUCTURAL_AUDIT'and pa['stage_manifest_sha256']==m['parent_manifest_sha256']
    timing=json.loads((parent_timing/'results.json').read_text())
    assert timing['status']=='REVIEWED_ROUTED_NATIVE_CE_CORE_WITH_REAL_SETUP_FAILURE'
    assert timing['stage_manifest_sha256']==m['parent_manifest_sha256']and timing['terminal_all_nine_stages_zero']
    p=json.loads((publication/'run-state.json').read_text())
    assert p['status']=='PASS_COMPLETE_NATIVE_PREPARED_HOLD_PUBLICATION'and p['assumptions']==0
    assert p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert p['composed_from_induction_and_unrestricted_full_interface_congruence']and all(v['passed']for v in p['actions'])
    assert p['source_bound_buffer_induction_sha256']==m['prepared_MemC_Hold_induction_sha256']
    for v in p['actions']:
        verify(publication/v['label'],v['files'])
        assert sha(publication/v['label']/'proof.log')==v['log_sha256']
    # The parent has the complete original ROM/RIOT/clock ancestry. Structural
    # audit proves this candidate leaves those sources/processes unchanged.
    for receipt in pm['RIOT_block_ram_receipts'].values():
        d=Path(receipt['directory']);states=[q for q in [d/'results.json',d/'run-state.json']if q.is_file()and sha(q)==receipt['state_sha256']]
        assert len(states)==1
        value=json.loads(states[0].read_text());verify(d,value['files'])
    builder=stage/'build_r_front_ready_native.py'
    assert sha(builder)==sha(parent/'build_r_front_ready_native.py')
    out.mkdir(parents=True);shutil.copy2(Path(__file__),out/Path(__file__).name)
    preflight=dict(prepared_Hold_publication_sha256=sha(publication/'run-state.json'),
                   structural_audit_sha256=sha(audit/'results.json'),parent_audit_sha256=sha(parent_audit/'results.json'),
                   parent_timing_review_sha256=sha(parent_timing/'results.json'),
                   diagnostic_fit_only=True,preparation_waits_and60ns_schedule_still_need_measurement=True)
    return launch_pinned_stage(stage,out,m,preflight)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','publication','audit','parent-audit','parent-timing','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();start(a.stage.resolve(),a.publication.resolve(),a.audit.resolve(),a.parent_audit.resolve(),a.parent_timing.resolve(),a.output.resolve())
