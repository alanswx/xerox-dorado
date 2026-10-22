#!/usr/bin/env python3
"""Review the faster native board-ready build before guarded assembly.

Regenerate the complete503-source recipe from qualified component inputs.
Require all original/ready/backend timing categories, unchanged logical RAM
contracts, the explicitly reviewed paired38 cache mapping, actual clock and
command encoding, terminal jobs and exact warning differences. Firmware,
physical memory and measured speed remain separate hardware gates.
"""
import argparse
from argparse import Namespace
import collections
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import tempfile

from review_sdram_burst_native import memory_comparison, readers, raw
from audit_r_front_ready_native import ram_table
from audit_native_conta import ucp
from stage_storage_board_native import stage

ROOT = Path(__file__).resolve().parents[1]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def paired_memory(before, after, manifest):
    """Classify precisely Native58's existing paired38 physical placement.

    Only four logical row block counts change: each shared3-bit pair lists
    its common three blocks instead of its earlier independent two. First
    prove geometry, capacity, disjoint groups and OLD_DATA with the original
    strict physical reader. Then compare all remaining RAM fields against
    the qualified parent. Never edit the actual reports or ignore a RAM mode.
    """
    cache = readers.native_cache_physical(after, manifest, 'paired38')
    old = ram_table(before, 'Fitter RAM Summary')
    new = ram_table(after, 'Fitter RAM Summary')
    changed = {}
    for n, row in old.items():
        if n in new and row['M10K blocks'] != new[n]['M10K blocks'] and ':b_MemD|' in n:
            bank = int(re.search(r'snapshot_bank_(\d+)\|', n)[1])
            assert bank in {3, 5, 8, 10}
            assert row['M10K blocks'] == '2' and new[n]['M10K blocks'] == '3'
            changed[n] = bank
    assert set(changed.values()) == {3, 5, 8, 10}
    header = None
    lines = []
    for line in before.splitlines(keepends=True):
        fields = line.split(';')
        values = [x.strip() for x in fields[1:-1]]
        if values and values[0] == 'Name' and 'M10K blocks' in values:
            header = values
        if values and values[0] in changed:
            assert header and len(header) == len(values)
            index = header.index('M10K blocks') + 1
            assert fields[index].strip() == '2'
            fields[index] = fields[index].replace('2', '3')
            line = ';'.join(fields)
        lines.append(line)
    result = memory_comparison(''.join(lines), after, True, True)
    occupied = {site for group in cache['groups'] for site in group['locations']}
    for n, row in new.items():
        if ':b_MemD|' not in n:
            assert not occupied.intersection(x.strip() for x in row['Location'].split(',')), n
    result['cache_repacking'] = dict(previous_unique_m10ks=40, current_unique_m10ks=38,
                                    changed_logical_rows=changed, physical=cache)
    return result


