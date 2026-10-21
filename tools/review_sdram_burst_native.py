#!/usr/bin/env python3
"""Review the BL8 native fit against its qualified MC1668 parent.

Require exact source deltas, unchanged RAM behavior/parameters, all original
timing categories and the new raw-buffer register/path inventory. This is
preassembly evidence; original-firmware boot and measured speed are separate.
"""
import argparse
from collections import Counter
import gzip
import hashlib
import json
from pathlib import Path
import re
import sys

from audit_r_front_ready_native import ram_table, memory_parameters
from stage_native_sdram_burst import WRAPPER, BACKEND

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'fpga/mister/snapshot'))
import audit_native_dsketh as readers
from audit_native_conta import ucp


def raw(path):
    if path.exists():
        return path.read_bytes()
    return gzip.decompress(Path(str(path) + '.gz').read_bytes())


def sha(data):
    return hashlib.sha256(data).hexdigest()


def json_at(path):
    return json.loads(raw(path))


def memory_comparison(before, after, fitted=False):
    title = 'Fitter RAM Summary' if fitted else 'Analysis & Synthesis RAM Summary'
    previous, current = ram_table(before, title), ram_table(after, title)
    assert current.keys() == previous.keys() and len(current) == 170, 'RAM inventory differs'
    roms = {n for n in current if '|cell_i2716:' in n}
    assert len(roms) == 4
    for name, row in current.items():
        ignored = {'Location'} if fitted else set()
        if fitted and name in roms:
            # Quartus packs the unchanged four ROMs into a shared seven-M10K
            # pool; each logical row lists only the blocks carrying its bits.
            ignored.add('M10K blocks')
        assert {k: v for k, v in row.items() if k not in ignored} == {
            k: v for k, v in previous[name].items() if k not in ignored}, ('RAM behavior differs', name)
    if fitted:
        sites = set()
        for name in roms:
            row = current[name]
            assert row['Mode'] == 'ROM' and row['Implementation Bits'] == '16384'
            locations = [x.strip() for x in row['Location'].split(',')]
            assert len(locations) == len(set(locations)) == int(row['M10K blocks'])
            assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+', n) for n in locations)
            sites.update(locations)
        assert len(sites) == 7, 'BaseBoard shared ROM allocation differs'
        assert not any(set(r['Location'].split(', ')) & sites for n, r in current.items() if n not in roms)
    else:
        assert memory_parameters(before) == memory_parameters(after), 'RAM parameters differ'
        assert len(memory_parameters(after)) == 165
    return dict(rows=len(current), identical_behavior=True, repacked_ROM_pool_blocks=7 if fitted else None)


