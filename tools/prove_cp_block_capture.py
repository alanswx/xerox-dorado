#!/usr/bin/env python3
"""Prove default-off CP Block capture preserves all original control results.

Use both real terminal depths, unconstrained Q/pins/roots/refresh and all363
outputs. Enabling the descriptor intentionally changes some captures; its
arrival qualification belongs to the independently timed original-cell tests.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_cp_block_capture import rewrite
from prove_control_fall_fusion import sources
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha


def run(source,out,yosys):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='PASS_OWNED_FULL_PROCESSOR_PACKETS' and s['mutation'] is None
    raw=(source/'candidate.sv').read_text()
    original,_=sources(raw);actual,_=sources(rewrite(raw,False))
    original=re.sub(r'\bdorado_control_(hold_atomic_step|terminal_body)\b',r'original_\1',original)
    out.mkdir(parents=True)
    for name in [Path(__file__).name,'generate_cp_block_capture.py']:
        shutil.copy2(Path(__file__).with_name(name),out/name)
    (out/'original.sv').write_text(original);(out/'actual.sv').write_text(actual)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),source=str(source),
               source_state_sha256=sha(source/'run-state.json'),scope=__doc__,
               arbitrary_binary_input_bits=955,compared_bits=363,wave_depths=[5,24],
               no_assumptions=True,enabled_capture_qualified=False,native_integrated=False,
               files={p.name:sha(p) for p in out.iterdir()},cases=[])
    def save():
        (out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for waves in [5,24]:
        d=out/f'disabled-w{waves}';d.mkdir()
        (d/'top.sv').write_text(f'''module equivalence(input wire [155:0] q,
 input wire before_root,after_root,input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire [362:0] actual,expected,output wire equivalent);
 original_hold_atomic_step #(.WAVES({waves})) original(q,before_root,after_root,before_pins,after_pins,
  expected[362:207],expected[206:151],expected[150:95],expected[94],refresh,expected[93:0]);
 dorado_control_hold_atomic_step #(.WAVES({waves})) candidate(q,before_root,after_root,before_pins,after_pins,
  actual[362:207],actual[206:151],actual[150:95],actual[94],refresh,actual[93:0]);
 assign equivalent=actual==expected;
endmodule
''')
        (d/'proof.ys').write_text('''read_verilog -sv -defer /work/../original.sv /work/../actual.sv /work/top.sv
hierarchy -check -top equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equivalent 1 -verify -show-ports
''')
        # Mount the parent, keeping the exact frozen sibling source files.
        script=(d/'proof.ys').read_text().replace('/work/../','/work/').replace('/work/top.sv',f'/work/disabled-w{waves}/top.sv')
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(out)
        cmd=[str(yosys),'-Q','-T','-s',f'/work/disabled-w{waves}/proof.ys']
        row=dict(waves=waves,command=cmd,started=time.time(),files={p.name:sha(p) for p in d.iterdir()})
        with (d/'proof.log').open('x') as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT,env=env)
            row['pid']=child.pid;state['cases'].append(row);save();code=child.wait()
        row.update(returncode=code,passed=code==0 and 'no model found: SUCCESS!' in (d/'proof.log').read_text(),
                   log_sha256=sha(d/'proof.log'),finished=time.time());save()
        print('disabled',waves,'PASS' if row['passed'] else 'FAIL',flush=True)
        if not row['passed']:break
    passed=len(state['cases'])==2 and all(r['passed'] for r in state['cases'])
    state.update(status='PASS_DISABLED_CP_BLOCK_CAPTURE_ALL_RESULTS' if passed else 'TERMINAL_CP_BLOCK_DEFAULT_NOT_ESTABLISHED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve()))
