#!/usr/bin/env python3
"""Stage the qualified Ready/R-front slice in the actual Native59 wrappers.

This is an experimental normal-operation replacement, not a firmware/timing
qualification. Preserve unrelated board logic and all external devices. The
administrative state interface is inactive; selected history/scan slots are
not a supported snapshot ABI. No Main, daemon, clock or hardware changes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from fit_r_front_ready import RTL, quartus_cone
from sil_to_verilog import vname

ROOT = Path(__file__).resolve().parents[1]
BACKPLANE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def q_outputs(kind, bits):
    """Independent reference's physical bit order, never snapshot slot order."""
    if kind in ('MC10231', 'SE10231', 'MC1668'):
        assert len(bits) == 2
        return {2: bits[0], 3: '~' + bits[0], 15: bits[1], 14: '~' + bits[1]}
    order = {'F10000': [3, 2, 15, 14], 'MC10141': [14, 15, 2, 3],
             'MC10173': [1, 2, 15, 14], 'MC10176': [2, 3, 4, 13, 14, 15]}
    assert len(bits) == len(order[kind])
    return dict(zip(order[kind], bits))


def instance(text, row):
    pattern = (r'^  cell_' + row['kind'] + r'_snapshot[^\n]* ' +
               re.escape(row['native_instance']) + r'\s*\(\n(.*?)^  \);[^\n]*')
    matches = list(re.finditer(pattern, text, re.M | re.S))
    assert len(matches) == 1, (row['board'], row['package'])
    m = matches[0]
    pins = dict(re.findall(r'\.p(\d+)\(([^()]*)\)', m[1]))
    assert pins == {str(k): v for k, v in row['physical_pin_connections'].items()}
    return m


def replace_package(text, row):
    m = instance(text, row)
    pins = row['physical_pin_connections']
    pins = {int(k): v for k, v in pins.items()}
    lines = ['  // Qualified central storage replaces ' + row['native_instance'] + '.']
    if row['kind'] == 'F10145A':
        bits = row['global_Q_bits_lsb_first']
        words = ["{" + ','.join(f'r_front_q[{b}]' for b in reversed(bits[i:i+4])) + "}"
                 for i in range(0, 64, 4)]
        name = 'r_front_' + row['package']
        lines += [f'  wire [63:0] {name}=' + '{' + ','.join(reversed(words)) + '};',
                  f'  wire [3:0] {name}_a=' + '{' + ','.join(pins[p] for p in [10, 9, 7, 6]) + '};',
                  f'  wire [3:0] {name}_out=' + pins.get(3, "1'b0") + f" ? 4'b0 : {name}[4*{name}_a+:4];"]
        outputs = {p: f'{name}_out[{i}]' for i, p in enumerate([2, 1, 15, 14])}
    elif 'global_Q_bits_lsb_first' in row:
        outputs = q_outputs(row['kind'], [f'r_front_q[{b}]' for b in row['global_Q_bits_lsb_first']])
    else:
        half = 0 if row['board'] == 'ProcH' else 1
        if row['parity']:
            outputs = {15: f'({pins[7]} ? 1\'b0 : r_front_parity[{half}])'}
        else:
            sel = ' | '.join(pins[p] for p in [3, 4, 5, 6])
            outputs = {p: f'(({sel}) ? 1\'b0 : r_front_data[{8*half+bit}])'
                       for p, bit in zip([23, 22, 21, 20], row['component_word_bits'])}
    for p, value in outputs.items():
        if p in pins:
            lines.append(f'  assign {pins[p]}={value};')
    # Administrative paths are statically inactive in the native backplane.
    # Do not fabricate obsolete clock history or advertise snapshot support.
    for port in ['state_result', 'state_frozen', 'state_done', 'state_error']:
        targets = re.findall(r'\.' + port + r'\(([^()]*)\)', m[1])
        assert len(targets) <= 1
        if port == 'state_result':
            assert len(targets) == 1
        if targets and targets[0]:
            lines.append(f"  assign {targets[0]}='0;")
    return text[:m.start()] + '\n'.join(lines) + text[m.end():]


