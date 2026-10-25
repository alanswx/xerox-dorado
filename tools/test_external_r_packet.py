#!/usr/bin/env python3
"""Replay the complete qualified packet fixture with caller-owned SRAM.

All original assertions, 14884 records, 2470 result bits, 844 owners, 9216 SRAM
bits, 100 clocks, 403 rejection cases and cache/forwarding coverage remain.
Only SRAM hierarchy and wires move. Native clock/source and staged fallback
integration remain open. Keep the source trace and every failed experiment.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from generate_external_r_packet import packet, caller, bench


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(source, out, engine, mutation):
    assert not out.exists()
    parent = json.loads((source/'run-state.json').read_text())
    assert parent['status'] == 'PASS_OWNED_FULL_PROCESSOR_PACKETS'
    assert parent['all_original_assertions_retained']
    for name, digest in parent['files'].items():
        assert sha(source/name) == digest, name
    for row in parent['source_files']:
        assert sha(source/row['path']) == row['output_sha256']
    assert sha(Path(parent['trace_links']['trace'])) == parent['trace_links']['trace_sha256']
    assert parent['trace_links']['records'] == 14884
    raw = (source/'packet.sv').read_text()
    transformed = packet(raw)
    if mutation == 'drop-forward':
        old = '.external_file_read(later_read)'
        assert transformed.count(old) == 1
        transformed = transformed.replace(old, '.external_file_read(original_read)', 1)
    outputs = {'packet.sv': transformed,
               'caller.sv': caller((source/'caller.sv').read_text(), mutation),
               'tb.sv': bench((source/'tb.sv').read_text())}
    out.mkdir(parents=True)
    paths = []
    for row in parent['source_files']:
        path = out/row['path'];path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source/row['path'], path);paths.append(path)
    for name, text in outputs.items():
        path = out/name;path.write_text(text);paths.append(path)
    shutil.copy2(source/'forward.sv', out/'forward.sv');paths.append(out/'forward.sv')
    for name in [Path(__file__).name, 'generate_external_r_packet.py']:
        shutil.copy2(Path(__file__).with_name(name), out/name)
    # The frozen source set also carries an unused dorado_r_memory_step
    # library module with its own files instance. Count the selected caller
    # and packet here; the elaborated hierarchy is checked separately.
    assert sum(outputs[n].count('dorado_r_file_ports files(') for n in ['packet.sv','caller.sv']) == 1
    state = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                 source=str(source), source_state_sha256=sha(source/'run-state.json'),
                 scope=__doc__, engine=engine, mutation=mutation,
                 files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},
                 source_files=[dict(path=str(p.relative_to(out)), output_sha256=sha(p)) for p in paths],
                 actions=[], all_original_assertions_retained=True,
                 selected_caller_SRAM_instances=1, physical_SRAM_bits=9216,
                 stored_owner_bits=828, external_visible_R_bits=16,
                 staged_fallback_integrated=False, native_integrated=False, hardware_changed=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')
    commands = ([['iverilog','-g2012','-s','tb_r_front_packet','-o',str(out/'test.vvp'),*map(str,paths)],
                 ['vvp','-i',str(out/'test.vvp')]] if engine == 'icarus' else
                [['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_packet',
                  '--Mdir',str(out/'obj'),'-o','test',*map(str,paths)], [str(out/'obj/test')]])
    save()
    code = None
    for label, cmd in zip(['compile','test'], commands):
        with (out/(label+'.log')).open('x') as log:
            child = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, command=cmd, pid=child.pid, started=time.time())
            state['actions'].append(row);save();code = child.wait()
        row.update(returncode=code, finished=time.time());save()
        if code:
            break
    log_path = out/'test.log';log = log_path.read_text() if log_path.exists() else ''
    if mutation:
        passed = state['actions'][0]['returncode'] == 0 and code != 0 and bool(re.search(
            r'PACKET (?:first|second|hold|committed|invalid geometry committed)', log))
        state['failure_lines'] = [v for v in log.splitlines() if 'FATAL' in v or 'PACKET_DIFF' in v][:40]
    else:
        m = re.search(r'^FULL_PROCESSOR_PACKET_PASS (.+)$', log, re.M)
        coverage = re.search(r'^CACHED_PACKET_COVERAGE (.+)$', log, re.M)
        clocks = re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
        original_clocks = re.findall(r'^PACKET_CLOCK channel=(\d+) edges=(\d+)$', (source/'test.log').read_text(), re.M)
        passed = (code == 0 and m is not None and m[1] == parent['summary'] and
                  coverage is not None and coverage[1] == parent['cache_coverage'] and
                  len(clocks) == 100 and clocks == original_clocks and
                  'PACKET_GEOMETRY_REJECTIONS cases=403' in log)
        if passed:
            state.update(summary=m[1], cache_coverage=coverage[1], all100_clock_counts_match=True)
    state.update(status=('PASS_EXTERNAL_PACKET_COUNTEREXAMPLE' if mutation else 'PASS_EXTERNAL_FULL_PROCESSOR_PACKETS')
                 if passed else 'TERMINAL_EXTERNAL_PACKET_NOT_ESTABLISHED', finished=time.time(),
                 test_log_sha256=sha(log_path) if log_path.exists() else None)
    save();print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus','verilator'], required=True)
    p.add_argument('--mutation', choices=['early-write','drop-forward'])
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.output.resolve(), a.engine, a.mutation))
