#!/usr/bin/env python3
"""Freeze and test optional HPS write windows, including byte-damaging controls.

This is a component gate, not native timing, Cedar or speed qualification.
The fixture is read only; all controller writes affect the test's RAM copy.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
FILES = ['fpga/mister/dorado_pack_hps.sv',
         'verilog/verilator/tb_pack_write_window.sv',
         'verilog/verilator/pack_write_window_test.cpp',
         'verilog/verilator/hps_disk_test.h']


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for data in iter(lambda: stream.read(65536), b''):
            h.update(data)
    return h.hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run', type=Path, required=True)
    p.add_argument('--fixture', type=Path, default=ROOT/'build/rtl-executive/pilot-pack-fixture.pack')
    a = p.parse_args()
    run, fixture = a.run.resolve(), a.fixture.resolve()
    run.mkdir(parents=True, exist_ok=False)
    inputs = run/'inputs'
    for name in FILES + ['tools/test_pack_write_window.py']:
        target = inputs/name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT/name, target)
    state = dict(status='RUNNING', started=time.time(), fixture=str(fixture),
                 fixture_sha256=sha(fixture), inputs={n: sha(inputs/n) for n in FILES},
                 actions=[], native_timing_qualified=False, hardware_speedup_qualified=False)

    def save():
        temp = run/'run-state.json.tmp'
        temp.write_text(json.dumps(state, indent=2)+'\n')
        temp.replace(run/'run-state.json')

    def invoke(name, argv, diagnostic=None):
        log = run/(name+'.log')
        action = dict(name=name, argv=[str(x) for x in argv], started=time.time())
        state['actions'].append(action)
        save()
        with log.open('wb') as out:
            child = subprocess.Popen(action['argv'], cwd=ROOT, stdout=out, stderr=subprocess.STDOUT)
            action['pid'] = child.pid
            save()
            action['returncode'] = child.wait()
        action['finished'] = time.time()
        text = log.read_text(errors='replace')
        if diagnostic:
            assert action['returncode'] != 0 and diagnostic in text, (name, text[-2000:])
            action['rejected'] = True
        else:
            assert action['returncode'] == 0, (name, text[-2000:])
        save()

    def build(name, window, rtl):
        obj = run/name
        invoke('compile-'+name, ['verilator', '--cc', '--exe', '--build', '-j', '4',
            '--top-module', 'tb_pack_write_window', '-Wno-fatal', '-GWINDOW='+str(window),
            '--Mdir', obj, '-o', 'pack-write-window-test', '-CFLAGS', '-O2 -std=c++20',
            inputs/FILES[1], inputs/FILES[2], rtl])
        return obj/'pack-write-window-test'

    try:
        save()
        for window in [0, 1]:
            exe = build('window'+str(window), window, inputs/FILES[0])
            for case in ['normal', 'readonly', 'late-readonly', 'remount', 'timeout']:
                invoke('window%d-%s' % (window, case), [exe, fixture, str(window), case])
            if window == 0:
                invoke('whole-track-cannot-claim-window', [exe, fixture, '1', 'normal'],
                       'wrong write window LBA/count')
        rtl = (inputs/FILES[0]).read_text()
        controls = [
            ('wrong-lba', "cached_lba + ((DIRTY_BLOCK_WINDOW!=0) ? {27'd0,dirty_first_block} : 32'd0)",
             'cached_lba', 'writeback lost changed words'),
            ('wrong-buffer-base', 'host_write_base : 13\'d0', "13'd0 : 13'd0",
             'writeback lost changed words'),
            ('lost-coalescing', 'generation == flushed_generation ||', "1'b1 ||",
             'later write lost changed bytes'),
            ('write-offset-on-read', '(host_state==1 || host_state==2)', "1'b1",
             'new track serial payload differs'),
        ]
        for name, before, after, diagnostic in controls:
            assert before in rtl, name
            source = run/(name+'.sv')
            source.write_text(rtl.replace(before, after))
            exe = build(name, 1, source)
            invoke(name+'-reject', [exe, fixture, '1', 'normal'], diagnostic)
        assert sha(fixture) == state['fixture_sha256'], 'source medium changed'
        assert all(sha(inputs/n) == state['inputs'][n] for n in FILES)
        state.update(status='PASS_COMPONENT_ONLY', finished=time.time(),
                     scenarios=10, rejected_controls=5,
                     limits='No native timing/FPGA boot qualification; production default remains whole-track')
        save()
        print('PACK-WRITE-WINDOW PASS: 10 scenarios, 5 old/range/LBA/buffer/read-port controls rejected', flush=True)
    except BaseException:
        state.update(status='FAIL', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    main()
