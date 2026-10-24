#!/usr/bin/env python3
"""Calibrate a stronger minimum-delay contract on the failed pack capture.

The old completed netlist has a real same-clock hold failure. Query exactly
the complete 32-bit generation/flushed-generation buses, then strengthen their
minimum-delay relationship by0.250ns. Check all32 paths before/after: the
hold change must equal the relationship plus the existing derived clock
uncertainty, with identical data/skew and unchanged setup at all four corners. This is
read-only calibration, not a refit, timing pass or permission to mask a path.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

CONSTRAINT_BODY = r'''# Stronger hold relationship for the complete pack-generation capture.
# Preserve real clock latency/skew. No false paths or guest-cycle changes.
set pack_generation [get_registers {*|u_media|generation*}]
set pack_flushed [get_registers {*|u_media|flushed_generation*}]
foreach collection [list $pack_generation $pack_flushed] {
 set bits [dict create]
 foreach_in_collection node $collection {
  set name [get_register_info -name $node]
  if {![regexp {generation\[([0-9]+)\]} $name match index]} {error "Unbound generation register: $name"}
  dict set bits $index 1
 }
 if {[lsort -integer [dict keys $bits]] ne {0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31}} {
  error "Incomplete generation capture coverage: [dict keys $bits]"
 }
}
set_min_delay -from $pack_generation -to $pack_flushed 0.250
'''
CONSTRAINT = ('proc dorado_apply_generation_hold_contract {} {\n'+CONSTRAINT_BODY+
              '}\ndorado_apply_generation_hold_contract\nrename dorado_apply_generation_hold_contract {}\n')
TCL = '''project_open Dorado
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {lappend corners [get_operating_conditions_info $op -name]}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions"}
foreach corner $corners {
 create_timing_netlist
 set_operating_conditions $corner
 read_sdc
 update_timing_netlist
 set a [get_registers {*|u_media|generation*}]
 set b [get_registers {*|u_media|flushed_generation*}]
 puts "PACK_HOLD_MARGIN $corner source=[get_collection_size $a] sink=[get_collection_size $b]"
 foreach mode {setup hold} {report_timing -$mode -from $a -to $b -npaths 32 -detail full_path -file OUTPUT/$mode-before-$corner.rpt}
 set name DORADO_HOLD_NAMESPACE_SENTINEL
CONSTRAINT
 if {$name ne "DORADO_HOLD_NAMESPACE_SENTINEL"} {error "Hold SDC leaked local variables"}
 update_timing_netlist
 foreach mode {setup hold} {report_timing -$mode -from $a -to $b -npaths 32 -detail full_path -file OUTPUT/$mode-after-$corner.rpt}
 delete_timing_netlist
}
project_close
'''


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def paths(raw):
    result = {}
    for part in re.split(r'Path #\d+:', raw)[1:]:
        def field(label):
            return re.search(r'; '+label+r'\s*; ([^;]+);', part)[1].strip()
        source, sink = field('From Node'), field('To Node')
        index = int(re.search(r'\|generation\[(\d+)\]', source)[1])
        assert int(re.search(r'\|flushed_generation\[(\d+)\]', sink)[1]) == index
        assert index not in result
        row = dict(source=source, sink=sink, slack=float(re.search(r'(?:Hold|Setup) slack is ([+-]?[\d.]+)', part)[1]),
                   data=float(field('Data Delay')), skew=float(field('Clock Skew')))
        for line in part.splitlines():
            cells = [c.strip() for c in line.split(';')]
            if len(cells) > 3 and cells[-2] in ['clock uncertainty', 'latch edge time']:
                row[cells[-2]] = float(cells[2])
        result[index] = row
    assert set(result) == set(range(32)), sorted(result)
    return result


def run(project, out, quartus):
    assert not out.exists()
    manifest = project/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    assert not m['assembly_permitted']
    for name, digest in m['source_hashes'].items():
        assert sha(project/name) == digest, name
    pids = []
    for name in ['run-state.json', 'physical-report/run-state.json', 'cp-key-report/run-state.json']:
        s = json.loads((project/name).read_text())
        assert s['status'].startswith('TERMINAL')
        pids.append(s.get('builder_pid', s.get('pid')))
        pids.extend(a['pid'] for a in s.get('stages', s.get('actions', [])))
    assert all(not (Path('/proc')/str(pid)).exists() for pid in pids)
    out.mkdir(parents=True)
    script = out/'audit_pack_hold_margin.tcl'
    script.write_text(TCL.replace('CONSTRAINT', CONSTRAINT).replace('OUTPUT', str(out)))
    (out/'pack_generation_hold.sdc').write_text(CONSTRAINT)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING_READ_ONLY_HOLD_CALIBRATION', driver_pid=os.getpid(), started=time.time(),
                 project=str(project), manifest_sha256=sha(manifest), script_sha256=sha(script),
                 previous_pids_absent=pids, setup_constraint_changed=False, RTL_changed=False,
                 minimum_delay_strengthening_ns=0.250, assembly_permitted=False,
                 constraint_namespace_isolated=True)
    path = out/'run-state.json'
    path.write_text(json.dumps(state, indent=2)+'\n')
    with (out/'audit.log').open('x') as log:
        process = subprocess.Popen([str(quartus/'quartus_sta'), '-t', str(script)], cwd=project/'core', stdout=log, stderr=subprocess.STDOUT)
        state['pid'] = process.pid
        path.write_text(json.dumps(state, indent=2)+'\n')
        code = process.wait()
    corners = []
    for before in sorted(out.glob('hold-before-*.rpt')):
        corner = before.name[len('hold-before-'):-4]
        row = dict(condition=corner)
        for mode in ['setup', 'hold']:
            values = []
            details = []
            for phase in ['before', 'after']:
                raw = (out/f'{mode}-{phase}-{corner}.rpt').read_text()
                values.append(float(re.search(r'Worst case slack is ([+-]?[\d.]+)', raw, re.I)[1]))
                details.append(paths(raw))
            row[mode] = values
            for index in range(32):
                before, after = details[0][index], details[1][index]
                assert before['source'] == after['source'] and before['sink'] == after['sink']
                assert before['data'] == after['data'] and before['skew'] == after['skew']
                if mode == 'setup':
                    assert before == after
                else:
                    edge = after['latch edge time']-before['latch edge time']
                    uncertainty = after['clock uncertainty']-before['clock uncertainty']
                    assert abs(edge-0.250) < 1e-6
                    assert uncertainty >= 0
                    assert abs(after['slack']-before['slack']+edge+uncertainty) < 0.002
            row[mode+'_all32_paths_verified'] = True
            if mode == 'hold':
                row['derived_uncertainty_increment_ns'] = uncertainty
        corners.append(row)
    for name, digest in m['source_hashes'].items():
        assert sha(project/name) == digest
    passed = code == 0 and len(corners) == 4
    state.update(status='PASS_STRONGER_PACK_HOLD_CONTRACT_CALIBRATION' if passed else 'TERMINAL_HOLD_CALIBRATION_NOT_ESTABLISHED',
                 returncode=code, finished=time.time(), corners=corners,
                 files={p.name: sha(p) for p in out.iterdir() if p.is_file() and p.name != 'run-state.json'})
    path.write_text(json.dumps(state, indent=2)+'\n')
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['project', 'output']:
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args = parser.parse_args()
    try:
        raise SystemExit(run(args.project.resolve(), args.output.resolve(), args.quartus.resolve()))
    except Exception as error:
        path = args.output/'run-state.json'
        if path.exists():
            state = json.loads(path.read_text())
            state.update(status='TERMINAL_HOLD_CALIBRATION_NOT_ESTABLISHED',
                         error=repr(error), finished=time.time())
            path.write_text(json.dumps(state, indent=2)+'\n')
        raise