def review(root, config):
    root = root.resolve()
    evidence = {}

    def read(n):
        b = (root / n).read_bytes()
        evidence[n] = sha(b)
        return b

    def js(n):
        return json.loads(read(n))

    r = js('storage-board-native.json')
    assert r['frequency_hz'] == 57142857 and r['sysper'] == 4
    assert r['state_admin'] == 'INACTIVE' and r['CPU_cache_map_ECC_peripheral_logic_unchanged']
    assert not r['assembly_permitted'] and not r['firmware_qualified']
    actual = {n: sha(read(n)) for n in r['source_hashes']}
    assert actual == r['source_hashes'] and len(actual) == 503
    inputs = json.loads(config.read_text())
    with tempfile.TemporaryDirectory(prefix='dorado-board-native-review-') as d:
        args = {n: Path(v).resolve() for n, v in inputs.items()}
        args.update(output=Path(d) / 'stage', project=Path(r['project']), clock_hz=r['frequency_hz'])
        with contextlib.redirect_stdout(io.StringIO()):
            stage(Namespace(**args))
        expected = json.loads((args['output'] / 'storage-board-native.json').read_text())
        assert expected == r, 'Native source recipe differs, including rehashed inputs'
        assert {n: sha((args['output'] / n).read_bytes()) for n in actual} == actual

    build = js('run-state.json')
    report = js('storage-ready-reports-v2/run-state.json')
    check = js('collected-ready-input-check.json')
    stages = ['map', 'fit', 'sta', 'clocks', 'corners', 'control-crossings',
              'contb-crossings', 'adapter-crossings', 'storage-ready']
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert [a['stage'] for a in build['stages']] == stages
    assert all(a['returncode'] == 0 for a in build['stages'])
    assert report['status'] == 'TERMINAL_STORAGE_READY_REPORTS_REVIEW_REQUIRED'
    assert [a['stage'] for a in report['actions']] == ['ready', 'backend', 'inventory']
    assert all(a['returncode'] == 0 for a in report['actions'])
    assert check['actual'] == actual
    pids = {build['builder_pid'], report['driver_pid']} | {
        a['pid'] for a in build['stages'] + report['actions']}
    assert set(check['absent_pids']) == pids
    assert sha(read('build_r_front_ready_native.py')) == build['builder_sha256']
    for n, h in report['report_inputs'].items():
        assert sha(read('storage-ready-reports-v2/' + n)) == h, n

    parent = args['parent'] / 'terminal-v1/receipt'
    mapping = read('core/output_files/Dorado.map.rpt').decode('latin-1')
    fitting = read('core/output_files/Dorado.fit.rpt').decode('latin-1')
    manifest = js('src/fpga/mister/snapshot_native_memd/assets.json')['manifests']['MemD']
    memory = dict(
        mapping=memory_comparison(raw(parent / 'core/output_files/Dorado.map.rpt').decode('latin-1'), mapping, False, True),
        fitting=paired_memory(raw(parent / 'core/output_files/Dorado.fit.rpt').decode('latin-1'), fitting, manifest))

    summary = re.findall(r'Type\s*:\s*([^\n]+)\nSlack\s*:\s*([-+\d.]+)',
                         read('core/output_files/Dorado.sta.summary').decode())
    assert len(summary) == 31 and all(float(v) > 0 for _, v in summary)
    paths = []
    coverage = {}
    for corner in sorted(readers.CORNERS):
        for mode in readers.MODES:
            n = f'core/native59-{mode}-{corner}.rpt'
            paths.append(readers.timed(read(n).decode(), n))
        n = f'core/native59-ucp-{corner}.rpt'
        coverage[corner] = ucp(read(n).decode())
        assert coverage[corner] == ucp(raw(parent / n).decode())
        for label in ['predict', 'gate', 'step']:
            for mode in ['setup', 'hold']:
                n = f'core/native-ready2-{label}-{mode}-{corner}.rpt'
                paths.append(readers.timed(read(n).decode(), n))
        for label in ['internal', 'fill', 'cached-read', 'tag-select', 'input-capture',
                      'request', 'completion', 'first-fault', 'selftest-internal',
                      'selftest-input', 'selftest-output', 'trace-capture']:
            for mode in ['setup', 'hold']:
                n = f'core/burst-{label}-{mode}-{corner}.rpt'
                paths.append(readers.timed(read(n).decode(), n))
    assert len(paths) == 316
    clocks = read('core/native-clocks.rpt').decode()
    for name in ['dorado_sdram_clk', 'emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk']:
        rows = [line for line in clocks.splitlines() if line.startswith('; ' + name + ' ')]
        assert len(rows) == 1 and re.search(r'; Generated\s*; 17\.500\s*;', rows[0]), name
    for parameter, value in [('HOST_CLOCK_HZ', 57142857), ('HOST_TIMEOUT_CLOCKS', 228571428)]:
        values = re.findall(r';\s*' + parameter + r'\s*;\s*(\d+)\s*;', mapping)
        assert len(values) == 1 and int(values[0]) == value, parameter
    encoding = mapping[mapping.index('Encoding Type:  User-Encoded'):].split('Encoding Type:', 1)[-1]
    encoding = encoding.split('Encoding Type:', 1)[0]
    assert '|dorado_storage_sdram_burst:u_sdram|command' in encoding
    command_bits = {}
    for name, a, b, c in re.findall(r'; command\.(\w+)\s*; ([01])\s*; ([01])\s*; ([01])\s*;', encoding):
        command_bits[name] = a + b + c
    assert command_bits == dict(MODE='000', REFRESH='001', PRECHARGE='010', ACTIVE='011', WRITE='100', READ='101', NOP='111')

    used = set()
    table = mapping[mapping.index('; Analysis & Synthesis Source Files Read'):].split('\n\n', 1)[0]
    for line in table.splitlines():
        f = [x.strip() for x in line.split(';')[1:-1]]
        if len(f) == 5 and f[1] == 'yes':
            used.add(f[3].replace(r['project'] + '/', ''))
    assert {n for n in r['modified_sources'] if Path(n).suffix in ('.sv', '.v')} <= used
    assert 'src/fpga/mister/dorado_storage_addr_preview.sv' in used

    ready = read('storage-ready-reports-v2/ready.log').decode()
    counts = re.findall(r'^NATIVE_READY_CORNER (\S+) (\d+)$', ready, re.M)
    assert len(counts) == 4 and {c for c, _ in counts} == readers.CORNERS
    assert all(int(n) == 8 for _, n in counts)
    names = re.findall(r'^NATIVE_READY_REGISTER (.+)$', ready, re.M)
    scope = 'emu:emu|dorado_native_machine:u_native|dorado_native_backplane:m|'
    wanted = {scope + f'msa_ddram:b_{board}|ready_q' for board in
              ['msa', 'msaOdd', 'msa1', 'msa1Odd', 'msa2', 'msa2Odd', 'msa3', 'msa3Odd']}
    assert set(names) == wanted and len(names) == 32
    assert all(n == 4 for n in collections.Counter(names).values())
    assert re.findall(r'^NATIVE_STEP_REGISTER (.+)$', ready, re.M) == ['emu:emu|network_step'] * 4
    old_ready = read('storage-ready.log').decode()
    assert collections.Counter(re.findall(r'^NATIVE_READY_REGISTER (.+)$', old_ready, re.M)) == collections.Counter(names)

    warnings = {}

    def normalized(row):
        code, text = row
        text = re.sub(r'/home/alans/mister/Dorado_\w+_202610\d+', 'PROJECT', text)
        text = re.sub(r'(at [^ ()]+)\(\d+\)', r'\1(LINE)', text)
        text = re.sub(r' Line: \d+', ' Line: LINE', text)
        return code, text

    for n in ['map.log', 'fit.log', 'sta.log']:
        text = read(n).decode()
        assert 'Critical Warning' not in text and not re.search(r'^Error', text, re.M)
        previous = collections.Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', raw(parent / n).decode(), re.M)))
        current = collections.Counter(map(normalized, re.findall(r'^Warning \((\d+)\): (.+)$', text, re.M)))
        assert not previous - current
        added = current - previous
        if n == 'map.log':
            assert len(added) == 1 and list(added.values()) == [2]
            code, text = next(iter(added))
            assert code == '10036' and 'object "ready_reference"' in text
        else:
            assert not added
        warnings[n] = dict(parent_count=sum(previous.values()), added=[
            dict(code=c, text=t, count=v) for (c, t), v in added.items()])
    for n in ['clocks', 'corners', 'control-crossings', 'contb-crossings', 'adapter-crossings',
              'storage-ready', 'storage-ready-reports-v2/ready']:
        assert not re.search(r'^(?:Critical )?Warning|^Error', read(n + '.log').decode(), re.M), n
    for n, count in [('backend', 4), ('inventory', 1)]:
        text = read('storage-ready-reports-v2/' + n + '.log').decode()
        rows = re.findall(r'^Warning \((\d+)\): (.+)$', text, re.M)
        assert len(rows) == count and all(c == '332174' and 'burst_observation* could not be matched with a register' in s for c, s in rows)
        warnings[n] = dict(unmatched_output_alias=count)
    inventory = read('storage-ready-reports-v2/inventory.log').decode()
    reference = (ROOT / 'build/rtl-executive/storage-ready-native-v1-20261022/terminal-v1/raw/storage-ready-reports-v1/inventory.log').read_text()
    registers = {}
    for label, count in re.findall(r'^SELFTEST_FIT_COUNT (\S+) (\d+)$', inventory, re.M):
        names = re.findall(r'^SELFTEST_FIT_REGISTER ' + label + r' (.+)$', inventory, re.M)
        original = re.findall(r'^SELFTEST_FIT_REGISTER ' + label + r' (.+)$', reference, re.M)
        assert len(names) == len(set(names)) == int(count)
        canonical = lambda items: {re.sub(r'~DUPLICATE(?:_\d+)?$', '', n) for n in items}
        assert canonical(names) == canonical(original), label
        assert all(re.sub(r'~DUPLICATE(?:_\d+)?$', '', n) in names for n in names), label
        registers[label] = names
    assert set(registers) == {'trace', 'observer', 'selftest', 'response'}
    fit_summary = read('core/output_files/Dorado.fit.summary').decode()
    assert 'Device : 5CSEBA6U23I7' in fit_summary and 'Timing Models : Final' in fit_summary
    resources = {}
    for label, name in [('Logic utilization (in ALMs)', 'alms'), ('Total registers', 'registers'),
                        ('Total RAM Blocks', 'ram_blocks'), ('Total DSP Blocks', 'dsp_blocks')]:
        match = re.search(re.escape(label) + r'\s*:\s*([\d,]+)', fit_summary)
        assert match, label
        resources[name] = int(match[1].replace(',', ''))
    assert resources['ram_blocks'] == 330 and resources['dsp_blocks'] == 35
    return dict(status='PASS_NATIVE_STORAGE_BOARD_PREASSEMBLY_REVIEW', scope=__doc__,
                source_hashes=actual, reviewed_evidence_hashes=evidence,
                recipe_inputs=inputs, recipe_config_sha256=sha(config.read_bytes()),
                frequency_hz=57142857, frequency_mhz=57.142857, sysper=4,
                state_admin='INACTIVE', timing_checks=summary, detailed_paths=paths,
                coverage=coverage, memory=memory, warnings=warnings,
                ready_registers=8, registers=registers, resources=resources, command_encoding=command_bits,
                minimum_setup_ns=min(p['slack_ns'] for p in paths if '-setup-' in p['report']),
                minimum_hold_ns=min(p['slack_ns'] for p in paths if '-hold-' in p['report']),
                assembly_permitted=False, firmware_qualified=False, hardware_speedup_qualified=False)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--recipe', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    result = review(a.root, a.recipe)
    assert not a.output.exists()
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(result['detailed_paths']), result['minimum_setup_ns'], result['minimum_hold_ns'])
