#!/usr/bin/env python3
"""Stage the parallel Ready comparison on the completed task-parallel core.

Preserve every physical state update, mismatch output, machine/storage gate,
clock, timer and constraint. The incremental change is two implementation
files plus one stateless helper. Full native timing only, no assembly/load.
"""
import argparse
import json
from pathlib import Path
import shutil

from qualify_native_ready_compare import CONTROL,READY,HELPER,control_transform,child_transform
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'


def prepare(parent,proof,xz,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert len(p['source_hashes'])==574 and p['frequency_hz']==66666667 and p['sysper']==4 and not p['assembly_permitted']
    assert p['task_parallel_proof_state_sha256']
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_ACTUAL_NATIVE_READY_COMPARISON'and q['arbitrary_binary_input_bits']==1982 and q['compared_outputs']==2969 and q['assumptions']==0
    assert q['source_control_sha256']==sha(parent/CONTROL)and q['source_ready_sha256']==sha(parent/READY)
    assert q['helper_sha256']==sha(ROOT/HELPER)
    for a in q['actions']:
        d=proof/a['label'];verify(d,a['files']);assert a['passed']and a['returncode']==0 and sha(d/'proof.log')==a['log_sha256']
    x=checked(xz);assert x['status']=='PASS_ACTUAL_NATIVE_READY_COMPARE_XZ'and x['proof_state_sha256']==sha(proof/'run-state.json')
    assert 'known=3 unknown=11892 checks=11895 outputs=2969'in(xz/'test.log').read_text()
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-ready-compare-{mode}-{engine}-v1-20261025';r,row=observed(d)
            assert r['ready_compare_proof_state_sha256']==sha(proof/'run-state.json')
            assert r['original_assertions_and_reference_unchanged']and r['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            assert sha(d/'candidate.sv')==sha(proof/'native-control-after.sv')
            assert sha(d/'adapters/dorado_local_r_front_ready_step.sv')==sha(proof/'native-ready-after.sv')
            assert sha(d/'adapters/dorado_ready_compare_step.sv')==sha(ROOT/HELPER)
            if r['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:assert r['status']=='PASS_ACTUAL_NATIVE_PHASE_GUARD_PROCESSOR_REPLAY'
            replays.append(row)
    changed={CONTROL:control_transform((parent/CONTROL).read_text()).encode(),READY:child_transform((parent/READY).read_text()).encode(),
             'src/'+HELPER:(ROOT/HELPER).read_bytes()}
    assert changed[CONTROL]==(proof/'native-control-after.sv').read_bytes()and changed[READY]==(proof/'native-ready-after.sv').read_bytes()
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output.update(changed)
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    output['core/Dorado.qsf']+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/('src/'+HELPER))+'\n').encode()
    out.mkdir(parents=True)
    for n,raw in output.items():
        d=out/n;d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(raw)
    for n in ['build_r_front_ready_native.py',Path(__file__).name]:shutil.copy2(ROOT/'tools'/n,out/n)
    hashes={n:sha(out/n)for n in output};assert len(hashes)==575
    for n in p['source_hashes']:
        if n not in changed and Path(n).suffix not in ['.qsf','.qip','.qpf','.sdc','.tcl']:assert hashes[n]==p['source_hashes'][n]
        if n.endswith('.sdc'):assert(out/n).read_bytes()==(parent/n).read_bytes().replace(p['project'].encode(),str(project).encode())
    result={**p,'status':'PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC','scope':__doc__,
        'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),
        'source_hashes':hashes,'extra_RTL_sources':sorted(set(p['extra_RTL_sources'])|{'src/'+HELPER}),
        'ready_compare_proof':str(proof),'ready_compare_proof_state_sha256':sha(proof/'run-state.json'),
        'ready_compare_XZ':str(xz),'ready_compare_XZ_state_sha256':sha(xz/'run-state.json'),
        'ready_compare_replays':replays,'pending_ready_compare_Icarus_peers':pending,
        'incremental_functional_HDL_changes':sorted(changed),'functional_HDL_changes':sorted(set(p['functional_HDL_changes'])|set(changed)),
        'all2969_prefix_outputs_retained':True,'shared_state_mismatch_guard_retained':True,
        'nominal_ordinary_ns':120,'full60ns_event_batch_connected':False,'no_new_timing_exceptions':True,
        'assembly_permitted':False,'hardware_speedup_qualified':False,'physical_timing_qualified':False,'firmware_qualified':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_NATIVE_READY_COMPARISON',len(hashes),'inputs; changed HDL3; pending peers',len(pending));return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','xz','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),a.xz.resolve(),a.output.resolve(),a.project)
