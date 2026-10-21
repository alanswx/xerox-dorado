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
HOLD_MARGIN_SDC = '''# Additional hold-closure target; all original SDC files remain intact.
# Quartus17 -add preserves the derived uncertainty. Same-edge transfers
# include the host old_data -> gated machine read_pipe path.
set r_front_hold_clock [get_clocks {emu|pll|pll_inst|altera_pll_i|general?0?.gpll~PLL_OUTPUT_COUNTER|divclk}]
if {[get_collection_size $r_front_hold_clock]!=1} {error "Expected exactly one50MHz core clock"}
foreach_in_collection r_front_clock $r_front_hold_clock {
 if {[get_clock_info -period $r_front_clock]!=20.000} {error "Hold margin expects unchanged50MHz clock"}
}
set_clock_uncertainty -hold -enable_same_physical_edge -add 0.100 -from $r_front_hold_clock -to $r_front_hold_clock
unset r_front_hold_clock
'''


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


def expose_native_visible_r(text,board):
    """Export original latch Qs through synthesizable board ports."""
    assert board in('ProcH','ProcL')
    needle='    input wire [751:0] r_front_q,'
    assert text.count(needle)==1 and text.count('endmodule')==1
    text=text.replace(needle,'    output wire [7:0] r_front_visible_r,\n'+needle)
    first=0 if board=='ProcH'else 8
    assignments='\n'.join(f'  assign r_front_visible_r[{i}]=R_{first+i:02d};'for i in range(8))
    return text.replace('endmodule',assignments+'\nendmodule')


def canonical_visible_r_state(body):
    """Keep the original R-output latches as the sole owner of bits572..587.

    Their LOW-transparent clock is sampled by the retained native cell model,
    like the B-selector and MapBuf receivers. Exposing the evaluator's resolved
    R output early mixes those epochs. The evaluator reads the physical Qs;
    its predicted R result is combinational and is not stored a second time.
    """
    declaration='  reg [751:0] r_front_q;'
    update='    r_front_q<=r_front_next;'
    assert body.count(declaration)==body.count(update)==1
    # The physical inventory's raw order is R00 at572 through R15 at587.
    for board in ['ProcH','ProcL']:
        needle=f'{board}_full_snapshot_experiment b_{board} ('
        assert body.count(needle)==1
        body=body.replace(needle,needle+f'\n    .r_front_visible_r(r_front_visible_r_{board}),')
    body=body.replace(declaration,'''  wire [751:0] r_front_q;
  reg [571:0] r_front_lower_q;
  reg [163:0] r_front_upper_q;
  // 736 evaluator-owned bits plus16 ORIGINAL visible-R latch bits.
  // No duplicate architectural R storage or additional pipeline stage.
  wire [7:0] r_front_visible_r_ProcH,r_front_visible_r_ProcL;
  wire [15:0] r_front_physical_r={r_front_visible_r_ProcL,r_front_visible_r_ProcH};
  assign r_front_q={r_front_upper_q,r_front_physical_r,r_front_lower_q};''')
    return body.replace(update,'''    r_front_lower_q<=r_front_next[571:0];
    r_front_upper_q<=r_front_next[751:588];''')


def prepare(native, qualified, inventory, out, project, preserve_native_r=False,hold_margin_ps=0):
    assert hold_margin_ps in(0,100)and(not hold_margin_ps or preserve_native_r)
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
    retained_r=[row for row in inv['packages']if row['board']in('ProcH','ProcL')
                and row['package']in('g06','g07')]
    assert len(retained_r)==4 and all(row['kind']=='MC10173'for row in retained_r)
    assert sorted(bit for row in retained_r for bit in row['global_Q_bits_lsb_first'])==list(range(572,588))
    for row in retained_r:
        for pin,bit in zip([1,2,15,14],row['global_Q_bits_lsb_first']):
            assert row['physical_pin_connections'][str(pin)]==f'R_{bit-572:02d}'
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
                if preserve_native_r and row in retained_r:
                    continue
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
        if preserve_native_r and board in('ProcH','ProcL'):
            boards[board]=expose_native_visible_r(boards[board],board)
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
    if preserve_native_r:
        bp=canonical_visible_r_state(bp)
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
    if hold_margin_ps:
        outputs['core/r-front-hold-margin.sdc']=HOLD_MARGIN_SDC.encode()
        outputs[qsf]+=b'set_global_assignment -name SDC_FILE r-front-hold-margin.sdc\n'
    for n, raw in outputs.items():
        p = out / n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(raw)
    result = dict(status='STAGED_R_FRONT_READY_NATIVE_EXPERIMENT', scope=__doc__,
        project=str(project), source_hashes={n: sha(raw) for n, raw in outputs.items()},
        baseline_source_hashes=record['source_hashes'], state_admin='INACTIVE',
        frequency_mhz=50, sysper=4, physical_bits=752, ram_bits=9216,
        replaced_storage_packages=87 if preserve_native_r else 91, replaced_RAM_packages=12,
        preserved_visible_R_packages=4 if preserve_native_r else 0,
        evaluator_owned_Q_bits=736 if preserve_native_r else 752,
        native_visible_R_bits=16 if preserve_native_r else 0,
        internal_predicted_R_duplicate=False,
        additional_hold_uncertainty_ps=hold_margin_ps,
        live_producers=producers, unused_input_positions=[i for i in range(246) if i not in producers],
        qualification_input_hashes=gate['inputs'], native_inventory_sha256=sha((inventory/'results.json').read_bytes()),
        firmware_qualified=False, full_timing_qualified=False, hardware_changed=False,
        architectural_pipeline_stages_added=0, snapshot_abi_supported=False)
    (out / 'r-front-ready-native.json').write_text(json.dumps(result, indent=2) + '\n')
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print(result['status'],result['replaced_storage_packages'],
          'Q/storage +12 RAM packages replaced; unchanged50MHz/SYSPER4')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['native', 'qualified', 'inventory', 'output', 'project']:
        p.add_argument('--' + name, type=Path, required=True)
    p.add_argument('--preserve-native-r',action='store_true',help='Retain the four original visible-R latches as canonical state')
    p.add_argument('--hold-margin-ps',type=int,choices=[0,100],default=0,help='Add100ps to the original50MHz hold target, preserving all original constraints')
    a = p.parse_args()
    assert a.project.is_absolute()
    prepare(a.native.resolve(), a.qualified.resolve(), a.inventory.resolve(), a.output.resolve(), a.project,a.preserve_native_r,a.hold_margin_ps)
