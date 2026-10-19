#!/usr/bin/env python3
"""Independently check enabled board-state wiring against the frozen live core.

This audits source integration, not fitting, whole-machine snapshot completeness,
or equivalence of the already separately qualified board adapters.
"""
import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

BP = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
WRAPPER = 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
CORE = 'core/Dorado.sv'
BOARD_WIDTHS = [('ContA', 12), ('ContB', 15), ('ProcH', 16),
                ('ProcL', 16), ('IFU', 18), ('MemC', 16), ('MemD', 20),
                ('MemX', 24), ('DskEth', 13), ('DispY', 19), ('BaseBd', 17)]
SIGNALS = ('freeze', 'guest_held', 'request', 'write', 'board', 'index',
           'data', 'ready', 'done', 'error', 'result', 'frozen', 'fault')


def check(ok, why):
    if not ok:
        raise ValueError(why)


def compact(text):
    return re.sub(r'\s+', '', re.sub(r'//[^\n]*|/\*.*?\*/', '', text, flags=re.S))


def ports(text, instance):
    starts = list(re.finditer(r'\b' + re.escape(instance) + r'\s*\(', text))
    check(len(starts) == 1, 'instance missing/duplicated: ' + instance)
    start = starts[0].end()
    end = text.index(');', start)
    body = text[start:end]
    result = []
    for m in re.finditer(r'\.([A-Za-z_]\w*)\s*\(', body):
        pos, depth = m.end(), 1
        while depth and pos < len(body):
            depth += (body[pos] == '(') - (body[pos] == ')')
            pos += 1
        check(depth == 0, 'unbalanced port: ' + instance)
        result.append((m[1], compact(body[m.end():pos-1])))
    return result


