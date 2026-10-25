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


def start(stage,out,clock_enable_audit=None,baseboard=None,baseboard_negative=None):
    assert not out.exists();out.mkdir(parents=True)
    r=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,r['source_hashes'])
    assert r['status']=='PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC'and not r['assembly_permitted']
    project=r['project'];assert Path(project).parent==Path('/home/alans/mister')
    registered=r.get('registered_R_read_buffer_connected',False)
    if registered:assert r['R_read_buffer_proof_state_sha256']and r['single_SRAM_owner_and_write_edge_unchanged']and r['actual_native_single_comparison_connected']
    memc=r.get('registered_MemC_tag_reads_connected',False)
    if memc:assert registered and r['MemC_tag_read_proof_state_sha256']and len(r['MemC_tag_read_packages'])==21 and r['all21_address_matches_gate_whole_machine_publication']
    operand=r.get('prepared_native_operand_view_connected',False)
    if operand:assert memc and r['operand_buffer_proof_state_sha256']and r['operand_view_bits_registered']==88 and r['operand_view_tag_support_bits']==909
    source_count=578 if operand else 577 if memc else 576 if registered else 575 if r.get('ready_compare_proof_state_sha256')else 574
    assert len(r['source_hashes'])==source_count
    ce_preflight=None
    if r.get('clock_enable_conversion_staged'):
        assert clock_enable_audit and baseboard and baseboard_negative
        a=json.loads((clock_enable_audit/'results.json').read_text())
        assert a['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'and a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
        assert a['process_counts']['removed_guards']==1823 and a['process_counts']['HOST_CLOCK_RETAINS_EXISTING_POLICY']==1975
        for d,status in [(baseboard,'PASS_ORIGINAL_ROM_BASEBOARD_COMMON_CLOCK_EQUIVALENCE'),(baseboard_negative,'PASS_MISSING_CLOCK_ENABLE_COUNTEREXAMPLE')]:
            b=json.loads((d/'run-state.json').read_text());verify(d,b['files'])
            assert b['status']==status and all(x.get('returncode')==0 for x in b['actions']if x['label']=='compile')
            tested=Path(b['stage']);bm=json.loads((tested/'r-front-control-native.json').read_text());verify(tested,bm['source_hashes'])
            assert sha(tested/'r-front-control-native.json')==b['stage_manifest_sha256']
            assert {n:h for n,h in bm['source_hashes'].items()if Path(n).suffix in ['.v','.sv']}=={n:h for n,h in r['source_hashes'].items()if Path(n).suffix in ['.v','.sv']}
        ce_preflight=dict(structural_audit_sha256=sha(clock_enable_audit/'results.json'),
            baseboard_test_state_sha256=sha(baseboard/'run-state.json'),negative_state_sha256=sha(baseboard_negative/'run-state.json'),
            complete_functional_and_timing_qualification=False,diagnostic_fit_only=True)
    check='from pathlib import Path\np=Path('+repr(project)+')\nassert not p.exists(), "Observe existing project; do not restart"\n'
    subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],input=check,text=True,check=True)
    manifest_sha=sha(stage/'r-front-control-native.json')
    stagers=[n for n in ['stage_native_phase_guard_timing.py','stage_native_task_parallel.py','stage_native_ready_compare.py','stage_native_r_read_buffer.py','integrate_r_read_buffer.py','stage_native_memc_read_buffer.py','qualify_memc_read_buffer.py','stage_native_operand_buffer.py','integrate_native_operand_buffer.py','stage_native_clock_enables.py']if(stage/n).is_file()]
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
    for n in ['clock-enable-audit','baseboard','baseboard-negative']:p.add_argument('--'+n,type=Path)
    a=p.parse_args();start(a.stage.resolve(),a.output.resolve(),a.clock_enable_audit,a.baseboard,a.baseboard_negative)