def review(root, parent, parent_qualification, parent_reports):
    record = json_at(root / 'sdram-burst-native.json')
    prior = json_at(parent / 'mc1668-candidate.json')
    qualified = json_at(parent_qualification)
    assert qualified['status'] == 'PASS_NATIVE_MC1668_BUILD'
    assert qualified['source_hashes'] == prior['source_hashes']
    assert record['parent_manifest_sha256'] == sha(raw(parent / 'mc1668-candidate.json'))
    assert record['parent_qualification_sha256'] == sha(raw(parent_qualification))
    assert record['frequency_mhz'] == 50 and record['sysper'] == 4 and record['state_admin'] == 'INACTIVE'
    assert record['cache_lines'] == 4 and len(record['source_hashes']) == 499
    actual = {n: sha(raw(root / n)) for n in record['source_hashes']}
    assert actual == record['source_hashes'], 'Compiled input changed'
    assert actual[BACKEND] == sha((ROOT / 'fpga/mister/dorado_storage_sdram_burst.sv').read_bytes())
    old_type = 'dorado_storage_sdram #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ)) u_sdram('
    new_type = 'dorado_storage_sdram_burst #(.ADDR_BITS(RAW_BITS),.CLOCK_HZ(STORAGE_CLOCK_HZ),.CACHE_LINES(4)) u_sdram('
    for name, digest in prior['source_hashes'].items():
        value = raw(parent / name)
        assert sha(value) == digest, ('Qualified parent changed', name)
        if Path(name).suffix in ('.qsf', '.qip', '.qpf', '.sdc', '.tcl'):
            value = value.replace(prior['project'].encode(), record['project'].encode())
        current = raw(root / name)
        if name == WRAPPER:
            assert value.count(old_type.encode()) == 1
            value = value.replace(old_type.encode(), new_type.encode())
        elif name == 'core/Dorado.qsf':
            value += ('\n# Raw BL8 buffer only; unchanged CPU/cache/map/ECC and clocks.\n'
                      'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(Path(record['project']) / BACKEND) + '\n').encode()
        assert current == value, ('Unintended source/config delta', name)
    assert set(actual) - set(prior['source_hashes']) == {BACKEND}
    state = json_at(root / 'run-state.json')
    assert state['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [r['stage'] for r in state['stages']] == ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings']
    assert all(r['returncode'] == 0 for r in state['stages'])
    extra = json_at(root / 'burst-review-v2-state.json')
    assert extra['status'] == 'TERMINAL_BURST_REPORTS_REVIEW_REQUIRED'
    assert len(extra['actions']) == 1 and extra['actions'][0]['returncode'] == 0
    assert extra['tcl_sha256'] == sha(raw(root / 'audit_sdram_burst_native.tcl')) == sha((ROOT / 'tools/audit_sdram_burst_native.tcl').read_bytes())
    collected = json_at(root / 'collected-input-check.json')
    assert collected['actual'] == actual
    expected_pids = {state['builder_pid'], extra['driver_pid']} | {r['pid'] for r in state['stages'] + extra['actions']}
    assert set(collected['absent_pids']) == expected_pids
    core = root / 'core'
    mapping = raw(core / 'output_files/Dorado.map.rpt').decode()
    fitted = raw(core / 'output_files/Dorado.fit.rpt').decode('latin-1')
    memories = dict(synthesis=memory_comparison(raw(parent_reports / 'Dorado.map.rpt').decode(), mapping),
                    physical=memory_comparison(raw(parent_reports / 'Dorado.fit.rpt').decode('latin-1'), fitted, True))
    manifest = json_at(root / 'src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD']
    memories['cache'] = readers.native_cache_physical(fitted, manifest, 'independent40')
    assert memories['cache']['unique_m10ks'] == 40
    sources = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n', 1)[0]
    used = set()
    for line in sources.splitlines():
        fields = [v.strip() for v in line.split(';')[1:-1]]
        if len(fields) == 5 and fields[1] == 'yes':
            used.add(str(Path(fields[3]).relative_to(record['project'])) if fields[3].startswith(record['project'] + '/') else fields[3])
    assert {WRAPPER, BACKEND} <= used, 'Backend/wrapper absent from synthesized netlist'
    paths = []
    for corner in sorted(readers.CORNERS):
        for mode in readers.MODES:
            name = f'native59-{mode}-{corner}.rpt'
            paths.append(readers.timed(raw(core / name).decode(), name))
        assert ucp(raw(core / f'native59-ucp-{corner}.rpt').decode()) == ucp(raw(parent_reports.parent / f'native59-ucp-{corner}.rpt').decode()), ('Unconstrained coverage differs', corner)
    assert len(paths) == 196
    checks = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)', raw(core / 'output_files/Dorado.sta.summary').decode())
    assert len(checks) == 31 and min(float(v) for _, v in checks) >= 0
    log = raw(root / 'burst-review-v2.log').decode()
    counts = re.findall(r'^BURST_REGISTER_COUNT (\S+) (\S+) (\d+)$', log, re.M)
    assert len(counts) == 20 and {(c, n) for c, n, _ in counts} == {(c, n) for c in readers.CORNERS for n in ['words', 'tags', 'valid', 'capture', 'response']}
    # Every original MSA group consumes at most twelve of the raw halfword's
    # bits. Quartus prunes the four unused high data bits; 23 raw address bits
    # leave twenty effective tag bits per line. Require the exact fit here.
    msa = raw(root / 'src/build/rtl-executive/native16-tx-cable/msa_ddram.v').decode()
    widths = list(map(int, re.findall(r'dorado_storage_group #\(\.ADDR_BITS\(16\), \.WIDTH\((\d+)\)\)', msa)))
    assert len(widths) == 16 and max(widths) == 12
    expected_counts = dict(words=384, tags=80, valid=4, capture=12, response=12)
    assert all(int(count) == expected_counts[n] for c, n, count in counts), ('Buffer physical register coverage differs', counts)
    registers = re.findall(r'^BURST_REGISTER (.+)$', log, re.M)
    assert len(registers) == len(set(registers)) and len(registers) >= 492
    new_paths = []
    for corner in sorted(readers.CORNERS):
        for label in ['internal', 'fill', 'cached-read', 'tag-select', 'input-capture', 'request', 'completion']:
            for mode in ['setup', 'hold']:
                name = f'burst-{label}-{mode}-{corner}.rpt'
                new_paths.append(readers.timed(raw(core / name).decode(), name))
    assert len(new_paths) == 56
    warnings = {}
    classified_truncation = None
    for phase in ['map', 'fit']:
        text = raw(root / (phase + '.log')).decode()
        assert 'Critical Warning' not in text
        before = raw(parent_reports.parent.parent / (phase + '.log')).decode()
        if phase == 'map':
            # The FIFO wrap ternary uses an unsized zero, so Quartus reports
            # 32 -> 2 truncation. Its nonzero branch increments a two-bit
            # index only below three; its other branch assigns zero. This
            # exact warning is harmless for the bound four-line variant.
            line = ('Warning (10230): Verilog HDL assignment warning at '
                    'dorado_storage_sdram_burst.sv(213): truncated value with size 32 '
                    'to match size of target (2) File: ' + record['project'] + '/' + BACKEND + ' Line: 213')
            assert text.splitlines().count(line) == 1
            assert raw(root / BACKEND).decode().splitlines()[212].strip() == "next_line<=fill_line==CACHE_LINES-1 ? 0 : fill_line+1'b1;"
            classified_truncation = line
            text = text.replace(line, '')
        increase = Counter(re.findall(r'Warning \((\d+)\)', text)) - Counter(re.findall(r'Warning \((\d+)\)', before))
        assert not set(increase) - {'10036'}, ('New warning category/count', phase, increase)
        warnings[phase] = dict(increase)
    summary = raw(core / 'output_files/Dorado.fit.summary').decode()
    assert 'Fitter Status : Successful' in summary
    resources = {}
    for key, label in [('alms', 'Logic utilization (in ALMs)'), ('registers', 'Total registers'), ('ram_blocks', 'Total RAM Blocks'), ('dsp', 'Total DSP Blocks')]:
        match = re.search(re.escape(label) + r'\s*:\s*([\d,]+)', summary)
        assert match
        resources[key] = int(match[1].replace(',', ''))
    assert resources['ram_blocks'] == 332 and resources['dsp'] == 35
    evidence = {}
    for path in root.rglob('*'):
        if not path.is_file():
            continue
        name = str(path.relative_to(root))
        if name.endswith('.gz'):
            name = name[:-3]
        if ((name.startswith('core/') and Path(name).suffix in ('.rpt', '.summary', '.log')) or
                name in ['run-state.json', 'burst-review-v2-state.json', 'burst-review-v2.log',
                         'collected-input-check.json', 'audit_sdram_burst_native.tcl', 'map.log', 'fit.log']):
            evidence[name] = sha(raw(root / name))
    return dict(status='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW', scope=__doc__,
                source_hashes=actual, memory_evidence=memories, timing_checks=checks,
                detailed_paths=paths, burst_paths=new_paths, burst_register_counts=expected_counts,
                backend_registers=registers, resources=resources, warnings=warnings,
                classified_FIFO_index_truncation=classified_truncation,
                reviewed_evidence_hashes=evidence,
                minimum_slack_ns=min(x['slack_ns'] for x in paths + new_paths),
                frequency_mhz=50, sysper=4, state_admin='INACTIVE',
                firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'parent', 'parent-qualification', 'parent-reports', 'output']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    result = review(a.root.resolve(), a.parent.resolve(), a.parent_qualification.resolve(), a.parent_reports.resolve())
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], result['minimum_slack_ns'], result['resources'])
