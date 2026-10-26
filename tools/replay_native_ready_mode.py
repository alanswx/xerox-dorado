#!/usr/bin/env python3
"""Replay complete caller histories with the source-proved Ready comparison.

Change only production candidate.sv and add its combinational helper. Preserve
the existing prepared-Hold fixture, independently delayed oracle, assertions,
recorded inputs and limits byte-for-byte. Fixtures are not firmware/speed tests.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from qualify_native_ready_compare import CONTROL
from replay_r_read_buffer import outcomes
from review_baseboard_clock_consumer import verify
from stage_native_ready_mode import TARGET
from test_external_r_packet import sha


def run(source,stage,out,engine):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['engine']==engine and s['original_results_match']
    assert len(s['actions'])==2 and all(a['returncode']==0 for a in s['actions'])
    for a in s['actions']:assert sha(source/(a['label']+'.log'))==a['log_sha256']
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);assert s['stage_manifest_sha256']==sha(parent/'r-front-control-native.json')
    assert m['parallel_Ready_target_comparison_staged'] and not m['assembly_permitted']
    assert sha(source/'candidate.sv')==sha(parent/CONTROL)
    caller=(stage/'src/verilog/rtl/caller.sv').read_text().replace(',output wire [400:0] native_before_pins,output wire native_before_root','',1)
    caller=caller.replace(' assign native_before_pins=r_front_before;assign native_before_root=r_front_clock_before;\n','',1)
    assert caller==(source/'caller.sv').read_text()
    out.mkdir(parents=True);paths=[];rows=[]
    for name in s['files']:
        q=out/name;q.parent.mkdir(parents=True,exist_ok=True)
        q.write_bytes((stage/CONTROL).read_bytes()if name=='candidate.sv'else(source/name).read_bytes())
    for row in s['source_files']:
        name=row['path'];assert sha(source/name)==row['output_sha256']
        q=out/name;assert q.is_file();paths.append(q)
        rows.append(dict(path=name,parent_sha256=sha(source/name),output_sha256=sha(q)))
    q=out/'ready-target-compare.sv';shutil.copy2(stage/TARGET,q);paths.append(q)
    rows.append(dict(path=q.name,parent_sha256=None,output_sha256=sha(q)))
    assert sha(out/'tb.sv')==sha(source/'tb.sv')
    q=out/'executed-tools'/Path(__file__).name;shutil.copy2(Path(__file__),q)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),engine=engine,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        original_assertions_reference_and_recorded_inputs_retained=True,
        synthetic_producer_contexts=True,source_files=rows,actions=[],
        files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    commands=([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else
        [['verilator','--binary','--timing','--top-module','tb_r_front_packet','-Wno-fatal','-j','2','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    for label,cmd in zip(['compile','test'],commands):
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(a);save();code=p.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    original=(source/'test.log').read_text()
    metrics=re.findall(r'^HOLD_REPLAY_PREPARATION edges=(\d+) published0=(\d+) published1=(\d+)$',log,re.M)
    # JSON stores lists, while re.findall returns tuples.
    passed=code==0 and outcomes(log)==outcomes(original) and [list(v)for v in metrics]==s['metrics']
    state.update(status='PASS_READY_MODE_COMPLETE_CALLER_REPLAY'if passed else'TERMINAL_READY_MODE_CALLER_REPLAY_NOT_ESTABLISHED',
        returncode=code,finished=time.time(),original_results_match=outcomes(log)==outcomes(original),
        outcomes=outcomes(log),metrics=metrics,native_firmware_timing_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','stage','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.stage.resolve(),a.output.resolve(),a.engine))
