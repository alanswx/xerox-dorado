#!/usr/bin/env python3
"""Replay the original-ROM whole BaseBoard with the new RIOT RAM representation.

Keep all208 original scalar/vector checks,69 outputs,37 RAM comparisons,
reset-clear/MidasSetup assertions and random pauses. Only the candidate's
five physical RAM comparisons become comparisons of their logical byte view.
This is a whole-board differential test, not native firmware/clock timing.
"""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

from prepare_riot_blockram import RIOT
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
CELL='src/verilog/cells/cell_MCS6532.v'


def run(baseline,component,tests,out):
    assert not out.exists()
    b=json.loads((baseline/'run-state.json').read_text());verify(baseline,b['files'])
    assert b['status']=='PASS_ORIGINAL_ROM_BASEBOARD_COMMON_CLOCK_EQUIVALENCE'
    t=json.loads((tests/'run-state.json').read_text());verify(tests,t['files'])
    assert t['status']=='PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT'
    assert t['component_state_sha256']==sha(component/'results.json')
    c=json.loads((component/'results.json').read_text());verify(component,c['files'])
    stage=Path(b['stage']);m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert sha(stage/'r-front-control-native.json')==b['stage_manifest_sha256']
    assert m['source_hashes'][RIOT]==c['original_RIOT_sha256']
    out.mkdir(parents=True)
    raw=(stage/CELL).read_text();before=module(raw,'cell_MCS6532_ce')
    a='M6532_ce u_riot (.guest_advance(guest_advance),'
    z='dorado_riot_blockram_ce u_riot (.guest_advance(guest_advance),'
    assert before.count(a)==1
    after=before.replace(a,z,1)
    assert after.replace(z,a,1)==before
    (out/'cell_MCS6532.v').write_text(raw.replace(before,after,1))
    for n in ['candidate.sv','ram.sv']:shutil.copy2(component/n,out/n)
    bench=(baseline/'tb_native_baseboard_reset.sv').read_text();original=bench
    arrays=re.findall(r'candidate(\.\w+\.u_riot)\.riot_ram\[k\]',bench)
    assert len(arrays)==5 and len(set(arrays))==5,arrays
    for suffix in arrays:
        a='candidate'+suffix+'.riot_ram[k]'
        z='(candidate'+suffix+'.native_ram.written[k] ? candidate'+suffix+'.native_ram.backing[k] : 8\'b0)'
        assert bench.count(a)==1;bench=bench.replace(a,z,1)
    assert bench.count('$fatal')==original.count('$fatal')
    assert bench.count('CLOCK_ENABLE_RAM_MISMATCH')==original.count('CLOCK_ENABLE_RAM_MISMATCH')==37
    # The baseline emits scalar/output checks twice: every edge and at finish.
    assert bench.count('CLOCK_ENABLE_STATE_MISMATCH')==original.count('CLOCK_ENABLE_STATE_MISMATCH')==2*208
    assert bench.count('CLOCK_ENABLE_OUTPUT_MISMATCH')==original.count('CLOCK_ENABLE_OUTPUT_MISMATCH')==2*69
    (out/'tb_native_baseboard_reset.sv').write_text(bench)
    command=b['actions'][0]['command'].copy()
    assert command.count(str(stage/CELL))==1
    command[command.index(str(stage/CELL))]=str(out/'cell_MCS6532.v')
    old_tb=str(baseline/'tb_native_baseboard_reset.sv');assert command[-1]==old_tb
    command[-1:]=[str(out/'candidate.sv'),str(out/'ram.sv'),str(out/'tb_native_baseboard_reset.sv')]
    at=command.index('--Mdir')+1;command[at]=str(out/'obj')
    for n in [Path(__file__).name,'prepare_riot_blockram.py']:shutil.copy2(Path(__file__).with_name(n),out/n)
    state=dict(status='RUNNING',scope=__doc__,started=time.time(),baseline=str(baseline),
               baseline_state_sha256=sha(baseline/'run-state.json'),component_state_sha256=sha(component/'results.json'),
               component_tests_state_sha256=sha(tests/'run-state.json'),
               original_checks_retained={'state_vectors':208,'outputs':69,'RAM_arrays':37},
               logical_RIOT_RAM_representations=arrays,actions=[],
               files={str(q.relative_to(out)):sha(q)for q in out.rglob('*')if q.is_file()})
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for label,cmd in [('compile',command),('test',[str(out/'obj/test')])]:
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,cwd=stage/'src',stdout=log,stderr=log)
            action=dict(label=label,pid=child.pid,command=cmd,started=time.time());state['actions'].append(action);save();code=child.wait()
        action.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    lines=re.findall(r'^CLOCK_ENABLE_BASEBOARD_PASS .*$',log,re.M)
    passed=code==0 and len(lines)==1 and lines==re.findall(r'^CLOCK_ENABLE_BASEBOARD_PASS .*$',(baseline/'test.log').read_text(),re.M)
    passed=passed and 'BB_RESET_MIDAS_RETURN'in log and 'dex=256'in log
    state.update(status='PASS_ORIGINAL_ROM_BASEBOARD_RIOT_BLOCK_RAM'if passed else'TERMINAL_BASEBOARD_RIOT_BLOCK_RAM_NOT_ESTABLISHED',
                 returncode=code,finished=time.time(),metrics=lines,native_or_physical_timing_or_firmware_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['baseline','component','tests','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.baseline.resolve(),a.component.resolve(),a.tests.resolve(),a.output.resolve()))
