#!/usr/bin/env python3
"""Elaborate the private 844-Q replacement in the complete native harness.

No simulation is started, no media is opened and no FPGA build/load is run.
All staged identities and baseline harness inputs are checked before lint.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(stage,native_run,out,baseline_only=False,structural=False,report_unoptflat=False,optimization=None):
    out.mkdir(parents=True,exist_ok=False)
    record=json.loads((stage/'r-front-control-native.json').read_text())
    assert record['status']=='STAGED_PROCESSOR_CONTROL_NATIVE_EXPERIMENT'and not record['assembly_permitted']
    for n,h in record['source_hashes'].items():assert sha(stage/n)==h,n
    base=json.loads(gzip.decompress((native_run/'provenance.json.gz').read_bytes()))
    for n,h in base['inputs'].items():assert sha(native_run/'inputs'/n)==h,n
    root=native_run/'inputs'if baseline_only else stage
    support=[root/n for n in base['used_hdl']]
    if not baseline_only:support += [stage/n for n in record['extra_RTL_sources']if n not in base['used_hdl']]
    harness_root=native_run/'inputs'
    if not baseline_only and record.get('native_observation_harness'):
        assert record['native_observation_harness']=='harness/tb_native_cedar.sv'
        harness_root=stage
    support += [harness_root/'harness/xsds_model.sv',harness_root/'harness/tb_native_cedar.sv']
    assert all(p.exists()for p in support)
    cmd=['verilator','--lint-only','--timing','--top-module','tb_native_cedar','-Wno-fatal',
         '-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH','-Wno-TIMESCALEMOD',
         '-Iverilog/cells','-Iverilog/generated','-Iverilog/vendor/6502','-Iverilog/vendor/riot',
         *['-D'+d for d in base['defines']],*map(str,support)]
    if structural:cmd=[s for s in cmd if s not in ['-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN']]
    if report_unoptflat:
        assert structural
        cmd+=['--report-unoptflat','--Mdir',str(out/'obj')]
    if optimization is not None:cmd+=['-O'+str(optimization)]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),stage=str(stage),
               stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_files=len(support),
               command=cmd,baseline_only=baseline_only,structural_warnings_visible=structural,simulation_started=False,media_opened=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    with(out/'lint.log').open('w')as log:
        p=subprocess.Popen(cmd,cwd=root/'src',stdout=log,stderr=subprocess.STDOUT)
        state['lint_pid']=p.pid;save();rc=p.wait()
    state.update(status='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'if not rc else'NATIVE_PROCESSOR_CONTROL_ELABORATION_FAILED',
                 returncode=rc,finished=time.time(),firmware_qualified=False,physical_timing_qualified=False)
    save();print(state['status']);return rc


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--stage',type=Path,required=True)
    p.add_argument('--native-run',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--baseline-only',action='store_true');p.add_argument('--structural',action='store_true')
    p.add_argument('--report-unoptflat',action='store_true')
    p.add_argument('--optimization',type=int,choices=[0,1,2,3])
    a=p.parse_args();sys.exit(run(a.stage.resolve(),a.native_run.resolve(),a.output.resolve(),a.baseline_only,a.structural,a.report_unoptflat,a.optimization))
