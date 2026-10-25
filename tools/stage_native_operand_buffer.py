#!/usr/bin/env python3
"""Connect the proved early operand/flag view to the actual native backplane.

The whole-machine gate waits for a matching pre-event view. Every old result,
Ready/Hold/settling/alias/storage guard, SRAM owner, original R cell and capture
edge stays intact. Paired/unknown paths retain raw views. This is a timing-only
integration at the existing120ns cadence, not original60ns qualification.
"""
import argparse
import json
from pathlib import Path
import shutil

from integrate_native_operand_buffer import control,caller,HELPER
from prove_cp_prefix_cache import module
from qualify_native_ready_compare import CONTROL,SINGLE
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_processor_branch_composition import checked
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
CALLER='src/verilog/rtl/caller.sv'


def backplane(raw):
    original=raw;edits=[]
    for a,b in [('  wire [843:0] r_front_q,r_front_held,r_front_next;',
                 '  wire [843:0] r_front_q,r_front_held,r_front_next;\n  wire [843:0] r_front_prepared_q;'),
                ('  assign r_front_q=r_first_result[859:16];','  assign r_front_q=r_front_prepared_q;'),
                ('.native_R_frame(r_capture_frame),','.native_R_frame(r_capture_frame),.prepared_physical_state(r_front_prepared_q),')]:
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1);edits.append((a,b))
    restored=raw
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==original
    return raw


def prepare(parent,epoch,proof,xz,negative,out,project,replay_version='v2'):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert len(p['source_hashes'])==577 and p['registered_MemC_tag_reads_connected']and p['registered_R_read_buffer_connected']
    assert p['actual_native_single_comparison_connected']and not p['assembly_permitted']
    e=json.loads((epoch/'run-state.json').read_text());verify(epoch,e['files'])
    assert e['status']=='PASS_ACTUAL_NATIVE_PRE_EVENT_OPERAND_VIEW'and e['host_clock_varies_independently']
    assert e['source_control_sha256']==sha(parent/CONTROL)and e['assumptions']==0
    assert module((parent/CONTROL).read_text(),SINGLE)==(epoch/'pre-event-view'/(SINGLE+'.sv')).read_text()
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW'and q['epoch_host_clock_independence_proved']and q['assumptions']==0
    assert q['epoch_proof_state_sha256']==sha(epoch/'run-state.json')and q['helper_sha256']==sha(ROOT/HELPER)
    for directory,record in [(epoch,e),(proof,q)]:
        for a in record['actions']:
            d=directory/a['label'];verify(d,a['files']);assert a['passed']and a['returncode']==0 and sha(d/'proof.log')==a['log_sha256']
    for d,status in [(xz,'PASS_NATIVE_OPERAND_UNKNOWN_FALLBACK'),(negative,'PASS_NATIVE_OPERAND_UNKNOWN_BYPASS_COUNTEREXAMPLE')]:
        s=checked(d);assert s['status']==status and s['epoch_state_sha256']==sha(epoch/'run-state.json')and s['helper_sha256']==q['helper_sha256']
    assert 'checks=11895 unknown=11892 outputs=2969 physical=844'in(xz/'test.log').read_text()
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'native-operand-buffer-{mode}-{engine}-{replay_version}-20261025';s,row=observed(d)
            assert s['operand_proof_state_sha256']==sha(proof/'run-state.json')and s['original_assertions_and_reference_unchanged']
            assert s['native_cells_j19_single_SRAM_and_commit_barriers_retained']and sha(d/'adapters/dorado_native_operand_buffer.sv')==q['helper_sha256']
            assert(d/'candidate.sv').read_text()==control((parent/CONTROL).read_text())
            if s['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:assert s['status']=='PASS_PREPARED_OPERAND_COMPLETE_NATIVE_CALLER'
            replays.append(row)
    bp=p['native_backplane_binding_source'];qsf=(parent/'core/Dorado.qsf').read_text()
    for n in [bp,CONTROL,CALLER]:assert str(Path(p['project'])/n)in qsf,n
    assert ".use_pair(1'b0)"in(parent/bp).read_text()
    changed={CONTROL:control((parent/CONTROL).read_text()).encode(),CALLER:caller((parent/CALLER).read_text()).encode(),
             bp:backplane((parent/bp).read_text()).encode(),'src/'+HELPER:(ROOT/HELPER).read_bytes()}
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output.update(changed)
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    output['core/Dorado.qsf']+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/('src/'+HELPER))+'\n').encode()
    out.mkdir(parents=True)
    for n,raw in output.items():
        d=out/n;d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(raw)
    for n in [Path(__file__).name,'integrate_native_operand_buffer.py','build_r_front_ready_native.py']:shutil.copy2(ROOT/'tools'/n,out/n)
    result={**p,'scope':__doc__,'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),'source_hashes':{n:sha(out/n)for n in output},
        'extra_RTL_sources':sorted(set(p['extra_RTL_sources'])|{'src/'+HELPER}),
        'prepared_native_operand_view_connected':True,'operand_view_epoch_proof':str(epoch),
        'operand_view_epoch_state_sha256':sha(epoch/'run-state.json'),'operand_buffer_proof':str(proof),
        'operand_buffer_proof_state_sha256':sha(proof/'run-state.json'),'operand_view_bits_registered':88,
        'operand_view_tag_support_bits':q['tag_support_bits'],
        'operand_view_descriptor_bits':1264,'actual_native_physical_view_uses_prepared_data':True,
        'operand_buffer_XZ_state_sha256':sha(xz/'run-state.json'),'operand_buffer_XZ_negative_state_sha256':sha(negative/'run-state.json'),
        'operand_buffer_replays':replays,'pending_operand_buffer_Icarus_peers':pending,
        'incremental_functional_HDL_changes':sorted(changed),'functional_HDL_changes':sorted(set(p['functional_HDL_changes'])|set(changed)),
        'full60ns_event_batch_connected':False,'assembly_permitted':False,'physical_timing_qualified':False,
        'hardware_speedup_qualified':False,'firmware_qualified':False,'full_goal_active':True}
    assert len(result['source_hashes'])==578
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_ACTUAL_PREPARED_NATIVE_OPERAND_VIEW',len(output),'inputs; pending peers',len(pending),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','epoch','proof','xz','negative','output','project']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--replay-version',default='v2',choices=['v2'])
    a=p.parse_args();prepare(a.parent.resolve(),a.epoch.resolve(),a.proof.resolve(),a.xz.resolve(),a.negative.resolve(),a.output.resolve(),a.project,a.replay_version)
