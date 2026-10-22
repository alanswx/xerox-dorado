#!/usr/bin/env python3
"""Review response-overlap native sources, memory, timing and actual path coverage.

Require source regeneration, all 332 inherited categories plus 24 counter and
24 response-path categories across four conditions. Qualification here permits
neither an unchecked assembly nor a firmware/speed claim.
"""
import argparse
from collections import Counter
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import tempfile

from review_sdram_burst_native import memory_comparison, readers
from audit_native_conta import ucp
from review_pack_window_native import sample_paths
from review_storage_cache_native import canonical
from stage_storage_response_native import stage, WRAPPER

ROOT = Path(__file__).resolve().parents[1]
HOST_CLOCK = 'emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def review(root, config):
    evidence = {}

    def data(name):
        path = Path(name)
        assert not path.is_absolute() and '..' not in path.parts
        raw = (root / name).read_bytes()
        evidence[name] = sha(raw)
        return raw

    def text(name):
        return data(name).decode('latin1')

    def js(name):
        return json.loads(data(name))

    summary = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',
                         text('core/output_files/Dorado.sta.summary'))
    assert len(summary) == 31 and all(float(v) > 0 for _, v in summary), 'Native timing summary failed'
    r = js('storage-response-native.json')
    assert r['frequency_hz'] == 57142857 and r['sysper'] == 4 and r['state_admin'] == 'INACTIVE'
    assert r['response_overlap'] == 1 and r['raw_buffer_lines'] == 4
    assert not r['assembly_permitted'] and not r['native_timing_firmware_speed_qualified']
    actual = {n: sha(data(n)) for n in r['source_hashes']}
    assert len(actual) == 503 and actual == r['source_hashes']
    supplied = json.loads(config.read_text())
    assert set(supplied) == {'parent', 'qualification', 'generated', 'pins', 'board', 'parallel_lookup'}
    assert type(supplied['parallel_lookup']) is bool
    args = {k: Path(v).resolve() for k, v in supplied.items() if k != 'parallel_lookup'}
    args['parallel_lookup'] = supplied['parallel_lookup']
    assert r.get('parallel_lookup', False) == args['parallel_lookup']
    with tempfile.TemporaryDirectory(prefix='dorado-response-review-') as temporary:
        out = Path(temporary) / 'stage'
        with contextlib.redirect_stdout(io.StringIO()):
            stage(**args, out=out, project=Path(r['project']))
        assert json.loads((out / 'storage-response-native.json').read_text()) == r, 'Native recipe differs'
        assert {n: sha((out / n).read_bytes()) for n in actual} == actual
    assert set(r['modified_RTL_sources']) == {WRAPPER, 'src/fpga/mister/dorado_storage_arb.sv',
                                             'src/fpga/mister/dorado_storage_sdram_burst.sv'}
    parent = args['parent']
    q = json.loads(args['qualification'].read_text())
    build, reports, collection = js('run-state.json'), js('storage-ready-reports-v1/run-state.json'), js('collected-ready-input-check.json')
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [a['stage'] for a in build['stages']] == ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings', 'storage-ready']
    assert reports['status'] == 'TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED'
    assert [a['stage'] for a in reports['actions']] == ['ready', 'backend', 'inventory', 'cache-counter', 'response-overlap']
    assert all(a['returncode'] == 0 for a in build['stages'] + reports['actions'])
    pids = {build['builder_pid'], reports['driver_pid']} | {a['pid'] for a in build['stages'] + reports['actions']}
    assert set(collection['absent_pids']) == pids and collection['actual'] == actual
    assert sha(data('build_r_front_ready_native.py')) == build['builder_sha256']
    for n, h in reports['report_inputs'].items():
        assert sha(data('storage-ready-reports-v1/' + n)) == h
    for name in ['audit_storage_cache_native.tcl', 'audit_storage_response_native.tcl']:
        assert data('storage-ready-reports-v1/' + name) == (ROOT / 'tools' / name).read_bytes()

    mapping, fitting = text('core/output_files/Dorado.map.rpt'), text('core/output_files/Dorado.fit.rpt')
    memory = {}
    for label, name, fitted in [('mapping', 'Dorado.map.rpt', False), ('fitting', 'Dorado.fit.rpt', True)]:
        memory[label] = memory_comparison((parent / 'core/output_files' / name).read_text(encoding='latin1'),
                                          text('core/output_files/' + name), fitted, True)
    memory['cache'] = readers.native_cache_physical(fitting,
        js('src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD'], 'independent40')
    paths, coverage = [], {}
    inherited = [p['report'].removesuffix('.gz') for p in q['detailed_paths'][:332]]
    assert len(inherited) == len(set(inherited)) == 332
    for name in inherited:
        body = text(name)
        paths.append(readers.timed(body, name))
        if name.startswith('core/storage-sample-'):
            fields = name.split('-')
            sample_paths(body, fields[2], fields[3], HOST_CLOCK)
    for corner in sorted(readers.CORNERS):
        name = 'core/native59-ucp-' + corner + '.rpt'
        coverage[corner] = ucp(text(name))
        assert coverage[corner] == ucp((parent / name).read_text())
        for prefix, labels in [('cache-hit', ['counter', 'event', 'initialized']),
                              ('response', ['backend', 'groups', 'arbiter'])]:
            for label in labels:
                for mode in ['setup', 'hold']:
                    name = f'storage-ready-reports-v1/{prefix}-{label}-{mode}-{corner}.rpt'
                    paths.append(readers.timed(text(name), name))
    assert len(paths) == 380 and all(p['slack_ns'] > 0 for p in paths)
    clocks = text('core/native-clocks.rpt')
    for name in [HOST_CLOCK, 'dorado_sdram_clk']:
        rows = [s for s in clocks.splitlines() if s.startswith('; ' + name + ' ')]
        assert len(rows) == 1 and re.search(r'; Generated\s*; 17\.500\s*;', rows[0])
    for key, value, count in [('HOST_CLOCK_HZ', 57142857, 1), ('HOST_TIMEOUT_CLOCKS', 228571428, 1),
                              ('HOST_ADDR_SAMPLE', 1, 128), ('DIRTY_BLOCK_WINDOW', 1, 1),
                              ('SNAPSHOT_PAUSE', 0, 1), ('CACHE_LINES', 4, 1), ('RESPONSE_OVERLAP', 1, 2)]:
        values = re.findall(r';\s*' + key + r'\s*;\s*(\d+)\s*;', mapping)
        assert len(values) == count and all(int(v) == value for v in values), key
    command = {n: a+b+c for n, a, b, c in re.findall(r'; command\.(\w+)\s*; ([01])\s*; ([01])\s*; ([01])\s*;', mapping)}
    assert command == q['command_encoding']
    table = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n', 1)[0]
    used = {f[3].replace(r['project'] + '/', '') for line in table.splitlines()
            if len(f := [s.strip() for s in line.split(';')[1:-1]]) == 5 and f[1] == 'yes'}
    assert set(r['modified_RTL_sources']) <= used
    ready = text('storage-ready-reports-v1/ready.log')
    before_ready = (parent / 'storage-ready-reports-v1/ready.log').read_text()
    for label in ['STORAGE_SAMPLE_REGISTER', 'NATIVE_READY_REGISTER', 'NATIVE_STEP_REGISTER']:
        assert Counter(re.findall(r'^' + label + r' (.+)$', ready, re.M)) == Counter(re.findall(r'^' + label + r' (.+)$', before_ready, re.M)), label
    counts = re.findall(r'^STORAGE_SAMPLE_CORNER (\S+) (\d+) (\d+)$', ready, re.M)
    assert len(counts) == 4 and {c for c, _, _ in counts} == readers.CORNERS
    assert all(int(a) == 2048 and int(b) >= 2048 for _, a, b in counts)

    physical = canonical(re.findall(r'^BURST_REGISTER (.+)$', text('storage-ready-reports-v1/backend.log'), re.M))
    scope = 'emu:emu|dorado_native_machine:u_native|dorado_storage_sdram_burst:u_sdram|'
    buffer = {}
    for label, bits in [('line_words', {i for i in range(128) if i % 16 < 12}), ('line_tag', set(range(20)))]:
        expression = re.escape(scope + label) + r'\[\d+\]\[\d+\]'
        found = {n for n in physical if re.fullmatch(expression, n)}
        assert found == {scope + label + f'[{line}][{bit}]' for line in range(4) for bit in bits}, label
        buffer[label] = dict(bits=len(found), bits_per_line=len(bits))
    assert {n for n in physical if re.fullmatch(re.escape(scope) + r'line_valid\[\d+\]', n)} == {scope + f'line_valid[{i}]' for i in range(4)}
    for label in ['trace', 'observer', 'selftest', 'response']:
        expression = r'^SELFTEST_FIT_REGISTER ' + label + r' (.+)$'
        assert canonical(re.findall(expression, text('storage-ready-reports-v1/inventory.log'), re.M)) == canonical(re.findall(expression, (parent / 'storage-ready-reports-v1/inventory.log').read_text(), re.M)), label
    hit_log = text('storage-ready-reports-v1/cache-counter.log')
    hit_counts = re.findall(r'^CACHE_HIT_COUNTER_COUNT (\S+) (\d+)$', hit_log, re.M)
    assert len(hit_counts) == 4 and {c for c, _ in hit_counts} == readers.CORNERS
    wanted_hits = {f'emu:emu|dorado_speed_monitor:u_speed_monitor|causes.count[11][{i}]' for i in range(32)}
    family_log = text('storage-ready-reports-v1/response-overlap.log')
    families = {}
    for corner in sorted(readers.CORNERS):
        assert canonical(re.findall(r'^CACHE_HIT_COUNTER_REGISTER ' + re.escape(corner) + r' (.+)$', hit_log, re.M)) == wanted_hits
        families[corner] = {}
        for label in ['backend', 'groups', 'arbiter']:
            names = re.findall(r'^RESPONSE_FAMILY_REGISTER ' + re.escape(corner) + ' ' + label + r' (.+)$', family_log, re.M)
            counts = re.findall(r'^RESPONSE_FAMILY_COUNT ' + re.escape(corner) + ' ' + label + r' (\d+)$', family_log, re.M)
            assert len(counts) == 1 and int(counts[0]) == len(names) and len(names) == len(set(names)) > 0
            families[corner][label] = len(names)
            if label == 'groups':
                groups = {n.rsplit('|', 1)[0] for n in names}
                prefix = 'emu:emu|dorado_native_machine:u_native|dorado_native_backplane:m|'
                assert groups == {prefix + f'msa_ddram:b_{b}|dorado_storage_group:u_storage_{g}' for b in ['msa', 'msaOdd', 'msa1', 'msa1Odd', 'msa2', 'msa2Odd', 'msa3', 'msa3Odd'] for g in range(16)}

    def normalized(row):
        code, message = row
        message = re.sub(r'/home/alans/mister/Dorado_\w+_202610\d+', 'PROJECT', message)
        message = re.sub(r'(at [^ ()]+)\(\d+\)', r'\1(LINE)', message)
        return code, re.sub(r' Line: \d+', ' Line: LINE', message)
    warnings = {}
    for name in ['map.log', 'fit.log', 'sta.log']:
        body = text(name)
        assert 'Critical Warning' not in body and not re.search(r'^Error', body, re.M)
        before = Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', (parent / name).read_text(), re.M)))
        after = Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', body, re.M)))
        removed, added = before-after, after-before
        if name == 'map.log':
            def truncation(source, width):
                return ('10230', f'Verilog HDL assignment warning at {source}.sv(LINE): truncated value with size 32 to match size of target ({width}) File: PROJECT/src/fpga/mister/{source}.sv Line: LINE')
            assert removed == Counter({truncation('dorado_storage_sdram_burst', 4): 1})
            assert added == Counter({truncation('dorado_storage_sdram_burst', 2): 1, truncation('dorado_storage_arb', 7): 1})
            assert "next_line<=fill_line==CACHE_LINES-1 ? 0 : fill_line+1'b1;" in text('src/fpga/mister/dorado_storage_sdram_burst.sv')
            assert "next_group<=chosen_owner==GROUPS-1 ? 0 : chosen_owner+1'b1;" in text('src/fpga/mister/dorado_storage_arb.sv')
        else:
            assert not removed and not added, name
        warnings[name] = dict(removed=list(removed.items()), added=list(added.items()))
    for name in ['clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings', 'storage-ready', 'storage-ready-reports-v1/ready', 'storage-ready-reports-v1/cache-counter', 'storage-ready-reports-v1/response-overlap']:
        assert not re.search(r'^(?:Critical )?Warning|^Error', text(name + '.log'), re.M), name
    for name, count in [('backend', 4), ('inventory', 1)]:
        found = re.findall(r'^Warning \((\d+)\): (.+)$', text('storage-ready-reports-v1/' + name + '.log'), re.M)
        assert len(found) == count and all(code == '332174' and 'burst_observation* could not be matched with a register' in message for code, message in found)
    fit = text('core/output_files/Dorado.fit.summary')
    assert 'Device : 5CSEBA6U23I7' in fit and 'Timing Models : Final' in fit
    resources = {key: int(re.search(re.escape(label) + r'\s*:\s*([\d,]+)', fit)[1].replace(',', '')) for label, key in [('Logic utilization (in ALMs)', 'alms'), ('Total registers', 'registers'), ('Total RAM Blocks', 'ram_blocks'), ('Total DSP Blocks', 'dsp_blocks')]}
    assert resources['ram_blocks'] == 332 and resources['dsp_blocks'] == 35
    return dict(status='PASS_NATIVE_STORAGE_RESPONSE_PREASSEMBLY_REVIEW', scope=__doc__, source_hashes=actual,
                reviewed_evidence_hashes=evidence, recipe_config_sha256=sha(config.read_bytes()),
                frequency_hz=57142857, sysper=4, state_admin='INACTIVE', raw_buffer_lines=4,
                response_overlap=1, parallel_lookup=r.get('parallel_lookup', False),
                timing_checks=summary, detailed_paths=paths, coverage=coverage, memory=memory,
                buffer_registers=buffer, hit_counter_bits=32, response_families=families,
                warnings=warnings, resources=resources, command_encoding=command,
                minimum_setup_ns=min(p['slack_ns'] for p in paths if '-setup-' in p['report']),
                minimum_hold_ns=min(p['slack_ns'] for p in paths if '-hold-' in p['report']),
                assembly_permitted=False, firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'config', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    result = review(args.root.resolve(), args.config.resolve())
    assert not args.output.exists()
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(result['detailed_paths']), result['minimum_setup_ns'], result['minimum_hold_ns'])
