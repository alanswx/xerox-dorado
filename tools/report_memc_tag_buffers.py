#!/usr/bin/env python3
"""Query actual LCELL cells, net drivers and direct routing fanout after fitting.

Wait for this project's existing physical reporter and builder, verifying
their live handles and terminal states. Never refit, assemble or alter SDCs.
Direct fanout is measured at physical output pins: a register keeper itself
has one clock-to-Q CELL edge, not its downstream routing load count. That
distinction is calibrated against the preceding321-load fitted baseline.
Raw cells/nets may be merged or removed; do not infer168 distinct buffers
from168 RTL instances or turn this structural query into a timing pass.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

TCL = '''# Read-only query of the completed fitted design, preserving all SDCs.
project_open Dorado
create_timing_netlist -model slow
read_sdc
update_timing_netlist
proc direct_ic_count {node} {
 set count 0
 foreach edge [get_node_info -fanout_edges $node] {
  if {[string equal -nocase [get_edge_info -delay_type $edge] IC]} {incr count}
 }
 return $count
}
foreach package {a14 a15 b14 b15 c14 c15 d14 d15} {
 foreach side {qa qb} {
  set filter "*|MemC_full_snapshot_experiment:b_MemC|cell_MC1668_snapshot:u_${package}|${side}*"
  set owners [get_registers $filter]
  puts "TAG_OWNER_COUNT\\t$package\\t$side\\t[get_collection_size $owners]"
  foreach_in_collection owner $owners {
   set name [get_register_info -name $owner]
   foreach edge [get_node_info -fanout_edges $owner] {
    if {[string equal -nocase [get_edge_info -delay_type $edge] CELL]} {
     set output [get_edge_info -dst $edge]
     puts "TAG_OWNER_OUTPUT\\t$name\\t[get_node_info -name $output]\\t[direct_ic_count $output]"
    }
   }
  }
 }
}
set cells [get_cells -compatibility_mode {*tag_lcell*}]
puts "TAG_CELL_COUNT\\t[get_collection_size $cells]"
foreach_in_collection cell $cells {
 puts "TAG_CELL\\t[get_cell_info -name $cell]\\t[get_cell_info -type $cell]\\t[get_cell_info -wysiwyg_type $cell]\\t[get_cell_info -location $cell]"
}
set nets [get_nets {*tag_lcell*}]
puts "TAG_NET_COUNT\\t[get_collection_size $nets]"
foreach_in_collection net $nets {
 set pin [get_net_info -pin $net]
 set cell [get_pin_info -parent_cell $pin]
 puts "TAG_NET\\t[get_net_info -name $net]\\t[get_pin_info -name $pin]\\t[get_cell_info -name $cell]\\t[get_cell_info -wysiwyg_type $cell]\\t[get_cell_info -location $cell]\\t[direct_ic_count $pin]"
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
    assert m['cache_tag_lcell_buffers']['compared_address_bits'] == 168 and not m['assembly_permitted']
    out.mkdir(parents=True)
    script = out/'audit_memc_tag_buffers.tcl'
    script.write_text(TCL)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='WAITING_EXISTING_PHYSICAL_NATIVE_REPORT', pid=os.getpid(), started=time.time(),
        project=str(project), manifest_sha256=sha(manifest), script_sha256=sha(script),
        driver_sha256=sha(Path(__file__)), actions=[], constraints_changed=False,
        assembly_permitted=False, physical_replication_qualified=False, native_timing_qualified=False)

    def save():
        temp = out/'run-state.json.tmp'
        temp.write_text(json.dumps(state, indent=2)+'\n')
        os.replace(temp, out/'run-state.json')

    save()
    try:
        while True:
            original = json.loads((project/'physical-report/run-state.json').read_text())
            proc = Path('/proc')/str(original['pid'])
            if original['status'].startswith('TERMINAL') and not proc.exists():
                break
            assert proc.exists(), 'Existing reporter disappeared; inspect without restarting'
            assert str(project) in (proc/'cmdline').read_bytes().replace(b'\0', b' ').decode()
            time.sleep(10)
        build = json.loads((project/'run-state.json').read_text())
        state.update(original_report=original, original_build=build)
        save()
        assert original['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
        assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
        assert all(a['returncode'] == 0 for a in [*build['stages'], *original['actions']])
        for pid in [build['builder_pid'], original['pid'], *[a['pid'] for a in build['stages']], *[a['pid'] for a in original['actions']]]:
            assert not (Path('/proc')/str(pid)).exists(), pid
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest, name
        command = [str(quartus/'quartus_sta'), '-t', str(script)]
        with (out/'audit.log').open('x') as log:
            p = subprocess.Popen(command, cwd=project/'core', stdout=log, stderr=subprocess.STDOUT)
            row = dict(command=command, pid=p.pid, started=time.time())
            state['actions'].append(row)
            state['status'] = 'RUNNING_READ_ONLY_TAG_BUFFER_QUERY'
            save()
            row.update(returncode=p.wait(), finished=time.time())
            save()
        assert row['returncode'] == 0
        for name, digest in m['source_hashes'].items():
            assert sha(project/name) == digest, name
        text = (out/'audit.log').read_text()
        records = [line.split('\t') for line in text.splitlines() if line.startswith('TAG_')]
        owners = [r for r in records if r[0] == 'TAG_OWNER_OUTPUT']
        critical = [r for r in owners if ':u_d15|qb' in r[1]]
        assert critical and all(int(r[3]) > 0 for r in critical)
        nets = [r for r in records if r[0] == 'TAG_NET']
        cells = [r for r in records if r[0] == 'TAG_CELL']
        result = dict(status='TERMINAL_TAG_BUFFER_TOPOLOGY_REVIEW_REQUIRED', owners=owners,
            critical_owner_outputs=critical, named_cells=cells, named_nets=nets,
            named_cell_count=len(cells), named_net_count=len(nets),
            distinct_net_driver_pins=len({r[2] for r in nets}),
            direct_routing_fanout_at_output_pins=True, RTL_instances=m['cache_tag_lcell_buffers']['instances'],
            physical_replication_qualified=False, native_timing_qualified=False, assembly_permitted=False)
        (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
        state.update(status=result['status'], finished=time.time(), log_sha256=sha(out/'audit.log'))
        save()
    except BaseException:
        state.update(status='TERMINAL_TAG_BUFFER_QUERY_FAILED', finished=time.time())
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
