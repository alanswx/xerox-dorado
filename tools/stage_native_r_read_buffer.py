#!/usr/bin/env python3
"""Stage the actual registered RM/STK read boundary in the native full core.

Keep every clock, timer, original cell and publication/storage barrier. Bind
the tested caller and sole SRAM owner to the compiled native backplane before
adding the register. This remains a timing diagnostic, without assembly/load.
"""
import argparse
import json
from pathlib import Path
import shutil

from integrate_r_read_buffer import caller,memory
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1];BUILD=ROOT/'build/rtl-executive'
CALLER='src/verilog/rtl/caller.sv';MEMORY='src/verilog/rtl/dorado_r_file_ports.sv'
BUFFER='src/verilog/rtl/dorado_r_read_buffer.sv'


def prepare(parent,proof,out,project):
    assert not out.exists()and project.parent==Path('/home/alans/mister')
    p=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,p['source_hashes'])
    assert len(p['source_hashes'])==575 and p['frequency_hz']==66666667 and p['sysper']==4
    assert p['actual_native_single_comparison_connected']and not p['assembly_permitted']
    q=json.loads((proof/'run-state.json').read_text());verify(proof,q['files'])
    assert q['status']=='PASS_REGISTERED_R_READ_FRONTIER'and q['assumptions']==0
    assert q['memory_sha256']==sha(parent/MEMORY)and q['buffer_sha256']==sha(ROOT/'verilog/rtl/dorado_r_read_buffer.sv')
    for a in q['actions']:
        d=proof/a['label'];verify(d,a['files'])
        assert a['passed']and a['returncode']==0 and a['log_sha256']==sha(d/'proof.log')
    # Native caller has only these two extra read-only exports over the tested caller.
    raw_caller=(parent/CALLER).read_text()
    a='output wire [35:0] native_R_frame,output wire [400:0] native_before_pins,output wire native_before_root);'
    b='output wire [35:0] native_R_frame);'
    view=' assign native_before_pins=r_front_before;assign native_before_root=r_front_clock_before;\n'
    assert raw_caller.count(a)==raw_caller.count(view)==1
    tested_caller=raw_caller.replace(a,b,1).replace(view,'',1)
    replays=[];pending=[]
    for mode in ['full','words']:
        for engine in ['verilator','icarus']:
            d=BUILD/f'r-read-buffer-{mode}-{engine}-v2-20261025';r,row=observed(d)
            assert r['buffer_proof_state_sha256']==sha(proof/'run-state.json')
            assert r['original_assertions_and_reference_unchanged']and r['native_cells_j19_single_SRAM_and_commit_barriers_retained']
            assert(d/'caller.sv').read_text()==caller(tested_caller)
            assert(d/'inputs/verilog/rtl/dorado_r_file_ports.sv').read_text()==memory((parent/MEMORY).read_text())
            assert sha(d/'adapters/dorado_r_read_buffer.sv')==q['buffer_sha256']
            if r['status']=='RUNNING':
                assert engine=='icarus'and row['verified_live'];pending.append(row)
            else:assert r['status']=='PASS_REGISTERED_READ_COMPLETE_NATIVE_CALLER'
            replays.append(row)
    qsf=(parent/'core/Dorado.qsf').read_text()
    for n in [CALLER,MEMORY,p['native_backplane_binding_source']]:assert str(Path(p['project'])/n)in qsf,n
    assert ".use_pair(1'b0)"in(parent/p['native_backplane_binding_source']).read_text()
    changed={CALLER:caller(raw_caller).encode(),MEMORY:memory((parent/MEMORY).read_text()).encode(),BUFFER:(ROOT/'verilog/rtl/dorado_r_read_buffer.sv').read_bytes()}
    output={n:(parent/n).read_bytes()for n in p['source_hashes']};output.update(changed)
    for n,raw in output.items():
        if Path(n).suffix in ['.qsf','.qip','.qpf','.sdc','.tcl']:output[n]=raw.replace(p['project'].encode(),str(project).encode())
    output['core/Dorado.qsf']+=('\nset_global_assignment -name SYSTEMVERILOG_FILE '+str(project/BUFFER)+'\n').encode()
    out.mkdir(parents=True)
    for n,raw in output.items():
        d=out/n;d.parent.mkdir(parents=True,exist_ok=True);d.write_bytes(raw)
    for n in ['build_r_front_ready_native.py',Path(__file__).name,'integrate_r_read_buffer.py']:shutil.copy2(ROOT/'tools'/n,out/n)
    hashes={n:sha(out/n)for n in output};assert len(hashes)==576
    for n in p['source_hashes']:
        if n not in changed and Path(n).suffix not in ['.qsf','.qip','.qpf','.sdc','.tcl']:assert hashes[n]==p['source_hashes'][n]
        if n.endswith('.sdc'):assert(out/n).read_bytes()==(parent/n).read_bytes().replace(p['project'].encode(),str(project).encode())
    result={**p,'scope':__doc__,'project':str(project),'parent_project':p['project'],'parent_stage':str(parent),
        'parent_manifest_sha256':sha(parent/'r-front-control-native.json'),'source_hashes':hashes,
        'extra_RTL_sources':sorted(set(p['extra_RTL_sources'])|{BUFFER}),
        'registered_R_read_buffer_connected':True,'R_read_buffer_proof':str(proof),
        'R_read_buffer_proof_state_sha256':sha(proof/'run-state.json'),'R_read_buffer_replays':replays,
        'pending_R_read_buffer_Icarus_peers':pending,'single_SRAM_owner_and_write_edge_unchanged':True,
        'before_and_interior_reads_require_matching_registered_response':True,
        'incremental_functional_HDL_changes':sorted(changed),'functional_HDL_changes':sorted(set(p['functional_HDL_changes'])|set(changed)),
        'nominal_ordinary_ns':120,'full60ns_event_batch_connected':False,'no_new_timing_exceptions':True,
        'assembly_permitted':False,'hardware_speedup_qualified':False,'physical_timing_qualified':False,'firmware_qualified':False,'full_goal_active':True}
    (out/'r-front-control-native.json').write_text(json.dumps(result,indent=2)+'\n')
    print('STAGED_NATIVE_REGISTERED_R_READ',len(hashes),'inputs; pending peers',len(pending));return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','proof','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(a.parent.resolve(),a.proof.resolve(),a.output.resolve(),a.project)
