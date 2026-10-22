#!/usr/bin/env python3
"""Review the address-sampled disk-window candidate before assembly.

Regenerate every compiled source from the qualified parent and tested components.
Require original timing, physical RAM modes, exact half-cycle/register coverage,
and narrowly classified warning differences. Firmware/speed are separate gates.
"""
import argparse
from argparse import Namespace
import ast
from collections import Counter
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import tempfile

from audit_r_front_ready_native import ram_table
from review_sdram_burst_native import memory_comparison, readers
from audit_native_conta import ucp
from stage_pack_write_window_native import stage


def sha(b):
    return hashlib.sha256(b).hexdigest()


def memory(before_map, before_fit, mapping, fitting, manifest):
    cache = readers.native_cache_physical(fitting, manifest, 'independent40')
    old, new = ram_table(before_fit, 'Fitter RAM Summary'), ram_table(fitting, 'Fitter RAM Summary')
    changed = {}
    for name, row in old.items():
        if name in new and ':b_MemD|' in name and row['M10K blocks'] != new[name]['M10K blocks']:
            bank = int(re.search(r'snapshot_bank_(\d+)\|', name)[1])
            assert bank in {3, 5, 8, 10}
            assert row['M10K blocks'] == '3' and new[name]['M10K blocks'] == '2'
            changed[name] = bank
    assert set(changed.values()) == {3, 5, 8, 10}
    # Classify only the already-reviewed independent40 physical allocation.
    # Original reports remain byte-exact. No logical mode/geometry is ignored.
    lines, header = [], None
    for line in before_fit.splitlines(keepends=True):
        fields = line.split(';')
        values = [x.strip() for x in fields[1:-1]]
        if values and values[0] == 'Name' and 'M10K blocks' in values:
            header = values
        if values and values[0] in changed:
            assert header and len(header) == len(values)
            i = header.index('M10K blocks') + 1
            assert fields[i].strip() == '3'
            fields[i] = fields[i].replace('3', '2')
            line = ';'.join(fields)
        lines.append(line)
    occupied = {site for group in cache['groups'] for site in group['locations']}
    assert all(not occupied.intersection(s.strip() for s in row['Location'].split(','))
               for name, row in new.items() if ':b_MemD|' not in name)
    return dict(mapping=memory_comparison(before_map, mapping, False, True),
                fitting=memory_comparison(''.join(lines), fitting, True, True),
                cache=cache, changed_physical_row_counts=changed)


def sample_paths(text, label, mode, clock):
    rows = []
    for line in text.splitlines():
        f = [s.strip() for s in line.split(';')[1:-1]]
        if len(f) != 8 or not re.fullmatch(r'[-+\d.]+', f[0]):
            continue
        slack, source, sink, launch, latch, relationship, skew, delay = f
        assert float(slack) > 0 and launch == latch == clock
        assert float(relationship) == (8.75 if mode == 'setup' else -8.75)
        if label == 'input':
            a = re.fullmatch(r'(.+\|msa_ddram:b_\w+)\|cell_MK4096P_addr_preview:u_dram_addr_(\d+)\|(row|col)\[(\d+)\](?:~DUPLICATE(?:_\d+)?)?', source)
            b = re.fullmatch(r'(.+\|msa_ddram:b_\w+)\|dorado_storage_group:u_storage_(\d+)\|addr_before_host\[(\d+)\]', sink)
            assert a and b and a.group(1, 2) == b.group(1, 2)
            assert int(b[3]) == int(a[4]) + (8 if a[3] == 'row' else 0)
        else:
            a = re.fullmatch(r'(.+)\|addr_before_host\[(\d+)\]', source)
            b = re.fullmatch(r'(.+)\|saved_addr\[(\d+)\](?:~DUPLICATE(?:_\d+)?)?', sink)
            assert a and b and a.groups() == b.groups()
        rows.append((source, sink))
    assert len(rows) == 256, ('Missing half-cycle path rows', len(rows))
    return len(rows)


