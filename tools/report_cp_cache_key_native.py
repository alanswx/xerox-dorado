#!/usr/bin/env python3
"""Query actual CP-cache key registers after existing native reports complete.

Count fitted key-bit keepers and their physical output loads, distinguishing
clock-to-Q CELL arcs from IC routing edges. Record aliases/duplicates and any
bits outside the proven support. Run on the preceding completed design for
calibration, then on the projected-key candidate; infer no timing or firmware
pass from source masks or successful queries. Never refit or assemble.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

TCL = r'''project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
set keys [get_registers {*|cp_guard|cache_key*}]
puts "CP_CACHE_KEY_COUNT\t[get_collection_size $keys]"
foreach_in_collection key $keys {
 set name [get_register_info -name $key]
 if {![regexp {cache_key\[([0-9]+)\]} $name match index]} {
  error "Cannot bind fitted cache-key index: $name"
 }
 set outputs 0
 foreach edge [get_node_info -fanout_edges $key] {
  if {[string equal -nocase [get_edge_info -delay_type $edge] CELL]} {
   incr outputs
   set pin [get_edge_info -dst $edge]
   set count 0
   foreach route [get_node_info -fanout_edges $pin] {
    if {[string equal -nocase [get_edge_info -delay_type $route] IC]} {incr count}
   }
   puts "CP_CACHE_KEY_OUTPUT\t$index\t$name\t[get_node_info -name $pin]\t$count"
  }
 }
 puts "CP_CACHE_KEY_KEEPER\t$index\t$name\t$outputs"
}
delete_timing_netlist
project_close
'''


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run(project, out, quartus):
    assert not out.exists()
    manifest = project/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    assert not m['assembly_permitted']
    out.mkdir(parents=True)
    script = out/'audit_cp_cache_key_native.tcl'
    script.write_text(TCL)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='WAITING_EXISTING_PHYSICAL_NATIVE_REPORT', pid=os.getpid(), started=time.time(),
        project=str(project), manifest_sha256=sha(manifest), script_sha256=sha(script),
        driver_sha256=sha(Path(__file__)), actions=[], constraints_changed=False,
        assembly_permitted=False, native_timing_qualified=False)

    def save():
        tmp = out/'run-state.json.tmp'
        tmp.write_text(json.dumps(state, indent=2)+'\n')
        os.replace(tmp, out/'run-state.json')

    save()
    try:
        while True:
            previous = json.loads((project/'physical-report/run-state.json').read_text())
            proc = Path('/proc')/str(previous['pid'])
            if previous['status'].startswith('TERMINAL') and not proc.exists():
                break
            assert proc.exists(), 'Inspect the existing reporter; never restart it'
            assert str(project) in (proc/'cmdline').read_bytes().replace(b'\0', b' ').decode()
            time.sleep(10)
        build = json.loads((project/'run-state.json').read_text())
        state.update(original_report=previous, original_build=build)
        save()
        assert previous['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
        assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
        assert all(a['returncode'] == 0 for a in [*build['stages'], *previous['actions']])
        for pid in [build['builder_pid'], previous['pid'], *[a['pid'] for a in build['stages']], *[a['pid'] for a in previous['actions']]]:
            assert not (Path('/proc')/str(pid)).exists(), pid
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest, name
        command = [str(quartus/'quartus_sta'), '-t', str(script)]
        with (out/'audit.log').open('x') as log:
            proc = subprocess.Popen(command, cwd=project/'core', stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=command, pid=proc.pid, started=time.time())
            state['actions'].append(row)
            state['status'] = 'RUNNING_READ_ONLY_CP_KEY_QUERY'
            save()
            row.update(returncode=proc.wait(), finished=time.time())
            save()
        assert row['returncode'] == 0
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest, name
        records = [line.split('\t') for line in (out/'audit.log').read_text().splitlines() if line.startswith('CP_CACHE_KEY_')]
        counts = [r for r in records if r[0] == 'CP_CACHE_KEY_COUNT']
        keepers = [r for r in records if r[0] == 'CP_CACHE_KEY_KEEPER']
        outputs = [r for r in records if r[0] == 'CP_CACHE_KEY_OUTPUT']
        assert len(counts) == 1 and int(counts[0][1]) == len(keepers)
        assert keepers and outputs and all(0 <= int(r[1]) < 567 for r in keepers)
        indices = sorted({int(r[1]) for r in keepers})
        support = m.get('cp_cache_key_projection', {}).get('cared_key_bits')
        result = dict(status='TERMINAL_CP_KEY_TOPOLOGY_REVIEW_REQUIRED', keepers=keepers, outputs=outputs,
            physical_key_keeper_count=len(keepers), distinct_logical_key_bits=indices,
            recorded_support=support, out_of_support_bits=None if support is None else sorted(set(indices)-set(support)),
            critical_unused_key138_present=138 in indices,
            physical_output_pin_IC_counts=True, native_timing_qualified=False,
            firmware_qualified=False, speedup_qualified=False, assembly_permitted=False)
        (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
        state.update(status=result['status'], finished=time.time(), log_sha256=sha(out/'audit.log'))
        save()
    except BaseException:
        state.update(status='TERMINAL_CP_KEY_QUERY_FAILED', finished=time.time())
        save()
        raise
    print(state['status'], flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['project', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--quartus', type=Path, default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a = p.parse_args()
    run(a.project.resolve(), a.output.resolve(), a.quartus.resolve())
