#!/usr/bin/env python3
"""Start one new isolated full-core phase-guard timing job on misterubuntu.

Refuse an existing project: later calls must observe its saved process handles.
Upload exactly the pinned inputs, verify them before launch and never assemble
or program hardware. The previous full-core fit and native simulations are not
changed. A transport/observation timeout is not a failed remote build.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import tarfile

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def start(stage,out,clock_enable_audit=None,baseboard=None,baseboard_negative=None,publication_grant_proof=None):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,r['source_hashes'])
    assert r['status']=='PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC'and not r['assembly_permitted']
    project=r['project'];assert Path(project).parent==Path('/home/alans/mister')
    registered=r.get('registered_R_read_buffer_connected',False)
    if registered:assert r['R_read_buffer_proof_state_sha256']and r['single_SRAM_owner_and_write_edge_unchanged']and r['actual_native_single_comparison_connected']
    memc=r.get('registered_MemC_tag_reads_connected',False)
    if memc:assert registered and r['MemC_tag_read_proof_state_sha256']and len(r['MemC_tag_read_packages'])==21 and r['all21_address_matches_gate_whole_machine_publication']
    operand=r.get('prepared_native_operand_view_connected',False)
    compact=r.get('compact_operand_tag_qualified',False)
    if operand:assert memc and r['operand_buffer_proof_state_sha256']and r['operand_view_bits_registered']==88 and r['operand_view_tag_support_bits']==(465 if compact else 909)
    compact_proof=None
    if compact:
        pd=Path(r['operand_buffer_proof']);compact_proof=json.loads((pd/'run-state.json').read_text());verify(pd,compact_proof['files'])
        assert compact_proof['status']=='PASS_ACTUAL_NATIVE_PREPARED_OPERAND_VIEW'and compact_proof['tag_support_bits']==465
        assert sha(pd/'run-state.json')==r['operand_buffer_proof_state_sha256']
    riot=r.get('native_RIOT_block_ram_staged',False)
    source_count=580 if riot else 578 if operand else 577 if memc else 576 if registered else 575 if r.get('ready_compare_proof_state_sha256')else 574
    assert len(r['source_hashes'])==source_count
    ce_preflight=None
    grant=r.get('shared_publication_grant_staged',False)
    grant_proof=None
    comparison_sources=r['source_hashes']
    if riot:
        assert compact and not grant and r['RIOT_block_ram_original_cells']==5
        from prepare_riot_blockram import candidate,RIOT
        from stage_native_riot_blockram import CELL,MODULE,RAM
        from prove_cp_prefix_cache import module
        parent=Path(r['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
        assert sha(parent/'r-front-control-native.json')==r['parent_manifest_sha256']
        receipts={}
        for name,receipt in r['RIOT_block_ram_receipts'].items():
            d=Path(receipt['directory']);state=d/('results.json'if name in ['component','mapping']else'run-state.json')
            assert sha(state)==receipt['state_sha256']
            value=json.loads(state.read_text());verify(d,value['files']);receipts[name]=value
        assert receipts['proof']['status']=='PASS_RESET_HISTORY_RIOT_RAM_INDUCTION'and receipts['proof']['assumptions']==0
        assert receipts['tests']['status']=='PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT'
        assert receipts['baseboard']['status']=='PASS_ORIGINAL_ROM_BASEBOARD_RIOT_BLOCK_RAM'
        bb=Path(r['RIOT_block_ram_receipts']['baseboard']['directory'])
        assert sha(stage/CELL)==sha(bb/'cell_MCS6532.v')
        assert sha(stage/MODULE)==sha(bb/'candidate.sv')and sha(stage/RAM)==sha(bb/'ram.sv')==receipts['proof']['helper_sha256']
        original=(parent/CELL).read_text();old='M6532_ce u_riot (.guest_advance(guest_advance),';new='dorado_riot_blockram_ce u_riot (.guest_advance(guest_advance),'
        assert original.count(old)==1 and (stage/CELL).read_text()==original.replace(old,new,1)
        vendor=(parent/RIOT).read_text()
        assert (stage/MODULE).read_text()==vendor[:vendor.index('module M6532')]+candidate(module(vendor,'M6532_ce'))
        assert set(r['source_hashes'])-set(pm['source_hashes'])=={MODULE,RAM}
        assert {n for n,h in pm['source_hashes'].items()if Path(n).suffix in ['.v','.sv']and r['source_hashes'][n]!=h}=={CELL}
        comparison_sources=pm['source_hashes']
    if grant:
        assert publication_grant_proof
        grant_proof=json.loads((publication_grant_proof/'run-state.json').read_text())
        verify(publication_grant_proof,grant_proof['files'])
        assert grant_proof['status']=='PASS_NATIVE_SHARED_PUBLICATION_GRANT'
        assert grant_proof['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
        assert grant_proof['parent_manifest_sha256']==r['parent_manifest_sha256']
        from stage_native_publication_grant import CALLER,BACKPLANE,binding,caller,backplane
        binding(stage)
        parent=Path(r['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
        assert sha(parent/'r-front-control-native.json')==r['parent_manifest_sha256']
        assert (stage/CALLER).read_text()==caller((parent/CALLER).read_text())
        assert (stage/BACKPLANE).read_text()==backplane((parent/BACKPLANE).read_text())
        assert {n for n,h in pm['source_hashes'].items()if Path(n).suffix in ['.v','.sv']and r['source_hashes'][n]!=h}=={CALLER,BACKPLANE}
        comparison_sources=pm['source_hashes']
    if r.get('clock_enable_conversion_staged'):
        assert clock_enable_audit and baseboard and baseboard_negative
        a=json.loads((clock_enable_audit/'results.json').read_text())
        expected_audit='PASS_NATIVE_RIOT_BLOCK_RAM_STRUCTURAL_AUDIT'if riot else'PASS_NATIVE_SHARED_GRANT_STRUCTURAL_AUDIT'if grant else'PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'
        assert a['status']==expected_audit and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
        counts=a['parent_process_counts']if grant or riot else a['process_counts']
        assert counts['removed_guards']==1823 and counts['HOST_CLOCK_RETAINS_EXISTING_POLICY']==1975
        if grant:assert a['proof_state_sha256']==sha(publication_grant_proof/'run-state.json')and a['actual_caller_parameter']==1 and len(a['proved_guard_changes'])==1
        if riot:assert a['instances_after']==12787 and a['process_counts']['new_processes']==3813 and a['all_original_chip_input_wiring_retained']and a['all_new_processes_same_clock_and_advance']
        for d,status in [(baseboard,'PASS_ORIGINAL_ROM_BASEBOARD_COMMON_CLOCK_EQUIVALENCE'),(baseboard_negative,'PASS_MISSING_CLOCK_ENABLE_COUNTEREXAMPLE')]:
            b=json.loads((d/'run-state.json').read_text());verify(d,b['files'])
            assert b['status']==status and all(x.get('returncode')==0 for x in b['actions']if x['label']=='compile')
            tested=Path(b['stage']);bm=json.loads((tested/'r-front-control-native.json').read_text());verify(tested,bm['source_hashes'])
            assert sha(tested/'r-front-control-native.json')==b['stage_manifest_sha256']
            differences={n for n,h in bm['source_hashes'].items()if Path(n).suffix in ['.v','.sv']and comparison_sources.get(n)!=h}
            if compact:
                helper='src/verilog/rtl/dorado_native_operand_buffer.sv'
                assert differences=={helper}
                assert bm['source_hashes'][helper]==compact_proof['original_helper_sha256']and r['source_hashes'][helper]==compact_proof['helper_sha256']
            else:assert not differences
        ce_preflight=dict(structural_audit_sha256=sha(clock_enable_audit/'results.json'),
            baseboard_test_state_sha256=sha(baseboard/'run-state.json'),negative_state_sha256=sha(baseboard_negative/'run-state.json'),
            complete_functional_and_timing_qualification=False,diagnostic_fit_only=True)
        if grant:ce_preflight['shared_publication_grant_proof_sha256']=sha(publication_grant_proof/'run-state.json')
        if riot:ce_preflight['RIOT_block_ram_receipts']=r['RIOT_block_ram_receipts']
    check='from pathlib import Path\np=Path('+repr(project)+')\nassert not p.exists(), "Observe existing project; do not restart"\n'
    subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=check,text=True,check=True)
    manifest_sha=sha(stage/'r-front-control-native.json')
    stagers=[n for n in ['stage_native_phase_guard_timing.py','stage_native_task_parallel.py','stage_native_ready_compare.py','stage_native_r_read_buffer.py','integrate_r_read_buffer.py','stage_native_memc_read_buffer.py','qualify_memc_read_buffer.py','stage_native_operand_buffer.py','integrate_native_operand_buffer.py','stage_native_clock_enables.py','stage_compact_native_operand_buffer.py','stage_native_publication_grant.py','stage_native_riot_blockram.py']if(stage/n).is_file()]
    assert stagers,'Missing the executed staging helper'
    names=sorted(r['source_hashes'])+['r-front-control-native.json','build_r_front_ready_native.py',*stagers]
    bundle=out/'stage.tgz'
    with tarfile.open(bundle,'w:gz')as archive:
        for n in names:archive.add(stage/n,arcname=n,recursive=False)
    remote_bundle='/home/alans/mister/codex-phase-guard85-'+manifest_sha[:16]+'.tgz'
    subprocess.run(['scp','-q',str(bundle),'misterubuntu:'+remote_bundle],check=True)
    script='''from pathlib import Path
import hashlib,json,subprocess,tarfile,time
p=Path(PROJECT);bundle=Path(BUNDLE)
assert not p.exists(), 'Observe existing project; do not restart'
assert hashlib.sha256(bundle.read_bytes()).hexdigest()==BUNDLE_SHA
p.mkdir()
with tarfile.open(bundle)as archive:
 for member in archive.getmembers():
  assert member.isfile()and not Path(member.name).is_absolute()and '..'not in Path(member.name).parts
 archive.extractall(p,filter='data')
manifest=p/'r-front-control-native.json'
assert hashlib.sha256(manifest.read_bytes()).hexdigest()==MANIFEST_SHA
r=json.loads(manifest.read_text());assert r['project']==str(p)and not r['assembly_permitted']
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in r['source_hashes']}
assert actual==r['source_hashes']and len(actual)==SOURCE_COUNT
assert not(p/'run-state.json').exists()
with(p/'builder.log').open('xb')as log:
 child=subprocess.Popen(['python3',str(p/'build_r_front_ready_native.py'),'--project',str(p),'--manifest','r-front-control-native.json'],
                        cwd=p,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
for unused in range(50):
 if(p/'run-state.json').exists():break
 time.sleep(0.1)
process=subprocess.run(['ps','-p',str(child.pid),'-o','pid=,etime=,time=,stat=,args='],capture_output=True,text=True)
assert process.returncode==0,process.stdout
result={'project':str(p),'builder_pid':child.pid,'builder_ps':process.stdout.strip(),
        'verified_source_count':len(actual),'manifest_sha256':MANIFEST_SHA,'bundle_sha256':BUNDLE_SHA,
        'observed':time.time(),'assembly_permitted':False,
        'run_state':json.loads((p/'run-state.json').read_text())if(p/'run-state.json').exists()else None}
print(json.dumps(result))
'''
    replacements={'PROJECT':repr(project),'BUNDLE':repr(remote_bundle),'BUNDLE_SHA':repr(sha(bundle)),'MANIFEST_SHA':repr(manifest_sha),'SOURCE_COUNT':str(source_count)}
    script=re.sub(r'\b(?:PROJECT|BUNDLE_SHA|BUNDLE|MANIFEST_SHA|SOURCE_COUNT)\b',lambda m:replacements[m[0]],script)
    (out/'launch.py').write_text(script)
    launch=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=script,text=True,capture_output=True)
    (out/'launch.stdout').write_text(launch.stdout);(out/'launch.stderr').write_text(launch.stderr)
    assert launch.returncode==0,(launch.stdout,launch.stderr)
    result=json.loads(launch.stdout);assert result['verified_source_count']==source_count
    if ce_preflight:result['clock_enable_diagnostic_preflight']=ce_preflight
    (out/'launch-observation.json').write_text(json.dumps(result,indent=2)+'\n')
    print('NEW_FULL_CORE_PHASE_GUARD_TIMING_JOB',result['builder_pid'],result['project'])
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stage',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    for n in ['clock-enable-audit','baseboard','baseboard-negative','publication-grant-proof']:p.add_argument('--'+n,type=Path)
    a=p.parse_args();start(a.stage.resolve(),a.output.resolve(),a.clock_enable_audit,a.baseboard,a.baseboard_negative,a.publication_grant_proof)
