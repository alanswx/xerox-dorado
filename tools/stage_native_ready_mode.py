#!/usr/bin/env python3
"""Stage the source-proved Ready comparison without changing capture history.

Only the native single-step comparison and one combinational helper change.
Every existing caller, sequential process, clock, barrier and storage source
remains byte-identical. This is a timing experiment, not a selected FPGA core.
"""
import argparse
import json
from pathlib import Path
import shutil

from prepare_ready_mode_compare import native_transform, transform
from prove_cp_prefix_cache import module
from qualify_native_ready_compare import CONTROL, READY, SINGLE
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

TARGET='src/verilog/rtl/dorado_ready_target_compare.sv'


def prepare(parent,component,proof,native,tests,witness,out,project):
    assert not out.exists() and project.is_absolute()
    manifest=parent/'r-front-control-native.json'
    m=json.loads(manifest.read_text());verify(parent,m['source_hashes'])
    assert len(m['source_hashes'])==581 and m['prepared_MemC_Hold_staged']
    assert m['clock_enable_conversion_staged'] and not m['assembly_permitted']
    c=json.loads((component/'results.json').read_text());verify(component,c['files'])
    assert c['stage_manifest_sha256']==sha(manifest)
    assert c['original_sparse_Ready_identity_source_verified']
    p=json.loads((proof/'run-state.json').read_text())
    n=json.loads((native/'run-state.json').read_text())
    t=json.loads((tests/'run-state.json').read_text())
    w=json.loads((witness/'run-state.json').read_text())
    assert p['status']=='PASS_SOURCE_BOUND_READY_MODE_AND_COMPARISON'
    assert n['status']=='PASS_COMPLETE_NATIVE_READY_MODE_COMPARISON'
    assert t['status']=='PASS_DUAL_ENGINE_READY_MODE_COMPARISON_AND_XZ_FALLBACK'
    assert w['status']=='PASS_DUAL_ENGINE_READY_SPARSE_WITNESS_AND_FIX'
    assert all(a['passed'] for s in [p,n,t,w] for a in s['actions'])
    assert p['assumptions']==n['assumptions']==0
    assert n['arbitrary_binary_input_bits']==1982 and n['compared_public_outputs']==2970
    assert n['stage_manifest_sha256']==w['corrected_stage_manifest_sha256']==sha(manifest)
    assert p['source_receipt_sha256']==n['component_receipt_sha256']==t['source_receipt_sha256']==sha(component/'results.json')
    assert n['component_proof_sha256']==sha(proof/'run-state.json')
    for directory,state in [(proof,p),(native,n)]:
        for a in state['actions']:
            verify(directory/a['label'],a['files'])
            assert sha(directory/a['label']/'proof.log')==a['log_sha256']
    for i,a in enumerate(t['actions']):
        d=tests/(a['engine']+'-'+str(i));verify(d,a['files'])
    before,body,_=transform((parent/CONTROL).read_text())
    assert body==(component/'mode-body.sv').read_text()
    assert before==(component/'original-body.sv').read_text()
    candidate=native_transform((parent/CONTROL).read_text(),(parent/READY).read_text())
    assert candidate==(component/'native-control-after.txt').read_text()==(native/'native-control-after.txt').read_text()
    positive=native/'complete-native-interface'
    assert module(candidate,SINGLE)==(positive/(SINGLE+'.sv')).read_text()
    extra=[('dorado_ready_mode_body','mode-body.sv'),('dorado_ready_target_compare','compare.sv')]
    if(component/'binary-compare.sv').exists():extra.append(('dorado_ready_compare_binary_step','binary-compare.sv'))
    for mod,file in extra:
        assert module((component/file).read_text(),mod)==(positive/(mod+'.sv')).read_text()
    out.mkdir(parents=True)
    for name in m['source_hashes']:
        raw=(parent/name).read_bytes()
        if Path(name).suffix in ['.qsf','.qpf','.qip','.tcl','.sdc']:
            raw=raw.replace(m['project'].encode(),str(project).encode())
        if name==CONTROL:raw=candidate.encode()
        if name=='core/Dorado.qsf':
            raw+=('\n# Source-proved parallel Ready target comparison; no capture/clock changes.\nset_global_assignment -name SYSTEMVERILOG_FILE "'+str(project/TARGET)+'"\n').encode()
        path=out/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    helper=out/TARGET;helper.parent.mkdir(parents=True,exist_ok=True)
    helper.write_text(body+'\n'+(component/'compare.sv').read_text()+
        ('\n'+(component/'binary-compare.sv').read_text()if(component/'binary-compare.sv').exists()else''))
    changed={n for n in m['source_hashes']if(out/n).read_bytes().replace(str(project).encode(),m['project'].encode())!=(parent/n).read_bytes()}
    assert changed=={CONTROL,'core/Dorado.qsf'},changed
    receipts={key:dict(directory=str(d),state_sha256=sha(d/f))for key,d,f in [
        ('component',component,'results.json'),('proof',proof,'run-state.json'),('native',native,'run-state.json'),
        ('tests',tests,'run-state.json'),('witness',witness,'run-state.json')]}
    # Preserve inherited historical lists separately from the actual cumulative
    # HDL union, including the prepared-Hold helper omitted by its old manifest.
    hdl=set(m['functional_HDL_changes'])|{'src/verilog/rtl/dorado_memc_hold_buffer.sv',TARGET}
    assert len(hdl)==51
    record={**m,'project':str(project),'parent_stage':str(parent),'parent_manifest_sha256':sha(manifest),
        'scope':__doc__,'parallel_Ready_target_comparison_staged':True,
        'parallel_Ready_target_comparison_selected_or_speed_qualified':False,
        'parallel_Ready_target_receipts':receipts,
        'functional_HDL_changes':sorted(hdl),'incremental_functional_HDL_changes':[CONTROL,TARGET],
        'additional_native_lint_sources':[*m['additional_native_lint_sources'],TARGET],
        'source_hashes':{n:sha(out/n)for n in [*m['source_hashes'],TARGET]},
        'additional_host_or_guest_cycles':0,'added_sequential_processes':0,
        'guest_clock_schedule_changed':False,'original_readiness_barriers_retained':True,
        'assembly_permitted':False}
    (out/'r-front-control-native.json').write_text(json.dumps(record,indent=2)+'\n')
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    shutil.copy2(parent/'build_r_front_ready_native.py',out/'build_r_front_ready_native.py')
    print('STAGED_NATIVE_PARALLEL_READY_TARGET',len(record['source_hashes']),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['parent','component','proof','native','tests','witness','output','project']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();prepare(*[getattr(a,n).resolve()for n in ['parent','component','proof','native','tests','witness','output']],a.project)
