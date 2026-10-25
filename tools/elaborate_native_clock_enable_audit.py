#!/usr/bin/env python3
"""Freeze an unflattened actual native hierarchy for a clock-enable audit.

Use the exact source-bound native elaboration command at the staged QSF clock.
No model, input, running simulator, clock or remote fit is changed. XML is an
inventory input; it does not qualify a gated-clock to enable conversion.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(stage,lint,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    l=json.loads((lint/'run-state.json').read_text())
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'and l['stage_clock_macro_checked_against_QSF']
    assert l['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert '-DDORADO_HOST_CLOCK_HZ=66666667'in l['command']and l['source_files']==200
    command=[s for s in l['command']if s!='--lint-only']+['--xml-only','--xml-output',str(out/'native.xml'),'--Mdir',str(out/'obj')]
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',scope=__doc__,stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        lint_state_sha256=sha(lint/'run-state.json'),command=command,started=time.time(),source_files=200,
        synthesis_or_clock_conversion_qualified=False,hardware_or_media_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    with(out/'elaboration.log').open('x')as log:
        p=subprocess.Popen(command,cwd=stage/'src',stdout=log,stderr=log);state['pid']=p.pid;save();code=p.wait()
    state.update(status='ELABORATED_NATIVE_CLOCK_ENABLE_AUDIT_INPUT'if code==0 else'TERMINAL_CLOCK_ENABLE_AUDIT_ELABORATION_FAILED',
        returncode=code,finished=time.time(),files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'})
    save();print(state['status'],flush=True);return code


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','lint','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.lint.resolve(),a.output.resolve()))
