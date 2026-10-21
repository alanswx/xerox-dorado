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
import stage_boot_fault_observer as observer

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


def memory_comparison(before, after, fitted=False, allow_host_video_auto_ids=False):
    title = 'Fitter RAM Summary' if fitted else 'Analysis & Synthesis RAM Summary'
    previous, current = ram_table(before, title), ram_table(after, title)
    renamed = {}
    if allow_host_video_auto_ids:
        # Adding the observer changes two global vendor auto-instance IDs.
        # Keep the full logical video pipeline path and every geometry/mode/
        # register field. Never normalize any Dorado memory instance name.
        prefixes = ['ascal:ascal|altshift_taps:o_dev_rtl_0|shift_taps_5gv:auto_generated|',
                    'vga_out:vga_scaler_out|altshift_taps:din1_rtl_0|shift_taps_quu:auto_generated|']
        def normalized(rows):
            result = {}
            for name, row in rows.items():
                canonical = name
                for prefix in prefixes:
                    if name.startswith(prefix):
                        canonical, count = re.subn(r'^' + re.escape(prefix) + r'altsyncram_[a-z0-9]+:', prefix + 'altsyncram_AUTO:', name)
                        assert count == 1
                        renamed[name] = canonical
                assert canonical not in result
                result[canonical] = dict(row, Name=canonical)
            return result
        previous, current = normalized(previous), normalized(current)
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
    return dict(rows=len(current), identical_behavior=True, repacked_ROM_pool_blocks=7 if fitted else None,
                host_video_auto_instance_ids=renamed)