def audit(baseline, candidate, receipt, replacements=None):
    replacements = replacements or {}

    def read(name):
        return replacements.get(name, (candidate/name).read_text())

    checked = 0
    for name, expected in receipt.items():
        p = baseline/name
        check(p.is_file() and hashlib.sha256(p.read_bytes()).hexdigest() == expected,
              'frozen baseline receipt mismatch: ' + name)
        if name.startswith('src/') and name not in (BP, WRAPPER):
            q = candidate/name
            check(q.is_file() and hashlib.sha256(q.read_bytes()).hexdigest() == expected,
                  'unchanged source differs: ' + name)
        checked += 1
    bp = read(BP)
    original_bp = (baseline/BP).read_text()
    total_pins = 0
    for ident, (name, width) in enumerate(BOARD_WIDTHS):
        actual = ports(bp, 'b_' + name)
        old = ports(original_bp, 'b_' + name)
        normal = lambda items: [(p, v) for p, v in items if not p.startswith('state_')]
        check(normal(actual) == normal(old), 'original board pins differ: ' + name)
        total_pins += len(normal(old))
        state = [(p, v) for p, v in actual if p.startswith('state_')]
        expected = {
            'state_host_clk': 'storage_clk', 'state_guest_advance': 'contb_advance',
            'state_freeze': 'snapshot_freeze', 'state_guest_held': 'snapshot_guest_held',
            'state_request': f'snapshot_board_request[{ident}]',
            'state_write': 'snapshot_write', 'state_index': f'snapshot_index[{width-1}:0]',
            'state_data': 'snapshot_data', 'state_frozen': f'snapshot_frozen[{ident}]',
            'state_done': f'snapshot_board_done[{ident}]',
            'state_error': f'snapshot_board_error[{ident}]',
            'state_result': f'snapshot_board_result[{ident*18}+:18]',
        }
        check(len(state) == len(expected) and dict(state) == expected,
              'board state binding differs: ' + name)
    check('BaseBd_full_snapshot_experiment#(.SYSPER(SYSPER))b_BaseBd(' in compact(bp),
          'BaseBoard clock parameter differs')
    fabric = dict(ports(bp, 'u_snapshot_fabric'))
    expected_fabric = {p: 'snapshot_' + p for p in
                       ('freeze', 'guest_held', 'request', 'write', 'board', 'index',
                        'data', 'ready', 'done', 'error', 'result', 'fault')}
    expected_fabric.update(clk='storage_clk', reset='storage_reset',
                           board_frozen='snapshot_frozen', board_done='snapshot_board_done',
                           board_error='snapshot_board_error', board_result='snapshot_board_result',
                           board_request='snapshot_board_request')
    check(fabric == expected_fabric, 'fabric binding differs')
    def ordinary_backplane(text):
        text = re.sub(r'\.(?:state_\w+)\([^()]*\)\s*,?', '', text)
        text = re.sub(r'^\s*(?:input|output) wire [^\n]*snapshot_[^\n]*\n', '', text, flags=re.M)
        text = re.sub(r'\s*wire \[10:0\] snapshot_board_request[^;]*;\s*wire \[197:0\] snapshot_board_result;\s*'
                      r'dorado_board_state_fabric u_snapshot_fabric\(.*?\);', '', text, flags=re.S)
        return compact(text.replace('BaseBd_full_snapshot_experiment', 'BaseBd_m_Rev_m_Am_nominal'))
    check(ordinary_backplane(bp) == ordinary_backplane(original_bp),
          'ordinary backplane logic differs')
    for path, instance, prefix in ((WRAPPER, 'm', 'snapshot_'), (CORE, 'u_native', 'board_state_')):
        actual = ports(read(path), instance)
        original = ports((baseline/path).read_text(), instance)
        check([(p, v) for p, v in actual if not p.startswith('snapshot_')] == original,
              'normal machine binding differs: ' + instance)
        state = [(p, v) for p, v in actual if p.startswith('snapshot_')]
        check(len(state) == len(SIGNALS) and dict(state) ==
              {'snapshot_' + p: prefix + p for p in SIGNALS},
              'machine state binding differs: ' + instance)
    core = read(CORE)
    old_core = (baseline/CORE).read_text()
    for inst in ('u_network_uio', 'u_network_ddr'):
        check(ports(core, inst) == ports(old_core, inst), 'Ethernet binding differs: ' + inst)
    text = compact(core)
    for required in (
        'wiremachine_enable=~reset&&pack_ready&&storage_ready&&!board_state_freeze;',
        'always@(negedgeclk_sys)network_step<=machine_enable;',
        'assignboard_state_guest_held=board_state_freeze&&!network_step;',
        'assignethernet_ext_bus[35:33]=dorado_ext_bus[35:33];',
        'assignethernet_ext_bus[31:16]=dorado_ext_bus[31:16];',
        'assignboard_state_ext_bus[35:33]=dorado_ext_bus[35:33];',
        'assignboard_state_ext_bus[31:16]=dorado_ext_bus[31:16];',
        'assigndorado_ext_bus[32]=ethernet_ext_bus[32]|board_state_ext_bus[32];',
        "assigndorado_ext_bus[15:0]=ethernet_ext_bus[32]?ethernet_ext_bus[15:0]:board_state_ext_bus[32]?board_state_ext_bus[15:0]:16'b0;",
        '.EXT_BUS(dorado_ext_bus)',
    ):
        check(text.count(required) == 1, 'core boundary/bus differs: ' + required)
    uio = dict(ports(core, 'u_board_state_uio'))
    expected_uio = {p: 'board_state_' + p for p in
                    ('freeze', 'guest_held', 'board_frozen', 'request', 'write', 'board',
                     'index', 'data', 'ready', 'done', 'error', 'result', 'fault')}
    expected_uio['board_frozen'] = 'board_state_frozen'
    expected_uio.update(clk='clk_sys', reset='reset', bus='board_state_ext_bus',
                        busy='', complete='', txn_sequence='', control_errors='')
    check(uio == expected_uio, 'UIO binding differs')
    unchanged_core = 0
    special_core = {'Dorado.sv', 'Dorado.qsf', 'device-corners.tcl', 'audit-adapter-crossings.tcl',
                    'Dorado.sdc', 'native_timing.sdc', 'sdram_timing.sdc', 'checkpoint_timing.sdc'}
    for p in (baseline/'core').rglob('*'):
        relative = p.relative_to(baseline/'core')
        if (not p.is_file() or str(relative) in special_core
                or set(relative.parts) & {'db', 'incremental_db', 'output_files'}
                or p.suffix in ('.rpt', '.summary', '.log') or p.name == 'c5_pin_model_dump.txt'):
            continue
        name = str(p.relative_to(baseline))
        raw = p.read_bytes()
        if p.suffix in ('.qsf', '.qip', '.sdc', '.tcl', '.qpf'):
            old = raw.decode().replace(str(baseline), str(candidate))
            old = old.replace('/home/alans/mister/Dorado_Native59_20261019', str(candidate))
            check(read(name) == old, 'unchanged core source differs: ' + name)
        else:
            check((candidate/name).read_bytes() == raw, 'unchanged core source differs: ' + name)
        unchanged_core += 1
    for name in ('Dorado.sdc', 'native_timing.sdc', 'sdram_timing.sdc', 'checkpoint_timing.sdc'):
        old = (baseline/'core'/name).read_text().replace(str(baseline), str(candidate))
        old = old.replace('/home/alans/mister/Dorado_Native59_20261019', str(candidate))
        check(read('core/' + name) == old, 'timing constraint differs: ' + name)
    old_corners = (baseline/'core/device-corners.tcl').read_text().replace('native59-', 'enabled60-')
    check(read('core/device-corners.tcl') == old_corners, 'operating corner script differs')
    crossing = read('core/audit-adapter-crossings.tcl')
    crossing = crossing.replace('  {*|BaseBd_full_snapshot_experiment:b_BaseBd|*} basebd\n', '')
    check(crossing == (baseline/'core/audit-adapter-crossings.tcl').read_text().replace('native59-', 'enabled60-'),
          'crossing audit differs')
    old_qsf = (baseline/'core/Dorado.qsf').read_text().replace(str(baseline), str(candidate))
    old_qsf = old_qsf.replace('/home/alans/mister/Dorado_Native59_20261019', str(candidate))
    qsf = read('core/Dorado.qsf')
    check(qsf.startswith(old_qsf), 'existing project policy differs')
    added = re.findall(r'^set_global_assignment -name SYSTEMVERILOG_FILE (.+)$',
                       qsf[len(old_qsf):], re.M)
    check(len(added) == len(set(added)) and len(added) > 2, 'new project sources missing/duplicate')
    prepared = json.loads(read('src/fpga/mister/snapshot_enabled/assets.json'))
    expected_files = {str(candidate/'src/fpga/mister/snapshot_enabled'/n)
                      for n in prepared['assets'] if n not in prepared['shared']}
    # Quartus17 appends relative aliases for newly added absolute paths while
    # mapping. Accept exactly the original assignments, optionally followed by
    # one complete alias set; every alias must resolve to the same source.
    canonical = [str((candidate/'core'/n).resolve()) for n in added]
    counts = Counter(canonical)
    check(set(canonical) == expected_files, 'new project source paths differ')
    check(set(counts.values()) in ({1}, {2}), 'new source count differs')
    if set(counts.values()) == {2}:
        n = len(expected_files)
        check(set(added[:n]) == expected_files and
              set(added[n:]) == {'../src/fpga/mister/snapshot_enabled/' + Path(x).name for x in expected_files},
              'unexpected project source alias')
    for name, expected in prepared['assets'].items():
        path = prepared['shared'].get(name, 'fpga/mister/snapshot_enabled/' + name)
        check(hashlib.sha256((candidate/'src'/path).read_bytes()).hexdigest() == expected,
              'qualified adapter differs: ' + name)
    return dict(status='PASS', scope=__doc__, baseline_receipt_files=checked,
                boards=len(BOARD_WIDTHS), original_board_pins=total_pins,
                unchanged_core_files=unchanged_core,
                adapter_files=len(prepared['assets']), new_project_sources=len(expected_files),
                new_project_source_assignments=len(added),
                inspected_hashes={n: hashlib.sha256((candidate/n).read_bytes()).hexdigest()
                                  for n in (BP, WRAPPER, CORE, 'core/Dorado.qsf',
                                            'src/fpga/mister/snapshot_enabled/assets.json')})


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--baseline', type=Path, required=True)
    p.add_argument('--candidate', type=Path, required=True)
    p.add_argument('--receipt', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--mutations', action='store_true')
    a = p.parse_args()
    baseline, candidate = a.baseline.resolve(), a.candidate.resolve()
    receipt = json.loads(a.receipt.read_text())
    result = audit(baseline, candidate, receipt)
    controls = []
    if a.mutations:
        cases = [
            (BP, 'snapshot_board_request[10]', 'snapshot_board_request[0]', 'board state binding'),
            (BP, 'snapshot_index[23:0]', 'snapshot_index[22:0]', 'board state binding'),
            (BP, 'snapshot_board_result[180+:18]', 'snapshot_board_result[162+:18]', 'board state binding'),
            (BP, '.state_guest_advance(contb_advance)', ".state_guest_advance(1'b1)", 'board state binding'),
            (BP, 'BaseBd_full_snapshot_experiment #(.SYSPER(SYSPER))',
             'BaseBd_full_snapshot_experiment #(.SYSPER(16))', 'clock parameter'),
            (BP, '.board_frozen(snapshot_frozen)', ".board_frozen(11'h7ff)", 'fabric binding'),
            (WRAPPER, '.snapshot_guest_held(snapshot_guest_held)',
             ".snapshot_guest_held(1'b1)", 'machine state binding'),
            (CORE, '.snapshot_freeze(board_state_freeze)', ".snapshot_freeze(1'b0)", 'machine state binding'),
            (CORE, '&& !board_state_freeze', '', 'core boundary/bus'),
            (CORE, 'negedge clk_sys', 'posedge clk_sys', 'core boundary/bus'),
            (CORE, 'board_state_freeze && !network_step', 'board_state_freeze', 'core boundary/bus'),
            (CORE, 'ethernet_ext_bus[32]|board_state_ext_bus[32]', 'ethernet_ext_bus[32]', 'core boundary/bus'),
            (CORE, '.EXT_BUS(dorado_ext_bus)', '.EXT_BUS(ethernet_ext_bus)', 'core boundary/bus'),
            ('core/native_timing.sdc', '\n', '\nset_false_path -from [all_registers]\n', 'timing constraint'),
            ('core/Dorado.qsf', 'DORADO_STORAGE_INPUT_DELAY=2', 'DORADO_STORAGE_INPUT_DELAY=1', 'project policy'),
        ]
        for ident, (name, before, after, reason) in enumerate(cases):
            text = (candidate/name).read_text()
            check(before in text, 'control target absent: ' + before)
            changed = text.replace(before, after, 1)
            try:
                audit(baseline, candidate, receipt, {name: changed})
            except ValueError as e:
                check(reason in str(e), 'unexpected control rejection: ' + str(e))
                controls.append(dict(case=ident, path=name, before=before, after=after, rejection=str(e)))
            else:
                raise ValueError('broken integration accepted: ' + before)
    result['broken_controls'] = controls
    result['audit_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    a.output.write_text(json.dumps(result, indent=2) + '\n')
    print(f"INTEGRATION PASS: {result['boards']} boards, {result['original_board_pins']} original pins, "
          f"{result['baseline_receipt_files']} receipt files, {len(controls)} broken controls rejected")


if __name__ == '__main__':
    main()
