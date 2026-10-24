#!/usr/bin/env python3
"""Recover all native corners from the unchanged completed hold-margin fit.

The input SDC leaked a global variable called name, overwriting the original
corner reporter's filenames. Preserve those reports. This independent reader
uses private names, reapplies the selected operating condition after read_sdc,
and checks all four complete setup/hold and pack-capture reports. It does not
modify a compiled input, constraint, fitted database or FPGA image.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

TCL = '''project_open Dorado
create_timing_netlist -model slow
set dorado_hold_corners {}
foreach_in_collection dorado_hold_op [get_available_operating_conditions] {
 lappend dorado_hold_corners [get_operating_conditions_info $dorado_hold_op -name]
}
delete_timing_netlist
if {[llength $dorado_hold_corners]!=4} {error "Expected four corners"}
foreach dorado_hold_corner $dorado_hold_corners {
 create_timing_netlist
 set_operating_conditions $dorado_hold_corner
 read_sdc
 set_operating_conditions $dorado_hold_corner
 update_timing_netlist
 puts "CORRECTED_PACK_CORNER $dorado_hold_corner"
 foreach dorado_hold_mode {setup hold recovery removal} {
  report_timing -$dorado_hold_mode -npaths 100 -detail full_path -file OUTPUT/global-$dorado_hold_mode-$dorado_hold_corner.rpt
 }
 set dorado_hold_src [get_registers {*|u_media|generation*}]
 set dorado_hold_dst [get_registers {*|u_media|flushed_generation*}]
 puts "CORRECTED_PACK_COVERAGE $dorado_hold_corner source=[get_collection_size $dorado_hold_src] sink=[get_collection_size $dorado_hold_dst]"
 foreach dorado_hold_mode {setup hold} {
  report_timing -$dorado_hold_mode -from $dorado_hold_src -to $dorado_hold_dst -npaths 32 -detail full_path -file OUTPUT/pack-$dorado_hold_mode-$dorado_hold_corner.rpt
 }
 report_min_pulse_width -file OUTPUT/mpw-$dorado_hold_corner.rpt
 report_ucp -file OUTPUT/ucp-$dorado_hold_corner.rpt
 report_clocks -file OUTPUT/clocks-$dorado_hold_corner.rpt
 delete_timing_netlist
}
project_close
'''


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(project, output, quartus):
    assert not output.exists()
    manifest = project/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    assert m['pack_hold_margin']['RTL_byte_unchanged'] and not m['assembly_permitted']
    for name, digest in m['source_hashes'].items():
        assert sha(project/name) == digest
    for name in ['run-state.json', 'physical-report/run-state.json', 'cp-key-report/run-state.json']:
        s = json.loads((project/name).read_text())
        assert s['status'].startswith('TERMINAL')
        for pid in [s.get('builder_pid', s.get('pid'))]+[a['pid'] for a in s.get('stages', s.get('actions', []))]:
            assert not (Path('/proc')/str(pid)).exists(), pid
    output.mkdir(parents=True)
    script = output/'audit_corrected_pack_corners.tcl'
    script.write_text(TCL.replace('OUTPUT', str(output)))
    (output/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING_CORRECTED_READ_ONLY_CORNER_REPORTS', pid=os.getpid(), started=time.time(),
        manifest_sha256=sha(manifest), script_sha256=sha(script), constraints_changed=False,
        assembly_permitted=False, native_timing_qualified=False, actions=[])
    path = output/'run-state.json'

    def save():
        temp = path.with_suffix('.tmp')
        temp.write_text(json.dumps(state, indent=2)+'\n')
        os.replace(temp, path)

    save()
    try:
        with (output/'audit.log').open('x') as log:
            command = [str(quartus/'quartus_sta'), '-t', str(script)]
            process = subprocess.Popen(command, cwd=project/'core', stdout=log, stderr=subprocess.STDOUT)
            row = dict(pid=process.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            code = process.wait()
            row.update(returncode=code, finished=time.time())
            save()
        assert code == 0
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest
        state.update(status='TERMINAL_CORRECTED_PACK_REPORTS_REVIEW_REQUIRED', finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
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
