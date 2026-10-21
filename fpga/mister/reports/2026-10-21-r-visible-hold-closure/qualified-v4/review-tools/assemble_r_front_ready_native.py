#!/usr/bin/env python3
"""Assemble an independently reviewed native candidate; never program FPGA.

Require unchanged compiled inputs, completed/absent build and review jobs,
31 timing summaries, 332 legacy or 356 canonical-R targeted reports,
actual-wrapper Initial, and eleven legacy or fifteen canonical-R rejected
evidence controls, plus video-register and applied-margin controls when used.
Observe the same assembly job if output yields.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(project,quartus):
    q=json.loads((project/'preassembly-qualification.json').read_text())
    controls=json.loads((project/'fit-review-controls.json').read_text())
    assert q['status']=='PASS_NATIVE_R_FRONT_READY_PREASSEMBLY_REVIEW'
    assert q['minimum_slack_ns']>0 and q['state_admin']=='INACTIVE'
    assert q['frequency_mhz']==50 and q['sysper']==4
    canonical=q.get('native_visible_R_registers',0)==16
    assert len(q['timing_checks'])==31 and len(q['detailed_paths'])==196 and len(q['central_paths'])==(160 if canonical else 136)
    assert q['initial_component_enabled_samples']>=100000 and not q['firmware_qualified']
    expected_controls=(15 if canonical else 11)+bool(q.get('host_video_register_mapping'))+bool(q.get('additional_hold_uncertainty_ps'))
    assert controls['status']=='PASS_NATIVE_READY_FIT_READER_CONTROLS'and len(controls['cases'])==expected_controls
    assert all(r['rejected']for r in controls['cases'])
    record=json.loads((project/'r-front-ready-native.json').read_text())
    assert q.get('additional_hold_uncertainty_ps',0)==record.get('additional_hold_uncertainty_ps',0)
    if canonical:
        if q.get('host_video_register_mapping'):
            assert any(r['case']=='video_pipeline_register_coverage'for r in controls['cases'])
        if q.get('additional_hold_uncertainty_ps'):
            assert any(r['case']=='hold_margin_not_applied'for r in controls['cases'])
        assert record['native_visible_R_bits']==16 and record['evaluator_owned_Q_bits']==736
        assert record['internal_predicted_R_duplicate']is False
        proof=json.loads((project/'component-image-handoff.json').read_text())
        assert proof['status']=='PASS_REPAIRED_NATIVE_COMPONENT_IMAGE_HANDOFF'
        assert proof['exact_staged_native_sources_verified']and proof['canonical_native_R']
        assert proof['original_IM_words_verified']==3600 and proof['original_IFUM_entries_verified']==256
        assert proof['Cedar_entry_executed']and proof['final_RM_map_dump_identical']
        identities={h for n,h in proof['inputs'].items()if Path(n).name=='r-front-ready-native.json'}
        assert identities=={sha(project/'r-front-ready-native.json')},'Full-image test used different staged sources'
    assert q['source_hashes']==record['source_hashes']
    for n,h in q['source_hashes'].items():assert sha(project/n)==h,('Source changed after review',n)
    for name,pid_key,stages in [('run-state.json','builder_pid','stages'),('central-review-state.json','review_pid','actions')]:
        s=json.loads((project/name).read_text());assert s['status'].startswith('TERMINAL_')
        for pid in [s[pid_key]]+[r['pid']for r in s[stages]]:
            try:os.kill(pid,0)
            except ProcessLookupError:continue
            raise RuntimeError('Existing recorded process still present; inspect: '+str(pid))
    path=project/'assembly-state.json';assert not path.exists(),'Observe existing assembly job'
    rbf=project/'core/output_files/Dorado.rbf';assert not rbf.exists(),'Existing image requires inspection'
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),
        qualification_sha256=sha(project/'preassembly-qualification.json'),controls_sha256=sha(project/'fit-review-controls.json'))
    def write():path.write_text(json.dumps(state,indent=2)+'\n')
    write()
    try:
        with(project/'asm.log').open('x')as log:
            p=subprocess.Popen([str(quartus/'quartus_asm'),'Dorado'],cwd=project/'core',stdout=log,stderr=subprocess.STDOUT)
            state['assembler_pid']=p.pid;write();code=p.wait();state['returncode']=code;write();assert code==0
        assert 'Assembler was successful'in(project/'asm.log').read_text()
        assert rbf.stat().st_size>1000000
        state.update(status='TERMINAL_ASSEMBLED_REVIEWED_NATIVE_CANDIDATE',finished=time.time(),
            rbf_sha256=sha(rbf),rbf_bytes=rbf.stat().st_size,firmware_qualified=False,hardware_deployed=False)
        write();print(json.dumps(state),flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());write();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args();run(a.project.resolve(),a.quartus.resolve())
