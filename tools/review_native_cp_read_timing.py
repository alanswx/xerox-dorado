#!/usr/bin/env python3
"""Collect and review the SAME full-core15ns timing diagnostic, without assembly.

Source readback and all four operating corners are retained. A successful
fitter/STA invocation is not a timing pass. No machine state, media, build
inputs or remote job are changed by this review.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import re
import subprocess
import tarfile

from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
PROJECT='/home/alans/mister/Dorado_CPRead15_Diag_v2_20261025'
GUARD_PROJECT='/home/alans/mister/Dorado_CPRead15_PhaseGuard85_20261025'
TASK_PROJECT='/home/alans/mister/Dorado_CPRead15_TaskTerminal_20261025'
SHARED_PROJECT='/home/alans/mister/Dorado_CPRead15_SharedTask_20261025'
PARALLEL_PROJECT='/home/alans/mister/Dorado_CPRead15_TaskParallel_20261025'
READY_PROJECT='/home/alans/mister/Dorado_CPRead15_ReadyCompare_20261025'

QUERY=r'''from pathlib import Path
import hashlib,io,json,sys,tarfile,time
p=Path(PROJECT)
state=json.loads((p/'run-state.json').read_text())
assert state['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED',state
assert state['builder_pid']==BUILDER_PID and not state['assembly_permitted']
assert len(state['stages'])==9 and all(s['returncode']==0 for s in state['stages'])
manifest=json.loads((p/'r-front-control-native.json').read_text())
actual={n:hashlib.sha256((p/n).read_bytes()).hexdigest()for n in manifest['source_hashes']}
assert actual==manifest['source_hashes'] and len(actual)==SOURCE_COUNT
assert not list((p/'core/output_files').glob('*.sof'))
assert not list((p/'core/output_files').glob('*.rbf'))
files=[f for f in p.glob('*.log')]
files += [p/n for n in ['run-state.json','timing-observation.json','r-front-control-native.json','build_r_front_ready_native.py']]
files += [f for f in (p/'core').glob('*.rpt')if not f.name.startswith('._')]
files += [f for f in (p/'core/output_files').iterdir()if f.suffix in ['.rpt','.summary','.smsg','.pin']and not f.name.startswith('._')]
files += [f for f in (p/'core').iterdir()if f.suffix in ['.tcl','.sdc','.qsf','.qpf']and not f.name.startswith('._')]
files=sorted(set(files))
hashes={str(f.relative_to(p)):hashlib.sha256(f.read_bytes()).hexdigest()for f in files}
observation={'observed':time.time(),'project':str(p),'run_state':state,'source_readback':actual,'collected_hashes':hashes,'assembly_or_hardware_changed':False}
blob=(json.dumps(observation,indent=2)+'\n').encode()
with tarfile.open(fileobj=sys.stdout.buffer,mode='w|gz')as archive:
 for f in files:archive.add(f,arcname=str(f.relative_to(p)),recursive=False)
 info=tarfile.TarInfo('collection.json');info.size=len(blob);archive.addfile(info,io.BytesIO(blob))
'''


def paths(report):
    rows=[]
    for block in re.split(r'(?m)^Path #\d+:',report)[1:]:
        properties={k.strip():v.strip()for k,v in re.findall(
            r'^;\s*(Slack|From Node|To Node|Launch Clock|Latch Clock|Data Arrival Time|Data Required Time)\s*;\s*([^;]+);',block,re.M)}
        if properties:
            properties['slack_ns']=float(properties['Slack'].split()[0])
            rows.append(properties)
    return rows


def review(stage,out,launch=None):
    assert not out.exists();out.mkdir(parents=True)
    source=json.loads((stage/'r-front-control-native.json').read_text())
    project=source['project']
    native_single=source.get('actual_native_single_comparison_connected',False)
    registered=source.get('registered_R_read_buffer_connected',False)
    memc=source.get('registered_MemC_tag_reads_connected',False)
    operand=source.get('prepared_native_operand_view_connected',False)
    clock_enable=source.get('clock_enable_conversion_staged',False)
    new_ready=bool(native_single and source.get('ready_compare_proof_state_sha256'))
    launch_state=None
    if launch:
        launch_state=json.loads((launch/'launch-observation.json').read_text())
        assert launch_state['project']==project and launch_state['manifest_sha256']==sha(stage/'r-front-control-native.json')
        assert launch_state['verified_source_count']==len(source['source_hashes'])
        assert not launch_state['assembly_permitted']
        if clock_enable:
            preflight=launch_state['clock_enable_diagnostic_preflight']
            assert preflight['diagnostic_fit_only']and not preflight['complete_functional_and_timing_qualification']
            assert len(source['converted_module_types'])==33 and source['converted_unique_processes']==58
            assert source['converted_virtual_process_instances']==1823 and source['host_clock_processes_unchanged']
    assert (project in [PROJECT,GUARD_PROJECT,TASK_PROJECT,SHARED_PROJECT,PARALLEL_PROJECT,READY_PROJECT] or (new_ready and launch_state)) and source['frequency_hz']==66666667 and source['sysper']==4
    guarded=project!=PROJECT
    task=project in [TASK_PROJECT,SHARED_PROJECT,PARALLEL_PROJECT,READY_PROJECT] or new_ready
    shared=project in [SHARED_PROJECT,PARALLEL_PROJECT,READY_PROJECT] or new_ready
    parallel=project in [PARALLEL_PROJECT,READY_PROJECT] or new_ready
    ready=project==READY_PROJECT or new_ready
    if guarded:
        assert source['status']=='PREPARED_FULL_NATIVE_15NS_PHASE_GUARD_DIAGNOSTIC'
        assert source['phase_guard_new_pins']==85 and source['all795_control_and_shared_frame_bits_proved']
    if task:assert source['task_terminal_proof_state_sha256']and source['sparse_shared_capture_frames']
    if shared:assert source['processor_terminal_proof_state_sha256']and source['address_task_owner_proof_state_sha256']and len(source['functional_HDL_changes'])==(47 if clock_enable else 13 if operand else 12 if memc else 9 if registered else 6 if ready else 4)
    if parallel:
        assert source['task_parallel_proof_state_sha256']
        if not ready:assert source['incremental_functional_HDL_changes']==['src/verilog/rtl/dorado_local_task_capture_step.sv']
    if ready:assert source['ready_compare_proof_state_sha256']and source['all2969_prefix_outputs_retained']and source['shared_state_mismatch_guard_retained']and len(source['incremental_functional_HDL_changes'])==(35 if clock_enable else 4 if operand else 3)
    builder=launch_state['builder_pid'] if launch_state else {PROJECT:2865831,GUARD_PROJECT:3301754,TASK_PROJECT:3440305,SHARED_PROJECT:3571235,PARALLEL_PROJECT:3827091,READY_PROJECT:4042975}[project]
    if registered:assert source['R_read_buffer_proof_state_sha256']and source['single_SRAM_owner_and_write_edge_unchanged']
    if memc:assert registered and source['MemC_tag_read_proof_state_sha256']and len(source['MemC_tag_read_packages'])==21 and source['all21_address_matches_gate_whole_machine_publication']
    if operand:assert memc and source['operand_buffer_proof_state_sha256']and source['operand_view_bits_registered']==88 and source['operand_view_tag_support_bits']==909
    query=QUERY.replace('Path(PROJECT)','Path('+repr(project)+')',1).replace('BUILDER_PID',str(builder)).replace('SOURCE_COUNT','578'if operand else'577'if memc else'576'if registered else'575'if ready else'574')
    (out/'collection-query.py').write_text(query)
    bundle=out/'terminal.tgz'
    with bundle.open('xb')as f:
        p=subprocess.run(['ssh','-o','BatchMode=yes','-o','ConnectTimeout=10','misterubuntu','python3','-'],
                         input=query.encode(),stdout=f,stderr=subprocess.PIPE)
    (out/'collection.stderr').write_bytes(p.stderr);assert p.returncode==0,p.stderr.decode()
    terminal=out/'terminal';terminal.mkdir()
    with tarfile.open(bundle)as archive:
        for member in archive.getmembers():
            assert member.isfile()and not Path(member.name).is_absolute()and'..'not in Path(member.name).parts
        archive.extractall(terminal,filter='data')
    observation=json.loads((terminal/'collection.json').read_text())
    assert observation['source_readback']==source['source_hashes']
    for n,h in source['source_hashes'].items():assert sha(stage/n)==h,n
    for n,h in observation['collected_hashes'].items():assert sha(terminal/n)==h,n
    summary=(terminal/'core/output_files/Dorado.sta.summary').read_text()
    checks=re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',summary)
    assert checks
    corners={}
    for f in sorted((terminal/'core').glob('native59-setup-*.rpt')):
        rows=paths(f.read_text());assert len(rows)==3
        corners[f.name.removeprefix('native59-setup-').removesuffix('.rpt')]=rows
    assert len(corners)==4
    worst=min((row for rows in corners.values()for row in rows),key=lambda r:r['slack_ns'])
    if not task:
        expected_source='cell_MC1668_snapshot:u_a15|qa'if guarded else'r_front_lower_q[48]'
        assert expected_source in worst['From Node']and worst['To Node'].endswith('|network_step')
        assert '(INVERTED)'in worst['Latch Clock']
    full=next(f for f in(terminal/'core').glob('native59-setup-*.rpt')if worst in paths(f.read_text())).read_text()
    if not task:assert '; 7.500' in full and 'direct_arrival' in full and 'unsettled' in full
    fit=(terminal/'core/output_files/Dorado.fit.summary').read_text()
    assert 'Fitter Status : Successful'in fit
    rejected=min(float(x)for _,x in checks)<0 or worst['slack_ns']<0
    result=dict(status='REVIEWED_FULL_CORE_15NS_TIMING_REJECTION'if rejected else'REVIEWED_FULL_CORE_15NS_TIMING_REPORTS',scope=__doc__,
        project=project,source_files_verified=len(source['source_hashes']),terminal_state=observation['run_state'],
        nominal_ordinary_ns=120,target_ordinary_ns=60,frequency_hz=66666667,sysper=4,
        all_summary_checks=checks,setup_paths_by_corner=corners,worst_setup_path=worst,
        critical_path=(('Prepared native operand-view candidate: 'if operand else'MemC registered-read candidate: 'if memc else'RM/STK registered-read candidate: 'if registered else'Ready-comparison candidate: 'if ready else'Parallel-task candidate: 'if parallel else'Shared-task-owner candidate: 'if shared else'Task-terminal candidate: ')+'inspect preserved full path; '+worst['From Node']+' -> '+worst['To Node']if task else
                      'MemC a15 address -> asynchronous cache-tag b20 -> miss/Hold pin386 -> shared control frames -> processor T-path settling -> machine_enable -> falling-edge network_step'if guarded else
                       'R address -> asynchronous RM read -> ContA/ContB/direct capture -> event settling check -> machine_enable -> falling-edge network_step'),
        phase_guard_only=guarded and not task,task_terminal_and_sparse_frames=task,
        shared_address_task_owner=shared,
        parallel_task_capture=parallel,
        parallel_ready_comparison=ready,
        registered_R_read_buffer=registered,
        registered_MemC_tag_reads=memc,
        prepared_native_operand_view=operand,
        common_clock_guest_enables=clock_enable,
        clock_enable_diagnostic_preflight=launch_state.get('clock_enable_diagnostic_preflight')if launch_state else None,
        actual_native_single_comparison_connected=source.get('actual_native_single_comparison_connected',False),
        ready_v1_targeting_limitation='Paired prefix only; ordinary native single comparison unchanged'if ready and not source.get('actual_native_single_comparison_connected')else None,
        all_summary_slacks_nonnegative=all(float(x)>=0 for _,x in checks),
        fit_summary=fit,exact_stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        collection_sha256=sha(terminal/'collection.json'),bundle_sha256=sha(bundle),
        full_core_timing_qualified=False,SDRAM_protocol_or_physical_timing_qualified=False,
        assembly_permitted=False,hardware_changed=False,measured_speed_gain=False,full_goal_active=True,
        next_action=('Inspect the actual common-clock critical paths and all native clock/SDRAM constraints; finish complete native equivalence before assembly. Then split chronological producers within original60ns phases.'if clock_enable else
                     'Replace the deep iterative capture/settle dependency with qualified chronological producer phases; preserve the machine-edge storage barrier and network/machine edge alignment'))
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stage',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--launch',type=Path,help='Verified launch receipt for a new source-bound candidate')
    p.add_argument('--archive',type=Path)
    a=p.parse_args();r=review(a.stage.resolve(),a.output.resolve(),a.launch.resolve()if a.launch else None)
    if a.archive:append_archive(a.archive.resolve(),a.output.resolve(),[],label='full-core15-timing',extra_tools=[Path(__file__).name])
    print(r['status'],r['worst_setup_path']['slack_ns'])
