#!/usr/bin/env python3
"""Elaborate the actual converted native hierarchy on its common host clock."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(stage,baseline_lint,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['clock_enable_conversion_staged']and not m['assembly_permitted']
    old=json.loads((baseline_lint/'run-state.json').read_text())
    baseline_manifest=m.get('clock_enable_lint_source_manifest_sha256',m['parent_manifest_sha256'])
    baseline_stage=m.get('clock_enable_lint_source_stage',m['parent_stage'])
    assert old['stage_manifest_sha256']==baseline_manifest and old['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'
    command=[s.replace(baseline_stage,str(stage))for s in old['command']]
    extra=m.get('additional_native_lint_sources',[])
    for name in extra:
        assert name in m['source_hashes'] and str(stage/name) not in command
    command[-1:-1]=[str(stage/name)for name in extra]
    harness=Path(command[-1]);assert harness.name=='tb_native_cedar.sv'
    raw=harness.read_text();changes=[
      ('dorado_native_machine #','dorado_native_machine_ce #'),
      ('reg step=0;always @(negedge host_clk)step<=source_enable;','wire step=source_enable;'),
      ('wire machine_clk=host_clk && step;','wire machine_clk=host_clk;'),
      ("always @(posedge machine_clk)if(boot_samples<boot_end_samples)",
       "always @(posedge machine_clk)if(step && boot_samples<boot_end_samples)")]
    for a,b in changes:
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1)
    tb=out/harness.name;tb.write_text(raw);command[-1]=str(tb)
    # Run the real optimization/lint passes first: XML-only does not check
    # combinational settling loops. Then freeze ancestry separately.
    xml_command=[s for s in command if s!='--lint-only']+['--xml-only','--xml-output',str(out/'native.xml'),'--Mdir',str(out/'obj')]
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        baseline_lint_state_sha256=sha(baseline_lint/'run-state.json'),command=command,
        harness_sha256=sha(tb),started=time.time(),simulation_started=False,hardware_changed=False,actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    for label,cmd in [('lint',command),('xml',xml_command)]:
        with(out/(label+'.log')).open('x')as log:
            p=subprocess.Popen(cmd,cwd=stage/'src',stdout=log,stderr=log)
            action=dict(label=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(action);save();code=p.wait()
        action.update(returncode=code,finished=time.time());save()
        if code:break
    state.update(status='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'if code==0 else'TERMINAL_NATIVE_CLOCK_ENABLE_ELABORATION_FAILED',
        returncode=code,finished=time.time(),files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'},
        functional_or_physical_timing_qualified=False)
    save();print(state['status'],flush=True);return code


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','baseline-lint','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.baseline_lint.resolve(),a.output.resolve()))
