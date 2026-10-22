#!/usr/bin/env python3
"""Compare existing cache capacities using exact qualified native SDRAM RTL.

This is a physical-pin component workload, not measured FPGA CPU performance.
No production RTL, wrapper, clock, Main, receiver or running core is changed.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(stage, qualification, out):
    assert not out.exists(), 'Observe the existing recorded run'
    q = json.loads(qualification.read_text())
    assert q['status'] == 'PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW'
    assert q['frequency_hz'] == 57142857 and q['sysper'] == 4
    names = ['src/fpga/mister/dorado_storage_sdram_burst.sv',
             'src/fpga/mister/dorado_storage_sdram.sv']
    for n in names:
        assert sha(stage / n) == q['source_hashes'][n], n
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    files = {}
    for n in names:
        p = inputs / Path(n).name
        shutil.copyfile(stage / n, p)
        files[p.name] = sha(p)
    for n in ['verilog/verilator/tb_storage_sdram_burst.sv',
              'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv']:
        p = inputs / Path(n).name
        shutil.copyfile(ROOT / n, p)
        files[p.name] = sha(p)
    anchor = '        // Pin-mode performance comparison: same requests and response holds.\n'
    tb = inputs / 'tb_storage_sdram_burst.sv'
    text = tb.read_text()
    assert text.count(anchor) == 1
    # Real native mapping interleaves128 groups in the low seven bits. One
    # BL8 covers8 adjacent groups; sixteen lines cover one word in all groups.
    sweep = '''        // Geometry-derived capacity probe, before the unchanged coherence suite.
        for(int cluster=0;cluster<16;cluster++)seed_line(a0+32'(8*cluster));
        before_reads=physical_reads;before_ticks=host_ticks;
        for(int pass=0;pass<4;pass++)
            for(int group_id=0;group_id<128;group_id++)
                transfer(0,a0+32'(group_id),0);
        $display("CACHE_GROUP_SWEEP lines=%0d requests=512 physical_reads=%0d host_ticks=%0d",CACHE_LINES,physical_reads-before_reads,host_ticks-before_ticks);
'''
    patched = inputs / 'tb_capacity.sv'
    patched.write_text(text.replace(anchor, sweep + anchor))
    files[patched.name] = sha(patched)
    shutil.copyfile(Path(__file__), inputs / Path(__file__).name)
    files[Path(__file__).name] = sha(inputs / Path(__file__).name)
    provenance = dict(scope=__doc__, qualification_sha256=sha(qualification),
                      qualified_source_hashes={n: q['source_hashes'][n] for n in names},
                      frozen_inputs=files, host_clock_hz=57142857,
                      raw_native_mapping='(group_word_address <<7) | group_id',
                      RTL_or_core_changed=False, FPGA_speed_qualified=False)
    (out / 'provenance.json.gz').write_bytes(gzip.compress(
        (json.dumps(provenance, indent=2) + '\n').encode(), mtime=0))
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[])
    results = {}

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, cwd=out)
            row = dict(stage=label, pid=p.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            code = p.wait()
            row.update(returncode=code, finished=time.time())
            save()
            assert code == 0, (label, code)

    save()
    try:
        for lines in [4, 16]:
            label = 'lines%d' % lines
            obj = out / ('obj-' + label)
            execute(['verilator', '--binary', '--timing', '--autoflush', '-j', '4',
                     '+define+DORADO_CORRECT_READ_CAPTURE', '--top-module', 'tb_storage_sdram_burst',
                     '-Wno-fatal', '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND',
                     '-Wno-REALCVT', '-Wno-PINMISSING', '-Wno-DEFPARAM',
                     '-GCLOCK_HZ=57142857', '-GCACHE_LINES=' + str(lines),
                     '--Mdir', str(obj), '-o', 'test-capacity',
                     str(inputs / 'dorado_storage_sdram.sv'),
                     str(inputs / 'dorado_storage_sdram_burst.sv'), str(patched),
                     str(inputs / 'xsds_128mbyte_sdram_model.sv')], label + '-compile')
            execute([str(obj / 'test-capacity')], label + '-test')
            log = (out / (label + '-test.log')).read_text()
            assert 'STORAGE-SDRAM-BURST PASS use_burst=1 lines=%d' % lines in log
            m = re.search(r'CACHE_GROUP_SWEEP lines=(\d+) requests=512 physical_reads=(\d+) host_ticks=(\d+)', log)
            assert m and int(m[1]) == lines
            results[str(lines)] = dict(requests=512, physical_reads=int(m[2]), host_ticks=int(m[3]))
            print(label, results[str(lines)], flush=True)
        assert results['4']['physical_reads'] == 64 and results['16']['physical_reads'] == 16
        assert results['16']['host_ticks'] < results['4']['host_ticks']
        assert all(sha(inputs / n) == h for n, h in files.items())
        result = dict(status='PASS_NATIVE_SDRAM_CAPACITY_COMPONENT', results=results,
                      physical_read_reduction=1 - results['16']['physical_reads'] / results['4']['physical_reads'],
                      component_host_tick_reduction=1 - results['16']['host_ticks'] / results['4']['host_ticks'],
                      scope=__doc__, native_instruction_speed_qualified=False,
                      original_coherence_reset_refresh_backpressure_checks_pass=True)
        (out / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
        state.update(status=result['status'], finished=time.time())
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['stage', 'qualification', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.stage.resolve(), a.qualification.resolve(), a.output.resolve())
