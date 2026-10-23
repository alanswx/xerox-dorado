#!/usr/bin/env python3
"""Review a rejected full native fit and bounded address-epoch experiments.

Preserve raw source/readback, every operating condition, actual memory keeper
coverage, original-reference failures and remaining warnings. No native boot,
FPGA image, 60 ns cadence, faster SDRAM or speed qualification is implied.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

from audit_r_front_ready_native import memory_parameters
from review_r_front_control_native import archive
from test_r_front_forward_view import sha
from test_r_front_control_inputs import candidate
from probe_r_front_address_epoch import front

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def path(report):
    raw = report.read_text()
    values = re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)', raw)
    if not values:
        assert 'Found 0 paths' in raw or 'No paths' in raw, report
        return None
    return dict(slack_ns=min(map(float, values)),
                start=re.search(r'^; From Node[ \t]+; ([^;\n]+);', raw, re.M)[1].strip(),
                end=re.search(r'^; To Node[ \t]+; ([^;\n]+);', raw, re.M)[1].strip())


def proof(d, kind):
    r = read(d / 'run-state.json')
    parent = Path(r['parent'])
    assert sha(parent / 'run-state.json') == r['parent_run_state_sha256']
    changed = 0
    for row in r['source_files']:
        p, q = d / row['path'], parent / row['path']
        assert sha(p) == row['output_sha256'] and sha(q) == row['parent_sha256']
        raw = q.read_text()
        expected = raw
        if kind == 'control' and p.name == 'candidate.sv':
            expected = candidate(raw, r.get('complete_control_projection', False))[0]
        if kind == 'address' and p.name == 'dorado_local_r_front_step.sv':
            expected = front(raw, r.get('mutation'))
        assert p.read_text() == expected
        assert row['changed'] == (expected != raw)
        changed += row['changed']
    assert changed == 1
    text = (d / 'test.log').read_text()
    if r.get('mutation'):
        assert r['actions'][-1]['returncode'] != 0 and 'R front output mismatch check=2169' in text
    else:
        assert r['actions'][-1]['returncode'] == 0 and r['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
        clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', text, re.M)
        assert len(clocks) == 100 and all(int(n) > 0 for c, n in clocks)
    return dict(directory=d.name, status=r['status'], mutation=r.get('mutation'), exact_source_transform=True,
                run_state_sha256=sha(d / 'run-state.json'))


def review(native, out, collection_path=None):
    assert not out.exists()
    collection_path = collection_path or BUILD / 'r-front-physical-native-v2-terminal-20261023.tgz.json'
    collection = read(collection_path)
    assert sha(Path(str(collection_path)[:-5])) == collection['archive_sha256']
    for row in collection['members']:
        p = native / row['path']
        assert sha(p) == row['sha256'] and p.stat().st_size == row['bytes']
    readback = read(native / 'physical-native-input-readback.json')
    manifest = read(native / 'r-front-control-native.json')
    assert readback['actual'] == manifest['source_hashes']
    for n, h in manifest['source_hashes'].items():
        assert sha(native / n) == h
    build = read(native / 'run-state.json')
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert len(build['stages']) == 9 and all(s['returncode'] == 0 for s in build['stages'])
    follow = read(native / 'physical-native-reports-v1/run-state.json')
    assert follow['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    assert follow['actions'][-1]['returncode'] == 0
    assert sha(native / 'physical-native-reports-v1/audit_r_front_physical_native.tcl') == follow['script_sha256']
    log = (native / 'physical-native-reports-v1/audit.log').read_text()
    coverage = re.findall(r'PHYSICAL_NATIVE_COVERAGE (\S+) keepers=(\d+) central=(\d+) primary=(\d+) duplicates=(\d+) history=(\d+)', log)
    assert len(coverage) == len({r[0] for r in coverage}) == 4
    corners = []
    for c, keepers, central, primary, copies, history in coverage:
        assert int(central) == int(primary) + int(copies)
        owners = re.findall(r'^PHYSICAL_NATIVE_OWNER ' + re.escape(c) + r' (\S+)$', log, re.M)
        assert len(owners) == len(set(owners)) == int(central)
        visible = re.findall(r'^PHYSICAL_NATIVE_VISIBLE_R ' + re.escape(c) + r' (\S+)$', log, re.M)
        originals = [n for n in visible if '~DUPLICATE' not in n]
        assert len(originals) == len(set(originals)) == 16
        ram = re.findall(r'^PHYSICAL_NATIVE_RAM ' + re.escape(c) + r' (\S+) keepers=(\d+)$', log, re.M)
        assert len(ram) == 4 and {n for n, count in ram} == {'rm_h', 'stk_h', 'rm_l', 'stk_l'}
        row = dict(corner=c, keepers=int(keepers), central=int(central), primary=int(primary), duplicates=int(copies),
                   history=int(history), original_visible_R=16, SRAM_keeper_counts=dict(ram))
        for mode in ['setup', 'hold', 'mpw']:
            text = (native / f'core/native59-{mode}-{c}.rpt').read_text()
            row[mode + '_ns'] = min(map(float, re.findall(r'; Slack\s*;\s*(-?\d+\.\d+)', text)))
        row['worst'] = path(native / f'core/native59-setup-{c}.rpt')
        for group in ['internal', 'input', 'output']:
            row[group + '_setup'] = path(native / f'core/physical-native-{group}-setup-{c}.rpt')
        ucp = (native / f'core/physical-native-ucp-{c}.rpt').read_text()
        row['unconstrained_summary'] = {name: list(map(int, re.search(r'; ' + re.escape(name) + r'\s*; (\d+)\s*; (\d+)\s*;', ucp).groups()))
            for name in ['Illegal Clocks', 'Unconstrained Clocks', 'Unconstrained Input Ports', 'Unconstrained Input Port Paths', 'Unconstrained Output Ports', 'Unconstrained Output Port Paths']}
        assert row['unconstrained_summary']['Illegal Clocks'] == row['unconstrained_summary']['Unconstrained Clocks'] == [0, 0]
        corners.append(row)
    params = memory_parameters((native / 'core/output_files/Dorado.map.rpt').read_text(encoding='latin-1'))
    banks = {}
    for name, fields in params.items():
        m = re.search(r'\|dorado_r_file_ports:files\|altdpram:(rm_h|stk_h|rm_l|stk_l)_rtl_0$', name)
        if not m:
            continue
        for key, value in {'WIDTH': '9', 'WIDTHAD': '8', 'NUMWORDS': '256', 'RAM_BLOCK_TYPE': 'LUTRAM', 'OUTDATA_REG': 'UNREGISTERED'}.items():
            assert fields[key][0] == value
        banks[m[1]] = {key: value[0] for key, value in fields.items()}
    assert set(banks) == {'rm_h', 'stk_h', 'rm_l', 'stk_l'}
    # Keep this mapping difference explicit. Do not assert that register
    # absorption is harmless or that it introduces an extra guest cycle.
    read_address_mapping = {n: fields['RDADDRESS_REG'] for n, fields in banks.items()}
    assert read_address_mapping == {'stk_h': 'OUTCLOCK', 'stk_l': 'OUTCLOCK', 'rm_h': 'UNREGISTERED', 'rm_l': 'UNREGISTERED'}
    dirs, proofs = [native], []
    for engine in ['icarus', 'verilator']:
        for version in [1, 2]:
            d = BUILD / f'r-front-control-inputs-{engine}-v{version}-20261023'
            proofs.append(proof(d, 'control')); dirs.append(d)
        for suffix in ['epoch', 'zero']:
            d = BUILD / f'r-front-address-{suffix}-{engine}-v1-20261023'
            proofs.append(proof(d, 'address')); dirs.append(d)
    staged = []
    for v in [4, 5]:
        d = BUILD / f'r-front-physical-native-stage-v{v}-20261023'
        r = read(d / 'r-front-control-native.json')
        for n, h in r['source_hashes'].items():
            assert sha(d / n) == h
        lint = BUILD / f'r-front-physical-native-lint-v{v}-20261023'
        l = read(lint / 'run-state.json')
        assert l['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
        warnings = dict(Counter(re.findall(r'^%Warning-([A-Z0-9_]+):', (lint / 'lint.log').read_text(), re.M)))
        staged.append(dict(stage=d.name, manifest_sha256=sha(d / 'r-front-control-native.json'), warnings=warnings))
        dirs += [d, lint]
    out.mkdir(parents=True)
    result = dict(status='REVIEWED_NATIVE_PHYSICAL_FIT_REJECTED', scope=__doc__, raw_members=len(collection['members']),
                  source_readback_exact=True, all_build_and_report_processes_absent=readback['absent_pids'], corners=corners,
                  setup_min_ns=min(r['setup_ns'] for r in corners), hold_min_ns=min(r['hold_ns'] for r in corners),
                  mpw_min_ns=min(r['mpw_ns'] for r in corners), memory_parameters=banks,
                  STK_read_address_mapping_requires_review=read_address_mapping, bounded_proofs=proofs, native_stages=staged,
                  native_timing_qualified=False, firmware_qualified=False, ordinary60ns_qualified=False, speedup_qualified=False,
                  next='Finish broader address-epoch matrix and separate v5 full-core fit; audit actual capture epochs and STK register absorption before promotion. Preserve full A-F goal.')
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result, dirs


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--native', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--archive', type=Path)
    p.add_argument('--collection', type=Path)
    a = p.parse_args()
    result, dirs = review(a.native.resolve(), a.output.resolve(), a.collection.resolve() if a.collection else None)
    print(result['status'], result['setup_min_ns'])
    if a.archive:
        archive(a.archive.resolve(), a.output.resolve(), dirs, additional_tools=[
            'stage_r_front_physical_native.py', 'test_r_front_control_inputs.py', 'probe_r_front_address_epoch.py',
            'test_r_front_address_matrix.py', 'report_r_front_physical_native.py', 'audit_r_front_physical_native.tcl',
            'collect_r_front_physical_native.py', 'review_r_front_physical_native.py'])