def review(root, parent, parent_qualification, parent_reports, observer_component=None):
    record = json_at(root / 'sdram-burst-native.json')
    prior = json_at(parent / 'mc1668-candidate.json')
    qualified = json_at(parent_qualification)
    assert qualified['status'] == 'PASS_NATIVE_MC1668_BUILD'
    assert qualified['source_hashes'] == prior['source_hashes']
    assert record['parent_manifest_sha256'] == sha(raw(parent / 'mc1668-candidate.json'))
    assert record['parent_qualification_sha256'] == sha(raw(parent_qualification))
    assert record['frequency_mhz'] == 50 and record['sysper'] == 4 and record['state_admin'] == 'INACTIVE'
    has_observer = record.get('passive_boot_fault_observer', False)
    assert record['cache_lines'] == 4 and len(record['source_hashes']) == (500 if has_observer else 499)
    actual = {n: sha(raw(root / n)) for n in record['source_hashes']}
    assert actual == record['source_hashes'], 'Compiled input changed'
    assert actual[BACKEND] == sha((ROOT / 'fpga/mister/dorado_storage_sdram_burst.sv').read_bytes())
    observer_proof = None
    if has_observer:
        assert observer_component is not None, 'Missing tested passive observer component'
        s = json_at(observer_component / 'run-state.json')
        p = json_at(observer_component / 'provenance.json')
        assert s['status'] == 'PASS_PASSIVE_BOOT_FAULT_MONITOR'
        assert len(s['actions']) == 10
        negative = {'premature-arm-test', 'late-source-test', 'overwrite-first-test'}
        assert all((a['returncode'] != 0) if a['stage'] in negative else a['returncode'] == 0 for a in s['actions'])
        for n, h in p['inputs'].items():
            assert sha(raw(observer_component / 'inputs' / n)) == h
        assert actual[observer.OBSERVER] == p['inputs']['monitor.sv'] == sha((ROOT / 'fpga/mister/dorado_boot_fault_monitor.sv').read_bytes())
        observer_proof = p
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
        if has_observer:
            value = observer.expose(name, value)
            if name == 'core/Dorado.qsf':
                value += ('\n# Passive original boot first-error diagnostic.\n'
                          'set_global_assignment -name SYSTEMVERILOG_FILE ' + str(Path(record['project']) / observer.OBSERVER) + '\n').encode()
        assert current == value, ('Unintended source/config delta', name)
    assert set(actual) - set(prior['source_hashes']) == ({BACKEND, observer.OBSERVER} if has_observer else {BACKEND})
    state = json_at(root / 'run-state.json')
    assert state['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [r['stage'] for r in state['stages']] == ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings']
    assert all(r['returncode'] == 0 for r in state['stages'])
    extra = json_at(root / 'burst-review-v2-state.json')
    assert extra['status'] == 'TERMINAL_BURST_REPORTS_REVIEW_REQUIRED'
    assert len(extra['actions']) == 1 and extra['actions'][0]['returncode'] == 0
    tcl_name = 'audit_boot_fault_native.tcl' if has_observer else 'audit_sdram_burst_native.tcl'
    assert extra['tcl_sha256'] == sha(raw(root / tcl_name)) == sha((ROOT / 'tools' / tcl_name).read_bytes())
    collected = json_at(root / 'collected-input-check.json')
    assert collected['actual'] == actual
    expected_pids = {state['builder_pid'], extra['driver_pid']} | {r['pid'] for r in state['stages'] + extra['actions']}
    assert set(collected['absent_pids']) == expected_pids
    core = root / 'core'
    mapping = raw(core / 'output_files/Dorado.map.rpt').decode()
    fitted = raw(core / 'output_files/Dorado.fit.rpt').decode('latin-1')
    memories = dict(synthesis=memory_comparison(raw(parent_reports / 'Dorado.map.rpt').decode(), mapping, allow_host_video_auto_ids=has_observer),
                    physical=memory_comparison(raw(parent_reports / 'Dorado.fit.rpt').decode('latin-1'), fitted, True, has_observer))
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
    if has_observer:
        assert {observer.OBSERVER, observer.CONTB, observer.BACKPLANE, observer.WRAPPER, observer.TOP} <= used, 'Passive diagnostic patched an unused source'
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
    registers = re.findall(r'^BURST_REGISTER (.+)$', log, re.M)
    assert len(registers) == len(set(registers)) and len(registers) >= 492
    expected_counts = dict(words=384, tags=80, valid=4, capture=12, response=12)
    response_registers = [n for n in registers if '|rsp_data' in n]
    response_bits = {int(m[1]) for n in response_registers if (m := re.search(r'\|rsp_data\[(\d+)\]$', n))}
    assert response_bits == set(range(12)), 'Missing logical raw response bit'
    response_copies = [n for n in response_registers if n.endswith('~DUPLICATE')]
    if has_observer:
        # Physical optimization makes one extra copy of the unchanged raw
        # response bit1 in this placement. Preserve all twelve originals and
        # accept exactly that named fitter copy, with all its paths timed.
        assert len(response_copies) == 1 and response_copies[0].endswith('|rsp_data[1]~DUPLICATE')
        expected_counts['response'] = 13
    else:
        assert not response_copies
    assert len(response_registers) == expected_counts['response']
    assert all(int(count) == expected_counts[n] for c, n, count in counts), ('Buffer physical register coverage differs', counts)
    new_paths = []
    for corner in sorted(readers.CORNERS):
        for label in ['internal', 'fill', 'cached-read', 'tag-select', 'input-capture', 'request', 'completion']:
            for mode in ['setup', 'hold']:
                name = f'burst-{label}-{mode}-{corner}.rpt'
                new_paths.append(readers.timed(raw(core / name).decode(), name))
    assert len(new_paths) == 56
    observer_paths = []
    if has_observer:
        inventory = re.findall(r'^BOOT_FAULT_REGISTERS (\S+) (\d+)$', log, re.M)
        assert len(inventory) == 4 and {c for c, _ in inventory} == readers.CORNERS
        assert all(int(count) == 82 for _, count in inventory)
        for corner in sorted(readers.CORNERS):
            for mode in ['setup', 'hold']:
                name = f'burst-first-fault-{mode}-{corner}.rpt'
                observer_paths.append(readers.timed(raw(core / name).decode(), name))
        assert len(observer_paths) == 8
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
                         'collected-input-check.json', tcl_name, 'map.log', 'fit.log']):
            evidence[name] = sha(raw(root / name))
    return dict(status='PASS_NATIVE_SDRAM_BURST_PREASSEMBLY_REVIEW', scope=__doc__,
                source_hashes=actual, memory_evidence=memories, timing_checks=checks,
                detailed_paths=paths, burst_paths=new_paths, burst_register_counts=expected_counts,
                backend_registers=registers, response_register_copies=response_copies,
                resources=resources, warnings=warnings,
                classified_FIFO_index_truncation=classified_truncation,
                reviewed_evidence_hashes=evidence,
                minimum_slack_ns=min(x['slack_ns'] for x in paths + new_paths + observer_paths),
                passive_boot_fault_observer=has_observer, observer_component=observer_proof,
                observer_paths=observer_paths,
                frequency_mhz=50, sysper=4, state_admin='INACTIVE',
                firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'parent', 'parent-qualification', 'parent-reports', 'output']:
        p.add_argument('--' + name, type=Path, required=True)
    p.add_argument('--observer-component', type=Path)
    a = p.parse_args()
    result = review(a.root.resolve(), a.parent.resolve(), a.parent_qualification.resolve(), a.parent_reports.resolve(), a.observer_component.resolve() if a.observer_component else None)
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], result['minimum_slack_ns'], result['resources'])
