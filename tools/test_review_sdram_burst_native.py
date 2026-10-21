#!/usr/bin/env python3
"""Reject altered BL8 native source, RAM, pin/path and register evidence."""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]


def read(path):
    return gzip.decompress(path.read_bytes()) if path.suffix == '.gz' else path.read_bytes()


def replace(path, before, after):
    raw = read(path)
    assert raw.count(before) >= 1, (path, before)
    raw = raw.replace(before, after, 1)
    encoded = gzip.compress(raw, mtime=0) if path.suffix == '.gz' else raw
    # The fixture uses hard links; replace the inode, never modify its source.
    tmp = path.with_name(path.name + '.control-tmp')
    tmp.write_bytes(encoded)
    os.replace(tmp, path)


def run(args):
    out = args.output.resolve()
    assert not out.exists()
    out.mkdir(parents=True)
    original = args.root.resolve()
    manifest_hash = hashlib.sha256((original / 'archive-manifest.json.gz').read_bytes()).hexdigest()
    cases = []
    def exercise(label, change, error='AssertionError'):
        directory = out / label
        shutil.copytree(original, directory, copy_function=os.link)
        change(directory)
        command = [sys.executable, str(ROOT / 'tools/review_sdram_burst_native.py'),
                   '--root', str(directory), '--parent', str(args.parent.resolve()),
                   '--parent-qualification', str(args.parent_qualification.resolve()),
                   '--parent-reports', str(args.parent_reports.resolve()),
                   '--output', str(directory / 'must-not-qualify.json')]
        if args.observer_component:
            command += ['--observer-component', str(args.observer_component.resolve())]
        if args.selftest_component:
            command += ['--selftest-component', str(args.selftest_component.resolve())]
        if args.selftest_full_component:
            command += ['--selftest-full-component', str(args.selftest_full_component.resolve())]
        if args.burst_observer_component:
            command += ['--burst-observer-component', str(args.burst_observer_component.resolve())]
        if args.burst_observer_full_component:
            command += ['--burst-observer-full-component', str(args.burst_observer_full_component.resolve())]
        if args.cas_reference and label!='missing-cas-reference':command += ['--cas-reference',str(args.cas_reference.resolve())]
        if args.read_capture_matrix:command += ['--read-capture-matrix',str(args.read_capture_matrix.resolve())]
        if args.read_capture_board:command += ['--read-capture-board',str(args.read_capture_board.resolve())]
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
            row = dict(case=label, pid=p.pid, command=command, started=time.time())
            code = p.wait()
        row.update(returncode=code, finished=time.time(), rejected=code != 0)
        assert code != 0 and not (directory / 'must-not-qualify.json').exists(), label
        assert error in (out / (label + '.log')).read_text(), label
        cases.append(row)
        print(label, 'rejected', flush=True)
    def relaxed_clock(directory):
        path = directory / 'core/Dorado.sdc.gz'
        raw = read(path) + b'\ncreate_clock -name fake -period 1000 [get_ports FPGA_CLK1_50]\n'
        temp = path.with_name('relaxed-clock.tmp')
        temp.write_bytes(gzip.compress(raw, mtime=0))
        os.replace(temp, path)
        manifest = directory / 'sdram-burst-native.json.gz'
        record = json.loads(read(manifest))
        record['source_hashes']['core/Dorado.sdc'] = hashlib.sha256(raw).hexdigest()
        temp = manifest.with_name('rehash.tmp')
        temp.write_bytes(gzip.compress(json.dumps(record).encode(), mtime=0))
        os.replace(temp, manifest)
    exercise('relaxed-clock-rehashed', relaxed_clock)
    exercise('cache-data-registers-missing', lambda p: replace(p / 'burst-review-v2.log.gz', b'words 384', b'words 383'))
    map_path = Path('core/output_files/Dorado.map.rpt.gz')
    mapping = read(original / map_path)
    table = mapping[mapping.index(b'; Analysis & Synthesis Source Files Read'):].split(b'\n\n', 1)[0]
    entry = [line for line in table.splitlines() if b'dorado_storage_sdram_burst.sv' in line]
    assert len(entry) == 1 and entry[0].count(b'; yes ') == 1
    exercise('backend-not-used-in-netlist', lambda p: replace(p / map_path, entry[0], entry[0].replace(b'; yes ', b'; no  ', 1)))
    fit_path = Path('core/output_files/Dorado.fit.rpt.gz')
    fitted = read(original / fit_path)
    table = fitted[fitted.index(b'; Fitter RAM Summary'):].split(b'\n\n', 1)[0]
    entry_ram = [line for line in table.splitlines() if b'|dorado_snapshot_f10415_enable_bank:snapshot_bank_0|' in line]
    assert len(entry_ram) == 1 and b'; 18432 ' in entry_ram[0]
    exercise('RAM-geometry-changed', lambda p: replace(p / fit_path, entry_ram[0], entry_ram[0].replace(b'; 18432 ', b'; 18431 ', 1)))
    paths = sorted((original / 'core').glob('burst-input-capture-setup-*.rpt.gz'))
    assert len(paths) == 4
    name = str(paths[0].relative_to(original))
    text = read(original / name)
    value = re.search(rb'Worst case slack is\s+([-+\d.]+)', text)
    assert value
    exercise('negative-new-capture-setup', lambda p: replace(p / name, value[0], b'Worst case slack is -0.001'))
    old = sorted((original / 'core').glob('native59-sdram-input-hold-*.rpt.gz'))[0]
    name_old = str(old.relative_to(original))
    value_old = re.search(rb'Worst case slack is\s+([-+\d.]+)', read(old))
    assert value_old
    exercise('negative-original-SDRAM-hold', lambda p: replace(p / name_old, value_old[0], b'Worst case slack is -0.001'))
    exercise('missing-new-timing-corner', lambda p: (p / name).unlink(), 'FileNotFoundError')
    if args.observer_component:
        entry_observer = [line for line in mapping[mapping.index(b'; Analysis & Synthesis Source Files Read'):].split(b'\n\n', 1)[0].splitlines() if b'dorado_boot_fault_monitor.sv' in line]
        assert len(entry_observer) == 1 and b'; yes ' in entry_observer[0]
        exercise('observer-not-used-in-netlist', lambda p: replace(p / map_path, entry_observer[0], entry_observer[0].replace(b'; yes ', b'; no  ', 1)))
        def swapped_pins(directory):
            name = 'src/fpga/mister/snapshot_native/ContB_full_snapshot.sv'
            path = directory / (name + '.gz')
            replace(path, b'IOPE,RamPE,MemPE,MdPE,IMLHPEenable_p_', b'RamPE,IOPE,MemPE,MdPE,IMLHPEenable_p_')
            path_manifest = directory / 'sdram-burst-native.json.gz'
            r = json.loads(read(path_manifest))
            r['source_hashes'][name] = hashlib.sha256(read(path)).hexdigest()
            tmp = path_manifest.with_name('rehashed-observer.tmp')
            tmp.write_bytes(gzip.compress(json.dumps(r).encode(), mtime=0))
            os.replace(tmp, path_manifest)
        exercise('observer-pins-swapped-rehashed', swapped_pins)
        faults = sorted((original / 'core').glob('burst-first-fault-hold-*.rpt.gz'))
        assert len(faults) == 4
        fault_name = str(faults[0].relative_to(original))
        exercise('missing-observer-timing-corner', lambda p: (p / fault_name).unlink(), 'FileNotFoundError')
    if args.selftest_component:
        entry_selftest = [line for line in mapping[mapping.index(b'; Analysis & Synthesis Source Files Read'):].split(b'\n\n', 1)[0].splitlines() if b'dorado_storage_selftest.sv' in line]
        assert len(entry_selftest) == 1 and b'; yes ' in entry_selftest[0]
        exercise('selftest-not-used-in-netlist', lambda p: replace(p / map_path, entry_selftest[0], entry_selftest[0].replace(b'; yes ', b'; no  ', 1)))
        def bypass_selftest(directory):
            name = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
            path = directory / (name + '.gz')
            replace(path, b'assign storage_ready=storage_initialized && board_ready;', b'assign storage_ready=board_ready;')
            manifest = directory / 'sdram-burst-native.json.gz'
            record = json.loads(read(manifest))
            record['source_hashes'][name] = hashlib.sha256(read(path)).hexdigest()
            tmp = manifest.with_name('rehashed-selftest.tmp')
            tmp.write_bytes(gzip.compress(json.dumps(record).encode(), mtime=0))
            os.replace(tmp, manifest)
        exercise('selftest-release-bypassed-rehashed', bypass_selftest)
        reports = sorted((original / 'core').glob('burst-selftest-input-hold-*.rpt.gz'))
        assert len(reports) == 4
        name_selftest = str(reports[0].relative_to(original))
        exercise('missing-selftest-timing-corner', lambda p: (p / name_selftest).unlink(), 'FileNotFoundError')
    if args.burst_observer_component:
        text = read(original / 'burst-review-v2.log.gz')
        match = re.search(rb'BURST_TRACE_COUNT (\S+) (\d+)', text)
        assert match
        replacement = b'BURST_TRACE_COUNT '+match[1]+b' '+str(int(match[2])-1).encode()
        exercise('burst-trace-register-count-wrong', lambda p: replace(p / 'burst-review-v2.log.gz', match[0], replacement))
        def swapped_trace(directory):
            name = 'core/Dorado.sv';path = directory / (name + '.gz')
            replace(path, b'storage_burst_observation[32*status[12:11]+:32]', b'storage_burst_observation[32*(3-status[12:11])+:32]')
            manifest = directory / 'sdram-burst-native.json.gz';record = json.loads(read(manifest))
            record['source_hashes'][name] = hashlib.sha256(read(path)).hexdigest()
            tmp = manifest.with_name('rehashed-trace.tmp');tmp.write_bytes(gzip.compress(json.dumps(record).encode(),mtime=0));os.replace(tmp,manifest)
        exercise('burst-trace-wires-swapped-rehashed', swapped_trace)
        reports = sorted((original / 'core').glob('burst-trace-capture-hold-*.rpt.gz'))
        assert len(reports) == 4
        trace_report = str(reports[0].relative_to(original))
        exercise('missing-burst-trace-corner', lambda p: (p / trace_report).unlink(), 'FileNotFoundError')
    if args.cas_reference:
        def reverted_capture(directory):
            name='src/fpga/mister/dorado_storage_sdram_burst.sv';path=directory/(name+'.gz')
            replace(path,b'delay_count<=2;state<=READ_WAIT;',b'delay_count<=3;state<=READ_WAIT;')
            manifest=directory/'sdram-burst-native.json.gz';record=json.loads(read(manifest));record['source_hashes'][name]=hashlib.sha256(read(path)).hexdigest()
            tmp=manifest.with_name('rehashed-read-capture.tmp');tmp.write_bytes(gzip.compress(json.dumps(record).encode(),mtime=0));os.replace(tmp,manifest)
        exercise('read-capture-reverted-rehashed',reverted_capture)
        exercise('missing-cas-reference',lambda p:None)
    assert hashlib.sha256((original / 'archive-manifest.json.gz').read_bytes()).hexdigest() == manifest_hash
    for row in json.loads(gzip.decompress((original / 'archive-manifest.json.gz').read_bytes())):
        data = read(original / row['stored'])
        assert hashlib.sha256(data).hexdigest() == row['raw_sha256'] and len(data) == row['raw_bytes']
    result = dict(status='PASS_NATIVE_BURST_REVIEW_REJECTION_CONTROLS', cases=cases,
                  source_archive_manifest_sha256=manifest_hash, reviewer_sha256=hashlib.sha256((ROOT / 'tools/review_sdram_burst_native.py').read_bytes()).hexdigest())
    (out / 'controls.json').write_text(json.dumps(result, indent=2) + '\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'parent', 'parent-qualification', 'parent-reports', 'output']:
        p.add_argument('--' + name, type=Path, required=True)
    p.add_argument('--observer-component', type=Path)
    p.add_argument('--selftest-component', type=Path)
    p.add_argument('--selftest-full-component', type=Path)
    p.add_argument('--burst-observer-component', type=Path)
    p.add_argument('--burst-observer-full-component', type=Path)
    p.add_argument('--cas-reference',type=Path)
    p.add_argument('--read-capture-matrix',type=Path)
    p.add_argument('--read-capture-board',type=Path)
    run(p.parse_args())
