#!/usr/bin/env python3
"""Reject damaged timing, phase, memory and consistently rehashed sources."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

from review_pack_window_native import review
from test_review_storage_board_native import dependencies

ROOT = Path(__file__).resolve().parents[1]
CASES = ('missing-original-IFU', 'negative-SDRAM-output', 'missing-sample-input-hold',
         'wrong-half-cycle-relationship', 'missing-sample-bit', 'rehashed-wrong-sample-edge',
         'rehashed-disabled-window', 'rehashed-weakened-clock', 'wrong-cache-old-data',
         'cache-bank-alias', 'snapshot-enabled-in-map', 'missing-real-trace-bit',
         'failed-fit-state', 'new-unreviewed-warning', 'wrong-command-encoding',
         'missing-ready-corner')


def sha(b):
    return hashlib.sha256(b).hexdigest()


def run(root, recipe, qualified, output):
    output.mkdir(parents=True, exist_ok=False)
    expected = json.loads(qualified.read_text())
    assert json.loads(json.dumps(review(root, recipe))) == expected
    before = {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}

    def write(p, b):
        temp = p.with_name(p.name + '.mutation-tmp')
        temp.write_bytes(b)
        os.replace(temp, p)

    def replace(p, old, new):
        b = p.read_bytes()
        assert b.count(old) == 1, (p, old, b.count(old))
        write(p, b.replace(old, new))

    def source(p, name, old, new):
        replace(p / name, old, new)
        # Keep both hash books consistent: the recipe must catch this defect.
        for file, key in [('pack-window-native.json', 'source_hashes'),
                          ('collected-ready-input-check.json', 'actual')]:
            f = p / file
            r = json.loads(f.read_bytes())
            r[key][name] = sha((p / name).read_bytes())
            write(f, json.dumps(r).encode())

    def negative(p, name):
        f = p / name
        m = re.search(rb'Worst case slack is\s+([-+\d.]+)', f.read_bytes())
        assert m
        replace(f, m[0], b'Worst case slack is -0.001')

    corner = '7_slow_1100mv_100c'
    mapfile, fitfile = 'core/output_files/Dorado.map.rpt', 'core/output_files/Dorado.fit.rpt'

    def phase(p):
        f = p / f'core/storage-sample-output-setup-{corner}.rpt'
        rows = [s for s in f.read_bytes().splitlines()
                if len(s.split(b';')[1:-1]) == 8 and b'; 8.750 ' in s]
        assert len(rows) == 256
        replace(f, rows[0], rows[0].replace(b'; 8.750 ', b'; 17.500 ', 1))

    def missing_sample(p):
        f = p / 'storage-ready-reports-v1/ready.log'
        m = re.search(rb'^STORAGE_SAMPLE_REGISTER .+\|addr_before_host\[0\]\n', f.read_bytes(), re.M)
        assert m
        assert f.read_bytes().count(m[0]) == 4
        write(f, f.read_bytes().replace(m[0], b'', 1))

    def cache_row(p, bank, transform):
        f = p / fitfile
        rows = [s for s in f.read_bytes().splitlines() if b':b_MemD|' in s and
                f':snapshot_bank_{bank}|'.encode() in s and b'; Old data' in s]
        assert len(rows) == 1
        replace(f, rows[0], transform(rows[0]))

    def alias(p):
        rows = [s for s in (p / fitfile).read_bytes().splitlines() if b':b_MemD|' in s and b':snapshot_bank_3|' in s and b'; Old data' in s]
        assert len(rows) == 1
        sites = re.findall(rb'M10K_X\d+_Y\d+_N\d+', rows[0])
        assert len(sites) == 2
        def transform(row):
            previous = re.findall(rb'M10K_X\d+_Y\d+_N\d+', row)
            assert len(previous) == 2
            for a, b in zip(previous, sites):
                row = row.replace(a, b)
            return row
        cache_row(p, 8, transform)

    def snapshot(p):
        f = p / mapfile
        m = re.search(rb'; SNAPSHOT_PAUSE\s*; 0\s*;', f.read_bytes())
        assert m
        replace(f, m[0], m[0].replace(b'; 0 ', b'; 1 ', 1))

    def trace(p):
        f = p / 'storage-ready-reports-v1/inventory.log'
        m = re.search(rb'^SELFTEST_FIT_REGISTER trace .*\|first_burst_words\[0\]\n', f.read_bytes(), re.M)
        count = re.search(rb'^SELFTEST_FIT_COUNT trace (\d+)$', f.read_bytes(), re.M)
        assert m and count
        replace(f, m[0], b'')
        replace(f, count[0], b'SELFTEST_FIT_COUNT trace ' + str(int(count[1]) - 1).encode())

    def failed(p):
        f = p / 'run-state.json';r = json.loads(f.read_bytes())
        r['stages'][1]['returncode'] = 1
        write(f, json.dumps(r).encode())

    def encoding(p):
        f = p / mapfile
        m = re.search(rb'; command\.READ\s*; 1\s*; 0\s*; 1\s*;', f.read_bytes())
        assert m
        replace(f, m[0], m[0].replace(b'; 0 ', b'; 1 ', 1))

    mutations = [
        lambda p: (p / f'core/native59-ifu-in-setup-{corner}.rpt').unlink(),
        lambda p: negative(p, f'core/native59-sdram-output-setup-{corner}.rpt'),
        lambda p: (p / f'core/storage-sample-input-hold-{corner}.rpt').unlink(),
        phase, missing_sample,
        lambda p: source(p, 'src/fpga/mister/dorado_storage_group.sv', b'@(negedge host_clk)addr_before_host', b'@(posedge host_clk)addr_before_host'),
        lambda p: source(p, 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv', b'.DIRTY_BLOCK_WINDOW(1)', b'.DIRTY_BLOCK_WINDOW(0)'),
        lambda p: source(p, 'core/Dorado.qsf', b'DORADO_HOST_CLOCK_HZ=57142857', b'DORADO_HOST_CLOCK_HZ=50000000'),
        lambda p: cache_row(p, 3, lambda row: row.replace(b'Old data', b'New data')),
        alias, snapshot, trace, failed,
        lambda p: write(p / 'fit.log', (p / 'fit.log').read_bytes() + b'\nWarning (999999): unreviewed warning\n'),
        encoding,
        lambda p: (p / f'core/native-ready2-step-hold-{corner}.rpt').unlink(),
    ]
    cases = []
    for name, mutate in zip(CASES, mutations):
        with tempfile.TemporaryDirectory(prefix='dorado-pack-review-bad-') as d:
            p = Path(d) / 'receipt'
            shutil.copytree(root, p, copy_function=os.link)
            mutate(p)
            command = [sys.executable, str(ROOT / 'tools/review_pack_window_native.py'), '--root', str(p),
                       '--recipe', str(recipe), '--output', str(Path(d) / 'bad.json')]
            result = subprocess.run(command, capture_output=True, text=True)
            text = result.stdout + result.stderr
            (output / (name + '.log')).write_text(text)
            assert result.returncode != 0 and ('AssertionError' in result.stderr or 'FileNotFoundError' in result.stderr), (name, text[-1000:])
            cases.append(dict(name=name, returncode=result.returncode, rejected=True, log_sha256=sha(text.encode())))
            print(name, 'rejected', flush=True)
    assert json.loads(json.dumps(review(root, recipe))) == expected
    assert before == {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}
    r = dict(status='PASS_NATIVE_PACK_WINDOW_REVIEW_REJECTION_CONTROLS', cases=cases,
             positive_qualification_sha256=sha(qualified.read_bytes()), recipe_config_sha256=sha(recipe.read_bytes()),
             reviewer_dependencies=dependencies(), original_evidence_unchanged=True)
    (output / 'controls.json').write_text(json.dumps(r, indent=2) + '\n')
    print(r['status'], len(cases))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root', 'recipe', 'qualified', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.root.resolve(), a.recipe.resolve(), a.qualified.resolve(), a.output.resolve())
