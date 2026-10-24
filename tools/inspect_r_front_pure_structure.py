#!/usr/bin/env python3
"""Check the elaborated pure evaluator has no clocked state or memory arrays.

Bind every source to its frozen comparison manifest. Calibrate the same XML
inspection by injecting one observable clocked bit into the private pure top;
its edge and delayed assignment must be visible. Preserve full warnings. This
is a structural check, not physical timing or complete two-event qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time
import xml.etree.ElementTree as ET

from test_r_front_forward_view import sha


def inspect(path):
    r=ET.parse(path).getroot()
    modules=[v.get('origName') for v in r.iter('module')]
    assert modules.count('dorado_r_front_pure_step')==1
    return dict(module_types=len(modules),clocked_items=[v.attrib for v in r.iter('senitem')],
        delayed_assignments=len(list(r.iter('assigndly'))),delays=len(list(r.iter('delay'))),
        unpacked_arrays=len(list(r.iter('unpackarraydtype'))),
        forbidden_owners=[n for n in modules if n in ['dorado_cp_prefix_cache','dorado_r_file_ports']])


def run(source,out):
    assert not out.exists()
    s=json.loads((source/'run-state.json').read_text())
    assert s['all_original_assertions_retained'] and s['one_physical_SRAM_owner']
    paths=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        if p.name!='tb_r_front_local.sv':paths.append(p)
    out.mkdir(parents=True);shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
        source_files=s['source_files'],cases=[],hardware_changed=False,native_integrated=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    save()
    for bad in [False,True]:
        d=out/('clocked-bit-control' if bad else 'positive');d.mkdir();actual=list(paths)
        if bad:
            candidate=source/'candidate.sv';raw=candidate.read_text();old=" assign cp_ready=1'b1;"
            assert raw.count(old)==1
            raw=raw.replace(old," reg structural_probe=0;always @(posedge host_clk)structural_probe<=advance;\n assign cp_ready=structural_probe;",1)
            (d/'candidate.sv').write_text(raw);actual=[(d/'candidate.sv' if p==candidate else p) for p in paths]
        cmd=['verilator','--xml-only','--timing','-Wno-fatal','--top-module','dorado_r_front_pure_step',
            '--xml-output',str(d/'hierarchy.xml'),'--Mdir',str(d/'obj'),*map(str,actual)]
        with (d/'elaborate.log').open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            row=dict(bad=bad,pid=p.pid,command=cmd,started=time.time());state['cases'].append(row);save();code=p.wait()
        row.update(returncode=code,finished=time.time());save()
        if code:break
        a=inspect(d/'hierarchy.xml');row.update(structure=a,XML_sha256=sha(d/'hierarchy.xml'),log_sha256=sha(d/'elaborate.log'))
        if bad:passed=len(a['clocked_items'])>0 and a['delayed_assignments']>0
        else:passed=not a['clocked_items'] and not a['delayed_assignments'] and not a['delays'] and not a['unpacked_arrays'] and not a['forbidden_owners']
        row['passed']=passed;save()
        if not passed:break
    passed=len(state['cases'])==2 and all(v.get('passed') for v in state['cases'])
    state.update(status='PASS_PURE_PROCESSOR_NO_EMBEDDED_STATE' if passed else 'TERMINAL_PURE_PROCESSOR_STRUCTURE_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