def review(root, config):
    evidence = {}

    def read(name):
        b = (root / name).read_bytes()
        evidence[name] = sha(b)
        return b

    def text(name):
        return read(name).decode('latin-1')

    def js(name):
        return json.loads(read(name))

    r = js('pack-window-native.json')
    assert r['state_admin'] == 'INACTIVE' and r['frequency_hz'] == 57142857 and r['sysper'] == 4
    assert r['request_address_sample']['opposite_edge'] and r['request_address_sample']['added_request_cycles'] == 0
    assert not r['assembly_permitted'] and not r['firmware_qualified']
    actual = {n: sha(read(n)) for n in r['source_hashes']}
    assert len(actual) == 503 and actual == r['source_hashes']
    inputs = json.loads(config.read_text())
    parent = Path(inputs['parent_reports'])
    args = {n: Path(v).resolve() for n, v in inputs.items() if n != 'parent_reports'}
    args.update(project=Path(r['project']), repair_hold=False)
    with tempfile.TemporaryDirectory(prefix='dorado-pack-review-') as d:
        args['output'] = Path(d) / 'stage'
        with contextlib.redirect_stdout(io.StringIO()):
            stage(Namespace(**args))
        expected = json.loads((args['output'] / 'pack-window-native.json').read_text())
        # Stager's explanatory docstring changed after staging, not its recipe.
        assert r['scope'] == ast.get_docstring(ast.parse(text('stage_pack_write_window_native.py')), clean=False)
        assert {k: v for k, v in r.items() if k != 'scope'} == {k: v for k, v in expected.items() if k != 'scope'}
        assert {n: sha((args['output'] / n).read_bytes()) for n in actual} == actual

    build, reports, collection = js('run-state.json'), js('storage-ready-reports-v1/run-state.json'), js('collected-ready-input-check.json')
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [s['stage'] for s in build['stages']] == ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings', 'storage-ready']
    assert reports['status'] == 'TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED'
    assert [s['stage'] for s in reports['actions']] == ['ready', 'backend', 'inventory']
    assert all(s['returncode'] == 0 for s in build['stages'] + reports['actions'])
    pids = {build['builder_pid'], reports['driver_pid']} | {s['pid'] for s in build['stages'] + reports['actions']}
    assert set(collection['absent_pids']) == pids and collection['actual'] == actual
    assert sha(read('build_r_front_ready_native.py')) == build['builder_sha256']
    for n, h in reports['report_inputs'].items():
        assert sha(read('storage-ready-reports-v1/' + n)) == h

    mapping, fitting = text('core/output_files/Dorado.map.rpt'), text('core/output_files/Dorado.fit.rpt')
    mem = memory((parent / 'core/output_files/Dorado.map.rpt').read_text(encoding='latin-1'),
                 (parent / 'core/output_files/Dorado.fit.rpt').read_text(encoding='latin-1'), mapping, fitting,
                 js('src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD'])
    summary = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)', text('core/output_files/Dorado.sta.summary'))
    assert len(summary) == 31 and all(float(s) > 0 for _, s in summary)
    clock = 'emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk'
    paths, coverage = [], {}
    for corner in sorted(readers.CORNERS):
        for mode in readers.MODES:
            n = f'core/native59-{mode}-{corner}.rpt'
            paths.append(readers.timed(text(n), n))
        n = f'core/native59-ucp-{corner}.rpt'
        coverage[corner] = ucp(text(n))
        assert coverage[corner] == ucp((parent / n).read_text())
        for prefix, labels in [('native-ready2', ['predict', 'gate', 'step']),
                              ('burst', ['internal', 'fill', 'cached-read', 'tag-select', 'input-capture', 'request', 'completion', 'first-fault', 'selftest-internal', 'selftest-input', 'selftest-output', 'trace-capture']),
                              ('storage-sample', ['input', 'output'])]:
            for label in labels:
                for mode in ['setup', 'hold']:
                    n = f'core/{prefix}-{label}-{mode}-{corner}.rpt'
                    t = text(n)
                    paths.append(readers.timed(t, n))
                    if prefix == 'storage-sample':
                        sample_paths(t, label, mode, clock)
    assert len(paths) == 332
    clocks = text('core/native-clocks.rpt')
    for name in [clock, 'dorado_sdram_clk']:
        rows = [s for s in clocks.splitlines() if s.startswith('; ' + name + ' ')]
        assert len(rows) == 1 and re.search(r'; Generated\s*; 17\.500\s*;', rows[0])
    for key, value, count in [('HOST_CLOCK_HZ', 57142857, 1), ('HOST_TIMEOUT_CLOCKS', 228571428, 1),
                              ('HOST_ADDR_SAMPLE', 1, 128), ('DIRTY_BLOCK_WINDOW', 1, 1), ('SNAPSHOT_PAUSE', 0, 1)]:
        values = re.findall(r';\s*' + key + r'\s*;\s*(\d+)\s*;', mapping)
        assert len(values) == count and all(int(v) == value for v in values), key
    command = dict((name, a + b + c) for name, a, b, c in re.findall(r'; command\.(\w+)\s*; ([01])\s*; ([01])\s*; ([01])\s*;', mapping))
    assert command == dict(MODE='000', REFRESH='001', PRECHARGE='010', ACTIVE='011', WRITE='100', READ='101', NOP='111')
    used = set()
    table = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n', 1)[0]
    for line in table.splitlines():
        f = [x.strip() for x in line.split(';')[1:-1]]
        if len(f) == 5 and f[1] == 'yes':
            used.add(f[3].replace(r['project'] + '/', ''))
    assert set(r['modified_sources']) <= used
    ready = text('storage-ready-reports-v1/ready.log')
    counts = re.findall(r'^STORAGE_SAMPLE_CORNER (\S+) (\d+) (\d+)$', ready, re.M)
    assert len(counts) == 4 and {c for c, _, _ in counts} == readers.CORNERS
    assert all(int(n) == 2048 and int(m) >= 2048 for _, n, m in counts)
    names = re.findall(r'^STORAGE_SAMPLE_REGISTER (.+)$', ready, re.M)
    scope = 'emu:emu|dorado_native_machine:u_native|dorado_native_backplane:m|'
    wanted = {scope + f'msa_ddram:b_{b}|dorado_storage_group:u_storage_{g}|addr_before_host[{bit}]'
              for b in ['msa', 'msaOdd', 'msa1', 'msa1Odd', 'msa2', 'msa2Odd', 'msa3', 'msa3Odd']
              for g in range(16) for bit in range(16)}
    assert set(names) == wanted and set(Counter(names).values()) == {4}
    ready_names = re.findall(r'^NATIVE_READY_REGISTER (.+)$', ready, re.M)
    original_ready = (parent / 'storage-ready-reports-v2/ready.log').read_text()
    assert Counter(ready_names) == Counter(re.findall(r'^NATIVE_READY_REGISTER (.+)$', original_ready, re.M))
    assert re.findall(r'^NATIVE_STEP_REGISTER (.+)$', ready, re.M) == ['emu:emu|network_step'] * 4
    inventory = text('storage-ready-reports-v1/inventory.log')
    original = (parent / 'storage-ready-reports-v2/inventory.log').read_text()
    registers = {}
    for label, count in re.findall(r'^SELFTEST_FIT_COUNT (\S+) (\d+)$', inventory, re.M):
        names = re.findall(r'^SELFTEST_FIT_REGISTER ' + label + r' (.+)$', inventory, re.M)
        prior = re.findall(r'^SELFTEST_FIT_REGISTER ' + label + r' (.+)$', original, re.M)
        canonical = lambda xs: {re.sub(r'~DUPLICATE(?:_\d+)?$', '', n) for n in xs}
        assert len(names) == len(set(names)) == int(count) and canonical(names) == canonical(prior)
        assert all(re.sub(r'~DUPLICATE(?:_\d+)?$', '', n) in names for n in names)
        registers[label] = names
    assert set(registers) == {'trace', 'observer', 'selftest', 'response'}

    warnings = {}
    def normalized(row):
        code, message = row
        message = re.sub(r'/home/alans/mister/Dorado_\w+_202610\d+', 'PROJECT', message)
        message = re.sub(r'(at [^ ()]+)\(\d+\)', r'\1(LINE)', message)
        return code, re.sub(r' Line: \d+', ' Line: LINE', message)
    for n in ['map.log', 'fit.log', 'sta.log']:
        t = text(n)
        assert 'Critical Warning' not in t and not re.search(r'^Error', t, re.M)
        previous = Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', (parent / n).read_text(), re.M)))
        current = Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', t, re.M)))
        removed, added = previous - current, current - previous
        if n == 'map.log':
            assert removed == Counter({('12241', '2274 hierarchies have connectivity warnings - see the Connectivity Checks report folder'): 1})
            assert added == Counter({('12241', '2275 hierarchies have connectivity warnings - see the Connectivity Checks report folder'): 1})
        else:
            assert not removed and not added
        warnings[n] = dict(removed=list(removed.items()), added=list(added.items()))
    prior_mapping = (parent / 'core/output_files/Dorado.map.rpt').read_text(encoding='latin-1')
    headers = lambda t: set(re.findall(r'^; Port Connectivity Checks: "([^"]+)"', t, re.M))
    extra = 'emu:emu|dorado_native_machine:u_native|dorado_pack_hps:u_media'
    assert headers(mapping) - headers(prior_mapping) == {extra} and not headers(prior_mapping) - headers(mapping)
    ports = mapping[mapping.index('; Port Connectivity Checks: "' + extra + '"'):].split('\n\n', 1)[0]
    fields = [s.strip() for line in ports.splitlines() for s in line.split(';')[1:2] if 'snapshot_' in s]
    assert set(fields) == {'snapshot_hold', 'snapshot_guest_held', 'snapshot_idle', 'snapshot_held'}
    for n in ['clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings', 'storage-ready', 'storage-ready-reports-v1/ready']:
        assert not re.search(r'^(?:Critical )?Warning|^Error', text(n + '.log'), re.M), n
    for n, count in [('backend', 4), ('inventory', 1)]:
        rows = re.findall(r'^Warning \((\d+)\): (.+)$', text('storage-ready-reports-v1/' + n + '.log'), re.M)
        assert len(rows) == count and all(c == '332174' and 'burst_observation* could not be matched with a register' in m for c, m in rows)
    fitting_summary = text('core/output_files/Dorado.fit.summary')
    assert 'Device : 5CSEBA6U23I7' in fitting_summary and 'Timing Models : Final' in fitting_summary
    resources = {}
    for label, key in [('Logic utilization (in ALMs)', 'alms'), ('Total registers', 'registers'), ('Total RAM Blocks', 'ram_blocks'), ('Total DSP Blocks', 'dsp_blocks')]:
        resources[key] = int(re.search(re.escape(label) + r'\s*:\s*([\d,]+)', fitting_summary)[1].replace(',', ''))
    assert resources['ram_blocks'] == 332 and resources['dsp_blocks'] == 35
    return dict(status='PASS_NATIVE_PACK_WINDOW_PREASSEMBLY_REVIEW', scope=__doc__,
                source_hashes=actual, reviewed_evidence_hashes=evidence, recipe_inputs=inputs,
                recipe_config_sha256=sha(config.read_bytes()), frequency_hz=57142857,
                sysper=4, state_admin='INACTIVE', timing_checks=summary, detailed_paths=paths,
                coverage=coverage, memory=mem, warnings=warnings, resources=resources,
                sample_registers=2048, ready_registers=8, registers=registers, command_encoding=command,
                minimum_setup_ns=min(p['slack_ns'] for p in paths if '-setup-' in p['report']),
                minimum_hold_ns=min(p['slack_ns'] for p in paths if '-hold-' in p['report']),
                assembly_permitted=False, firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root', 'recipe', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    result = review(a.root.resolve(), a.recipe.resolve())
    assert not a.output.exists()
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(result['detailed_paths']), result['minimum_setup_ns'], result['minimum_hold_ns'])