def native_net(text, net):
    """Select a declared native read net, including output-only contributions."""
    name = vname(net)
    declarations = re.findall(r'\b(?:wire|reg)\s+(?:\[[^\]]+\]\s*)?([A-Za-z0-9_]+)', text)
    assert len([n for n in [name, name + '__drv'] if n in declarations]) >= 1, (net, name)
    return name if name in declarations else name + '__drv'


def prepare(native, qualified, inventory, out, project):
    record = json.loads((native / 'mc1668-candidate.json').read_text())
    assert record['state_admin'] == 'INACTIVE' and record['frequency_mhz'] == 50 and record['sysper'] == 4
    for n, h in record['source_hashes'].items():
        assert sha((native / n).read_bytes()) == h, n
    gate = json.loads((qualified / 'results.json').read_text())
    assert gate['status'] == 'PASS_R_FRONT_READY_ARRIVAL_CONTRACT' and len(gate['profiles']) == 6
    for n, h in gate['inputs'].items():
        assert sha((qualified / 'executed-inputs' / n).read_bytes()) == h, n
    inv = json.loads((inventory / 'results.json').read_text())
    assert inv['status'] == 'PASS_R_FRONT_READY_NATIVE_INVENTORY' and inv['physical_Q_bits'] == 752 and inv['RAM_bits'] == 9216
    for n, h in inv['inputs'].items():
        assert sha((inventory / 'executed-inputs' / n).read_bytes()) == h, n
    manifest = json.loads((qualified / 'reference/manifest.json').read_text())
    paths = {row['board']: row['native_source'] for row in inv['packages']}
    boards = {name: (native / path).read_text() for name, path in paths.items()}
    assert {p: sha(boards[k].encode()) for k, p in paths.items()} == inv['native_board_identities']
    # Every source consumed by the evaluator comes from a named live board
    # net. Internally resolved and unused input positions remain explicit.
    producers = {}
    for b in manifest['boards']:
        board = b['board'].split('-Rev-')[0]
        for net, expression in b['boundary_bindings'].items():
            m = re.fullmatch(r'pins\[(\d+)\]', expression)
            if m:
                i = int(m[1])
                producers.setdefault(i, []).append(dict(board=board, net=net,
                    expression=native_net(boards[board], net)))
    assert all(i < 246 for i in producers)
    for board, text in boards.items():
        needle = '    input wire state_host_clk,state_guest_advance,state_freeze,state_guest_held,'
        assert text.count(needle) == 1
        text = text.replace(needle, '''    input wire [751:0] r_front_q,
    input wire [15:0] r_front_data,input wire [1:0] r_front_parity,
    output wire [245:0] r_front_pins,output wire r_front_alias_ok,
''' + needle)
        for row in inv['packages'] + inv['RAM_packages']:
            if row['board'] == board:
                text = replace_package(text, row)
        extra = []
        aliases = []
        for i in range(246):
            sources = [s for s in producers.get(i, []) if s['board'] == board]
            expr = sources[0]['expression'] if sources else "1'b0"
            extra.append(f'  assign r_front_pins[{i}]={expr};')
            aliases += [f'({expr} == {s["expression"]})' for s in sources[1:]]
        extra.append('  assign r_front_alias_ok=' + (' & '.join(aliases) if aliases else "1'b1") + ';')
        assert text.count('endmodule') == 1
        boards[board] = text.replace('endmodule', '\n'.join(extra) + '\nendmodule')
    bp = (native / BACKPLANE).read_text()
    # All three ordinary root clocks are supplied by the BaseBoard. Equality
    # is checked in the live harness, rather than inferred from signal names.
    for board in paths:
        needle = f'{board}_full_snapshot_experiment b_{board} ('
        assert bp.count(needle) == 1
        bp = bp.replace(needle, needle + f'''
    .r_front_q(r_front_q),.r_front_data(r_front_data),.r_front_parity(r_front_parity),
    .r_front_pins(r_front_pins_{board}),.r_front_alias_ok(r_front_alias_{board}),''')
    logic = '''
  // Physical state only; surrounding original combinational gates remain.
  reg [751:0] r_front_q;
  reg r_front_clock_before;
  reg [245:0] r_front_before;
  wire [751:0] r_front_next;
  wire [245:0] r_front_live;
  wire [245:0] r_front_pins_ContA,r_front_pins_ProcH,r_front_pins_ProcL;
  wire r_front_alias_ContA,r_front_alias_ProcH,r_front_alias_ProcL;
  wire r_front_alias_ok;
  wire [15:0] r_front_data;wire [1:0] r_front_parity;
  wire [58:0] r_front_events,r_front_repeated;
  wire r_front_unsettled;
  always @(posedge storage_clk) if(contb_advance) begin
    r_front_q<=r_front_next;
    r_front_before<=r_front_live;
    r_front_clock_before<=CLK_ca_p_;
  end
'''
    for i in range(246):
        sources = producers.get(i, [])
        expr = f'r_front_pins_{sources[0]["board"]}[{i}]' if sources else "1'b0"
        logic += f'  assign r_front_live[{i}]={expr};\n'
    aliases = ['r_front_alias_' + k for k in paths]
    aliases += ['(CLK_ca_p_ == CLK_ph_p_)', '(CLK_ca_p_ == CLK_pl_p_)']
    for i, sources in producers.items():
        aliases += [f'(r_front_pins_{s["board"]}[{i}] == r_front_live[{i}])' for s in sources[1:]]
    logic += '  assign r_front_alias_ok=' + ' & '.join(aliases) + ';\n'
    logic += '''  dorado_r_front_ready_step r_front_evaluator(
    .host_clk(storage_clk),.advance(contb_advance),.state_q(r_front_q),
    .clock_before(r_front_clock_before),.clock_after(CLK_ca_p_),
    .pins_before(r_front_before),.pins_after(r_front_live),
    .state_next(r_front_next),.clock_events(r_front_events),.repeated_events(r_front_repeated),
    .unsettled(r_front_unsettled),.file_read_data(r_front_data),.file_read_parity(r_front_parity));
'''
    bp = bp.replace('endmodule', logic + '\nendmodule', 1)
    assert not out.exists()
    outputs = {}
    for n in record['source_hashes']:
        raw = (native / n).read_bytes()
        if Path(n).suffix in {'.qsf', '.qip', '.qpf', '.sdc', '.tcl'}:
            raw = raw.replace(record['project'].encode(), str(project).encode())
        outputs[n] = raw
    outputs.update({paths[k]: v.encode() for k, v in boards.items()})
    outputs[BACKPLANE] = bp.encode()
    for n in RTL:
        outputs['src/verilog/rtl/' + n] = (qualified / 'executed-inputs/verilog/rtl' / n).read_bytes()
    outputs['src/verilog/rtl/dorado_r_front_cone.sv'] = quartus_cone((qualified / 'dorado_r_front_cone.sv').read_text()).encode()
    qsf = 'core/Dorado.qsf'
    outputs[qsf] += b'\n# Qualified Ready/front component experimental native integration\n'
    for n in [*RTL, 'dorado_r_front_cone.sv']:
        outputs[qsf] += f'set_global_assignment -name SYSTEMVERILOG_FILE {project}/src/verilog/rtl/{n}\n'.encode()
    for n, raw in outputs.items():
        p = out / n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    result = dict(status='STAGED_R_FRONT_READY_NATIVE_EXPERIMENT', scope=__doc__,
        project=str(project), source_hashes={n: sha(raw) for n, raw in outputs.items()},
        baseline_source_hashes=record['source_hashes'], state_admin='INACTIVE',
        frequency_mhz=50, sysper=4, physical_bits=752, ram_bits=9216,
        replaced_storage_packages=91, replaced_RAM_packages=12,
        live_producers=producers, unused_input_positions=[i for i in range(246) if i not in producers],
        qualification_input_hashes=gate['inputs'], native_inventory_sha256=sha((inventory/'results.json').read_bytes()),
        firmware_qualified=False, full_timing_qualified=False, hardware_changed=False,
        architectural_pipeline_stages_added=0, snapshot_abi_supported=False)
    (out / 'r-front-ready-native.json').write_text(json.dumps(result, indent=2) + '\n')
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(result['status'], '91 Q/storage +12 RAM packages replaced; unchanged50MHz/SYSPER4')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['native', 'qualified', 'inventory', 'output', 'project']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    assert a.project.is_absolute()
    prepare(a.native.resolve(), a.qualified.resolve(), a.inventory.resolve(), a.output.resolve(), a.project)
