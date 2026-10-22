#!/usr/bin/env python3
"""Exercise actual damaged-evidence/source variants against the native reviewer."""
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

from review_storage_board_native import review

ROOT = Path(__file__).resolve().parents[1]
CASES = ('missing-ready-corner', 'negative-SDRAM-output-setup', 'missing-original-IFU',
         'rehashed-weakened-clock', 'rehashed-late-capture', 'preview-not-used',
         'wrong-derived-clock', 'wrong-actual-timeout', 'wrong-command-encoding',
         'wrong-cache-old-data', 'cache-pair-alias', 'cache-wrong-block-count',
         'missing-ready-register', 'missing-real-trace-bit', 'failed-fit-state',
         'new-unreviewed-warning', 'negative-first-fault', 'rehashed-disconnected-step')


def sha(b):
    return hashlib.sha256(b).hexdigest()


def dependencies():
    result = {}
    for m in list(sys.modules.values()):
        f = getattr(m, '__file__', None)
        if f:
            p = Path(f).resolve()
            if p.suffix == '.py' and p.is_relative_to(ROOT):
                result[str(p.relative_to(ROOT))] = sha(p.read_bytes())
    return result


def run(root, recipe, qualified, output):
    assert not output.exists()
    output.mkdir(parents=True)
    expected = json.loads(qualified.read_text())
    assert json.loads(json.dumps(review(root, recipe))) == expected
    before = {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}
    cases = []

    def exercise(name, mutate):
        with tempfile.TemporaryDirectory(prefix='dorado-board-review-bad-') as temp:
            p = Path(temp) / 'receipt'
            # All modifications below atomically replace a file or unlink only
            # the private hardlink. Original evidence is verified again below.
            shutil.copytree(root, p, copy_function=os.link)
            mutate(p)
            command = [sys.executable, str(ROOT / 'tools/review_storage_board_native.py'),
                       '--root', str(p), '--recipe', str(recipe), '--output', str(Path(temp) / 'bad.json')]
            result = subprocess.run(command, capture_output=True, text=True)
            (output / (name + '.log')).write_text(result.stdout + result.stderr)
            assert result.returncode != 0 and ('AssertionError' in result.stderr or 'FileNotFoundError' in result.stderr), name
            cases.append(dict(name=name, returncode=result.returncode, rejected=True,
                              log_sha256=sha((result.stdout + result.stderr).encode())))
            print(name, 'rejected', flush=True)

    def write(p, b):
        q = p.with_name(p.name + '.mutation-tmp')
        q.write_bytes(b)
        os.replace(q, p)

    def replace(p, old, new):
        b = p.read_bytes()
        assert b.count(old) == 1, (p, old, b.count(old))
        write(p, b.replace(old, new))

    def source(p, name, old, new):
        replace(p / name, old, new)
        f = p / 'storage-board-native.json'
        r = json.loads(f.read_bytes())
        r['source_hashes'][name] = sha((p / name).read_bytes())
        write(f, (json.dumps(r, indent=2) + '\n').encode())

    def negative(p, name):
        f = p / name
        match = re.search(rb'Worst case slack is\s+([-+\d.]+)', f.read_bytes())
        assert match
        replace(f, match[0], b'Worst case slack is -0.001')

    corner = '7_slow_1100mv_100c'
    exercise(CASES[0], lambda p: (p / f'core/native-ready2-step-hold-{corner}.rpt').unlink())
    exercise(CASES[1], lambda p: negative(p, f'core/native59-sdram-output-setup-{corner}.rpt'))
    exercise(CASES[2], lambda p: (p / f'core/native59-ifu-in-setup-{corner}.rpt').unlink())
    exercise(CASES[3], lambda p: source(p, 'core/Dorado.qsf', b'DORADO_HOST_CLOCK_HZ=57142857', b'DORADO_HOST_CLOCK_HZ=50000000'))
    exercise(CASES[4], lambda p: source(p, 'src/fpga/mister/dorado_storage_sdram_burst.sv', b'delay_count<=2;state<=READ_WAIT;', b'delay_count<=3;state<=READ_WAIT;'))
    mapfile = 'core/output_files/Dorado.map.rpt'

    def unused(p):
        f = p / mapfile
        rows = [l for l in f.read_bytes().splitlines() if b'dorado_storage_addr_preview.sv' in l and b'; yes ' in l]
        assert len(rows) == 1
        replace(f, rows[0], rows[0].replace(b'; yes ', b'; no  ', 1))

    exercise(CASES[5], unused)

    def clock(p):
        f = p / 'core/native-clocks.rpt'
        rows = [l for l in f.read_bytes().splitlines() if l.startswith(b'; dorado_sdram_clk ')]
        assert len(rows) == 1
        replace(f, rows[0], rows[0].replace(b'17.500', b'20.000'))

    exercise(CASES[6], clock)

    def timeout(p):
        f = p / mapfile
        match = re.search(rb'; HOST_TIMEOUT_CLOCKS\s*; 228571428\s*;', f.read_bytes())
        assert match
        replace(f, match[0], match[0].replace(b'228571428', b'200000000'))

    exercise(CASES[7], timeout)

    def encoding(p):
        f = p / mapfile
        match = re.search(rb'; command\.READ\s*; 1\s*; 0\s*; 1\s*;', f.read_bytes())
        assert match
        replace(f, match[0], match[0].replace(b'; 0 ', b'; 1 ', 1))

    exercise(CASES[8], encoding)
    fitfile = 'core/output_files/Dorado.fit.rpt'

    def cache_row(p, bank, transform):
        f = p / fitfile
        rows = [l for l in f.read_bytes().splitlines() if b':b_MemD|' in l and
                f':snapshot_bank_{bank}|'.encode() in l and b'; Old data' in l]
        assert len(rows) == 1
        replace(f, rows[0], transform(rows[0]))

    exercise(CASES[9], lambda p: cache_row(p, 3, lambda r: r.replace(b'Old data', b'New data')))

    def alias(p):
        text = (p / fitfile).read_bytes()
        rows = [l for l in text.splitlines() if b':b_MemD|' in l and b':snapshot_bank_3|' in l and b'; Old data' in l]
        assert len(rows) == 1
        sites = re.findall(rb'M10K_X\d+_Y\d+_N\d+', rows[0])
        assert len(sites) == 3
        def transform(row):
            old = re.findall(rb'M10K_X\d+_Y\d+_N\d+', row)
            assert len(old) == 3
            for a, b in zip(old, sites):
                row = row.replace(a, b)
            return row
        cache_row(p, 8, transform)

    exercise(CASES[10], alias)

    def block_count(p):
        def transform(row):
            fields = row.split(b';')
            # Find the table's exact header; never guess a numeric field.
            header = next(l.split(b';') for l in (p / fitfile).read_bytes().splitlines()
                          if l.startswith(b'; Name ') and b'M10K blocks' in l)
            index = [v.strip() for v in header].index(b'M10K blocks')
            assert fields[index].strip() == b'3'
            fields[index] = fields[index].replace(b'3', b'4')
            return b';'.join(fields)
        cache_row(p, 3, transform)

    exercise(CASES[11], block_count)
    exercise(CASES[12], lambda p: replace(p / 'storage-ready-reports-v2/ready.log',
                                        f'NATIVE_READY_CORNER {corner} 8'.encode(),
                                        f'NATIVE_READY_CORNER {corner} 7'.encode()))

    def trace(p):
        f = p / 'storage-ready-reports-v2/inventory.log'
        match = re.search(rb'^SELFTEST_FIT_REGISTER trace .*\|first_burst_words\[0\]\n', f.read_bytes(), re.M)
        count = re.search(rb'^SELFTEST_FIT_COUNT trace (\d+)$', f.read_bytes(), re.M)
        assert match and count
        replace(f, match[0], b'')
        replace(f, count[0], b'SELFTEST_FIT_COUNT trace ' + str(int(count[1]) - 1).encode())

    exercise(CASES[13], trace)

    def failed(p):
        f = p / 'run-state.json'
        r = json.loads(f.read_bytes())
        r['stages'][1]['returncode'] = 1
        write(f, json.dumps(r).encode())

    exercise(CASES[14], failed)

    def warning(p):
        f = p / 'fit.log'
        write(f, f.read_bytes() + b'\nWarning (999999): unreviewed warning\n')

    exercise(CASES[15], warning)
    exercise(CASES[16], lambda p: negative(p, f'core/burst-first-fault-hold-{corner}.rpt'))
    exercise(CASES[17], lambda p: source(p, 'src/build/rtl-executive/native16-tx-cable/msa_ddram.v',
                                        b'else ready_q<=storage_step ? ready_advance_all : ready_hold_all;',
                                        b'else ready_q<=ready_hold_all;'))
    assert before == {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}
    assert [c['name'] for c in cases] == list(CASES)
    result = dict(status='PASS_NATIVE_STORAGE_BOARD_REVIEW_REJECTION_CONTROLS',
                  positive_qualification_sha256=sha(qualified.read_bytes()),
                  recipe_config_sha256=sha(recipe.read_bytes()), cases=cases,
                  reviewer_dependencies=dependencies(), original_evidence_unchanged=True)
    (output / 'controls.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root', 'recipe', 'qualified', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.root.resolve(), a.recipe.resolve(), a.qualified.resolve(), a.output.resolve())
