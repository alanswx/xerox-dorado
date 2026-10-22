#!/usr/bin/env python3
"""Reject damaged source, RAM, coverage and timing in an actual native fit."""
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

from review_timing_paths_native import review
from test_review_storage_board_native import dependencies

ROOT = Path(__file__).resolve().parents[1]
CASES = ('missing-original-IFU', 'negative-SDRAM-output', 'negative-overlap-backend',
         'negative-overlap-groups-hold', 'negative-global-hold', 'wrong-half-cycle',
         'rehashed-disabled-overlap', 'rehashed-CPU-clock', 'wrong-cache-old-data',
         'cache-bank-alias', 'missing-buffer-bit', 'missing-hit-counter-bit',
         'missing-native-group', 'wrong-derived-clock', 'new-warning',
         'snapshot-enabled', 'failed-fit',
         'negative-look-selection', 'negative-cached-response', 'negative-look-group-hold',
         'negative-look-outgoing', 'negative-crystal-incoming', 'negative-crystal-downstream',
         'missing-look-owner-bit', 'missing-look-mask-bit', 'wrong-compiled-crystal-ratio',
         'wrong-compiled-lookahead', 'rehashed-ALU-carry', 'rehashed-crystal-ratio')


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(root, config, qualified, out):
    assert not out.exists()
    expected = json.loads(qualified.read_text())
    assert json.loads(json.dumps(review(root, config))) == expected
    out.mkdir(parents=True)

    def hashes(directory):
        return {str(p.relative_to(directory)): sha(p.read_bytes())
                for p in directory.rglob('*') if p.is_file()}

    before = hashes(root)

    def write(path, raw):
        temporary = path.with_name(path.name + '.mutation')
        temporary.write_bytes(raw)
        os.replace(temporary, path)

    def replace(path, old, new):
        raw = path.read_bytes()
        assert raw.count(old) == 1, (path, old, raw.count(old))
        write(path, raw.replace(old, new))

    def negative(path):
        match = re.search(rb'Worst case slack is\s+([-+\d.]+)', path.read_bytes())
        assert match
        replace(path, match[0], b'Worst case slack is -0.001')

    corner = '7_slow_1100mv_100c'

    def source(directory, name, old, new):
        replace(directory / name, old, new)
        for file, key in [('timing-paths-native.json', 'source_hashes'),
                          ('collected-ready-input-check.json', 'actual')]:
            p = directory / file
            value = json.loads(p.read_bytes())
            value[key][name] = sha((directory / name).read_bytes())
            write(p, json.dumps(value).encode())

    def half_cycle(directory):
        p = directory / f'core/storage-sample-output-setup-{corner}.rpt'
        rows = [s for s in p.read_bytes().splitlines() if len(s.split(b';')[1:-1]) == 8 and b'; 8.750 ' in s]
        assert len(rows) == 256
        replace(p, rows[0], rows[0].replace(b'; 8.750 ', b'; 17.500 ', 1))

    def cache_row(directory, bank, transform):
        p = directory / 'core/output_files/Dorado.fit.rpt'
        rows = [s for s in p.read_bytes().splitlines() if b':b_MemD|' in s and f':snapshot_bank_{bank}|'.encode() in s and b'; Old data' in s]
        assert len(rows) == 1
        replace(p, rows[0], transform(rows[0]))

    def alias(directory):
        p = directory / 'core/output_files/Dorado.fit.rpt'
        rows = [s for s in p.read_bytes().splitlines() if b':b_MemD|' in s and b':snapshot_bank_3|' in s and b'; Old data' in s]
        assert len(rows) == 1
        sites = re.findall(rb'M10K_X\d+_Y\d+_N\d+', rows[0])
        assert len(sites) == 2

        def transform(row):
            old = re.findall(rb'M10K_X\d+_Y\d+_N\d+', row)
            assert len(old) == 2
            for a, b in zip(old, sites):
                row = row.replace(a, b)
            return row
        cache_row(directory, 8, transform)

    def missing(path, pattern):
        raw = path.read_bytes()
        match = re.search(pattern, raw, re.M)
        assert match, path
        replace(path, match[0], b'')

    def group(directory):
        p = directory / 'storage-ready-reports-v1/response-overlap.log'
        raw = p.read_bytes()
        pattern = (rb'^RESPONSE_FAMILY_REGISTER ' + corner.encode() +
                   rb' groups .*\|msa_ddram:b_msa\|dorado_storage_group:u_storage_0\|[^\n]+\n')
        raw, count = re.subn(pattern, b'', raw, flags=re.M)
        assert count > 0
        pattern = rb'(^RESPONSE_FAMILY_COUNT ' + corner.encode() + rb' groups )(\d+)'
        raw, changed = re.subn(pattern, lambda m: m[1] + str(int(m[2])-count).encode(), raw, flags=re.M)
        assert changed == 1
        write(p, raw)

    def clock(directory):
        p = directory / 'core/native-clocks.rpt'
        rows = [s for s in p.read_bytes().splitlines() if b'dorado_sdram_clk ' in s and b'; Generated' in s]
        assert len(rows) == 1
        replace(p, rows[0], rows[0].replace(b'17.500', b'15.000'))

    def snapshot(directory):
        p = directory / 'core/output_files/Dorado.map.rpt'
        match = re.search(rb'; SNAPSHOT_PAUSE\s*; 0\s*;', p.read_bytes())
        assert match
        replace(p, match[0], match[0].replace(b'; 0 ', b'; 1 ', 1))

    def failed(directory):
        p = directory / 'run-state.json'
        value = json.loads(p.read_bytes())
        value['stages'][1]['returncode'] = 1
        write(p, json.dumps(value).encode())

    def global_hold(directory):
        p = directory / 'core/output_files/Dorado.sta.summary'
        raw = p.read_bytes()
        match = re.search(rb'(Type\s*:\s*Hold [^\n]+\nSlack\s*:\s*)([-+\d.]+)', raw)
        assert match
        replace(p, match[0], match[1] + b'-0.001')

    def look_bit(directory, field):
        p = directory / 'storage-ready-reports-v1/request-lookahead.log'
        raw = p.read_bytes()
        pattern = (rb'^LOOK_FAMILY_REGISTER ' + corner.encode() +
                   rb' lookahead .*\|' + re.escape(field.encode()) + rb'(?:~DUPLICATE(?:_\d+)?)?\n')
        raw, count = re.subn(pattern, b'', raw, flags=re.M)
        assert count > 0, field
        pattern = rb'(^LOOK_FAMILY_COUNT ' + corner.encode() + rb' lookahead )(\d+)'
        raw, changed = re.subn(pattern, lambda m: m[1] + str(int(m[2])-count).encode(), raw, flags=re.M)
        assert changed == 1
        write(p, raw)

    def crystal_ratio(directory):
        p = directory / 'core/output_files/Dorado.map.rpt'
        raw = p.read_bytes()
        headers = list(re.finditer(rb'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)', raw))
        sections = [raw[h.start():headers[i+1].start() if i+1 < len(headers) else len(raw)]
                    for i,h in enumerate(headers) if b'cell_MC12061:u_d08' in h[1]]
        assert len(sections) == 1
        match = re.search(rb'; SYSPER\s*; 8\s*;', sections[0]); assert match
        replace(p, sections[0], sections[0].replace(match[0], match[0].replace(b'; 8 ', b'; 4 ', 1)))

    def compiled_look(directory):
        p = directory / 'core/output_files/Dorado.map.rpt'
        match = re.search(rb'; REQUEST_LOOKAHEAD\s*; 1\s*;', p.read_bytes()); assert match
        replace(p, match[0], match[0].replace(b'; 1 ', b'; 0 ', 1))

    mutations = [
        lambda p: (p / f'core/native59-ifu-in-setup-{corner}.rpt').unlink(),
        lambda p: negative(p / f'core/native59-sdram-output-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/response-backend-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/response-groups-hold-{corner}.rpt'),
        global_hold, half_cycle,
        lambda p: source(p, 'src/fpga/mister/dorado_storage_sdram_burst.sv', b'(!response_pending || (RESPONSE_OVERLAP && rsp_ready))', b'!response_pending'),
        lambda p: source(p, 'src/fpga/mister/Dorado.sv', b'MACHINE_SYSPER = 4', b'MACHINE_SYSPER = 2'),
        lambda p: cache_row(p, 3, lambda row: row.replace(b'Old data', b'New data')),
        alias,
        lambda p: missing(p / 'storage-ready-reports-v1/backend.log', rb'^BURST_REGISTER .*\|line_words\[3\]\[0\]\n'),
        lambda p: missing(p / 'storage-ready-reports-v1/cache-counter.log', rb'^CACHE_HIT_COUNTER_REGISTER 7_slow_1100mv_100c .*count\[11\]\[0\]\n'),
        group, clock,
        lambda p: write(p / 'fit.log', (p / 'fit.log').read_bytes() + b'\nWarning (999999): unexpected new warning\n'),
        snapshot, failed,
        lambda p: negative(p / f'storage-ready-reports-v1/look-selection-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/look-cached-response-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/look-group-ready-hold-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/look-outgoing-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/crystal-baseboard-setup-{corner}.rpt'),
        lambda p: negative(p / f'storage-ready-reports-v1/crystal-downstream-hold-{corner}.rpt'),
        lambda p: look_bit(p, 'look_payload[39]'),
        lambda p: look_bit(p, 'look_mask[0]'),
        crystal_ratio, compiled_look,
        lambda p: source(p, 'src/verilog/cells/cell_MC10181.v', b'Cn ? incremented : gsum', b"1'b0 ? incremented : gsum"),
        lambda p: source(p, 'src/build/rtl-executive/native16-tx-cable/BaseBd_nominal.v', b'.SYSPER(2*SYSPER)', b'.SYSPER(SYSPER)'),
    ]
    assert len(mutations) == len(CASES)
    cases = []
    for name, mutate in zip(CASES, mutations):
        with tempfile.TemporaryDirectory(prefix='dorado-response-review-bad-') as temporary:
            directory = Path(temporary) / 'root'
            shutil.copytree(root, directory, copy_function=os.link)
            mutate(directory)
            command = [sys.executable, str(ROOT / 'tools/review_timing_paths_native.py'),
                       '--root', str(directory), '--config', str(config),
                       '--output', str(Path(temporary) / 'bad-qualification.json')]
            result = subprocess.run(command, capture_output=True, text=True)
            log = result.stdout + result.stderr
            (out / (name + '.log')).write_text(log)
            assert result.returncode != 0 and ('AssertionError' in result.stderr or 'FileNotFoundError' in result.stderr), (name, log[-1500:])
            cases.append(dict(name=name, rejected=True, returncode=result.returncode, log_sha256=sha(log.encode())))
            print(name, 'rejected', flush=True)
    assert json.loads(json.dumps(review(root, config))) == expected
    assert hashes(root) == before, 'Original evidence changed'
    bound_dependencies = dependencies()
    for name in ['audit_storage_cache_native.tcl', 'audit_storage_response_native.tcl', 'audit_baseboard_crystal_native.tcl', 'audit_storage_request_lookahead_native.tcl']:
        bound_dependencies['tools/' + name] = sha((ROOT / 'tools' / name).read_bytes())
    result = dict(status='PASS_NATIVE_TIMING_PATHS_REVIEW_REJECTION_CONTROLS', cases=cases,
                  positive_qualification_sha256=sha(qualified.read_bytes()),
                  recipe_config_sha256=sha(config.read_bytes()),
                  reviewer_dependencies=bound_dependencies, original_evidence_unchanged=True)
    (out / 'controls.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'config', 'qualified', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    run(args.root.resolve(), args.config.resolve(), args.qualified.resolve(), args.output.resolve())
