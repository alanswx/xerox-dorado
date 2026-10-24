#!/usr/bin/env python3
"""Test explicit local LCELL buffers on the measured cache-tag address path.

The shorter-control native fit still has321 address-source loads and fails
setup. Wire aliases did not create physical buffers. Vendor LCELL instances
buffer BOTH address polarities at21 tag RAMs. Optionally share each buffer
across a bounded cluster of RAMs to reduce routing/packing cost. Verify
all168 affected address pins and original unique output drivers. Preserve
all RAM ports, clocks, capture windows, state owners and timing constraints.

The exact installed vendor identity model is included for simulation/proof
ONLY, excluded from Quartus QSF/QIP. Quartus resolves its own LCELL primitive.
Actual preservation/fanout/area/timing must be read from the fitted netlist;
this preparation does not establish physical replication or permit assembly.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from stage_memc_tag_buffers import MEMC, CELL, PACKAGES, pins
from review_late_hold_and_input_reset import first_path
from sil_netlist import load_board
from sil_to_verilog import vname
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
SIM_MODEL = 'src/fpga/mister/snapshot_native_memc/lcell_sim.v'


def stage(parent, failed_fit, vendor, out, project, yosys, cluster_size=1):
    assert not out.exists() and project.is_absolute()
    assert cluster_size in [1, 2, 4]
    p = json.loads((parent/'r-front-control-native.json').read_text())
    assert p['frequency_hz'] == 57142857 and p['sysper'] == 4 and not p['assembly_permitted']
    assert p['shortened_control_terminal_wave']['compared_outputs'] == 363
    assert sha(parent/'r-front-control-native.json') == sha(failed_fit/'r-front-control-native.json')
    assert json.loads((failed_fit/'run-state.json').read_text())['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    critical = failed_fit/'core/native59-setup-7_slow_1100mv_100c.rpt'
    path = first_path(critical)
    assert path['slack_ns'] == -0.475 and 'u_d15|qb' in path['source'] and 'rm_l_rtl_0' in path['sink']
    assert 'u_c10|mem_rtl_0' in critical.read_text()
    for name, digest in p['source_hashes'].items():
        assert sha(parent/name) == sha(failed_fit/name) == digest, name
    v = json.loads(vendor.read_text())
    assert re.sub(r'\s+', ' ', v['model']).strip() == 'module lcell (in, out); input in; output out; assign out = in; endmodule'
    assert SIM_MODEL not in p['source_hashes']
    for name in p['source_hashes']:
        if Path(name).suffix in ['.qsf', '.qip']:
            assert 'lcell_sim' not in (parent/name).read_text()
    cell = (parent/CELL).read_text()
    assert 'assign p2 =  qa;  assign p3  = ~qa;' in cell and 'assign p15 = qb;  assign p14 = ~qb;' in cell
    original = (parent/MEMC).read_text()
    positive, inverted = {}, {}
    for index, package in enumerate(PACKAGES):
        row = pins(re.search(r'cell_MC1668_snapshot u_'+package+r' \(.*?\n  \);', original, re.S)[0])
        for offset, true_pin, inverse_pin in [(0, '2', '3'), (1, '15', '14')]:
            positive[row[true_pin]] = 2*index+offset
            inverted[row[inverse_pin]] = row[true_pin]
    assert len(positive) == len(inverted) == 16
    changed, bindings, declarations, old_addresses, new_addresses = original, [], [], [], []
    buffers = {}
    target_index = 0
    for match in re.finditer(r'cell_MB7071H_snapshot u_(\w+) \(.*?\n  \);', original, re.S):
        package, row = match[1], pins(match[0])
        if not any(row[str(pin)] in inverted for pin in range(7, 15)):
            continue
        cluster = target_index//cluster_size
        target_index += 1
        replacement, modified = match[0], False
        for pin in map(str, range(7, 15)):
            net = row[pin]
            assert net in inverted or net in positive, (package, pin, net)
            inverse = net in inverted
            source = inverted[net] if inverse else net
            expression = ('~' if inverse else '')+source
            key = (cluster, source, inverse)
            if key not in buffers:
                local = 'tag_lcell_'+package+'_'+pin if cluster_size == 1 else 'tag_lcell_g'+str(cluster)+'_'+net
                buffers[key] = local
                declarations += [f'  (* keep = "true" *) wire {local};',
                                 f'  lcell u_{local}(.in({expression}),.out({local}));']
            local = buffers[key]
            bindings.append(dict(package=package, pin=int(pin), original=net, source=source, local=local, inverted=inverse))
            replacement = replacement.replace(f'.p{pin}({net})', f'.p{pin}({local})', 1)
            modified = True
        if modified:
            assert changed.count(match[0]) == 1
            changed = changed.replace(match[0], replacement, 1)
            old_addresses.extend(row[str(pin)] for pin in range(7, 15))
            new_addresses.extend(pins(replacement)[str(pin)] for pin in range(7, 15))
    assert len(bindings) == len(old_addresses) == 168
    assert sum(row['inverted'] for row in bindings) == 84
    assert len({row['package'] for row in bindings}) == 21
    maximum_RAMs_per_buffer = max(len({row['package'] for row in bindings if row['local'] == local}) for local in buffers.values())
    assert maximum_RAMs_per_buffer <= cluster_size
    # The stopped v1 covered only inverse pins and missed THIS measured path.
    critical_binding = next(row for row in bindings if row['package'] == 'c10' and row['pin'] == 14)
    d15 = pins(re.search(r'cell_MC1668_snapshot u_d15 \(.*?\n  \);', original, re.S)[0])
    assert critical_binding['original'] == critical_binding['source'] == d15['15']
    assert not critical_binding['inverted']
    added = '\n'.join(declarations)+'\n'
    start = changed.index('  cell_MB7071H_snapshot ')
    changed = changed[:start]+added+changed[start:]
    restored = changed
    for row in bindings:
        restored = restored.replace(f'.p{row["pin"]}({row["local"]})', f'.p{row["pin"]}({row["original"]})', 1)
    assert restored.replace(added, '', 1) == original
    # Bind each complemented net to its ONE original physical output; never
    # replace one contributor to a wired-OR with the opposite latch output.
    wl = ROOT/'chm/sil/MemC-Rev-Be.dm!1_/MemC-Rev-Be.wl'
    board = load_board(str(wl))
    nets = {vname(name): (name, row) for name, row in board.nets.items()}
    audited = []
    for row in bindings:
        name, net = nets[row['original']]
        drivers = [pin for pin in net['pins'] if pin['dir'] == 'out']
        assert len(drivers) == 1 and drivers[0]['pin'] in ([3, 14] if row['inverted'] else [2, 15])
        driver = drivers[0]
        assert board.packages[driver['pkg']]['type'] == 'MC1668'
        source, net = nets[row['source']]
        true = [pin for pin in net['pins'] if pin['dir'] == 'out']
        assert len(true) == 1 and true[0]['pkg'] == driver['pkg']
        assert true[0]['pin'] == ({3: 2, 14: 15}[driver['pin']] if row['inverted'] else driver['pin'])
        audited.append(dict(original_net=name, inverse_driver=driver, source_net=source, source_driver=true[0]))
    out.mkdir(parents=True)
    (out/'vendor-lcell.json').write_bytes(vendor.read_bytes())
    for source in [wl, wl.with_suffix('.lc')]:
        (out/source.name).write_bytes(source.read_bytes())
    proof = out/'address-proof'
    proof.mkdir()
    common = ['module tag_address_equivalence(input wire [15:0] owners,output wire equivalent);']
    common += [f' wire {name}=owners[{index}];' for name, index in positive.items()]
    common += [f' wire {name}=~{source};' for name, source in inverted.items()]
    common += declarations
    common += [' wire [167:0] original_address={'+','.join(old_addresses)+'};',
               ' wire [167:0] next_address={'+','.join(new_addresses)+'};',
               ' assign equivalent=original_address==next_address;', 'endmodule']
    actual = '\n'.join(common)+'\n'
    rows = []
    for variant in ['positive', 'wrong-polarity']:
        d = proof/variant
        d.mkdir()
        raw = actual
        if variant != 'positive':
            first = critical_binding
            before = f'lcell u_{first["local"]}(.in({first["source"]}),.out({first["local"]}));'
            assert raw.count(before) == 1
            raw = raw.replace(before, before.replace('.in(', '.in(~'), 1)
        (d/'actual.sv').write_text(raw)
        (d/'lcell.v').write_text(v['model'])
        script = f'''read_verilog -sv /work/lcell.v /work/actual.sv
hierarchy -check -top tag_address_equivalence
proc
flatten
opt
check -assert
sat -prove equivalent 1 -{'verify' if variant == 'positive' else 'falsify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env = os.environ.copy()
        env['YOWASP_MOUNT'] = '/work='+str(d)
        env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
        with (d/'proof.log').open('x') as log:
            code = subprocess.run([str(yosys), '-Q', '-T', '-s', '/work/proof.ys'], env=env,
                                  stdout=log, stderr=subprocess.STDOUT).returncode
        expected = 'SAT proof finished - no model found: SUCCESS!' if variant == 'positive' else 'SAT proof finished - model found: FAIL!'
        assert code == 0 and expected in (d/'proof.log').read_text()
        rows.append(dict(variant=variant, returncode=code, source_sha256=sha(d/'actual.sv'),
                         model_sha256=sha(d/'lcell.v'), proof_log_sha256=sha(d/'proof.log')))
    tb = '''module tb_tag_lcell;
 reg [15:0] owners;wire equal;
 tag_address_equivalence dut(owners,equal);
 integer checks=0;
 initial begin
  for(integer pattern=0;pattern<2;pattern++)for(integer bit_index=0;bit_index<16;bit_index++)
   for(integer level=0;level<4;level++)begin
    owners=pattern ? 16'hffff : 16'h0000;
    case(level)0:owners[bit_index]=0;1:owners[bit_index]=1;2:owners[bit_index]=1'bx;3:owners[bit_index]=1'bz;endcase
    #1;if(dut.original_address!==dut.next_address)$fatal(1,"TAG_LCELL four-state address mismatch");checks++;
   end
  $display("TAG_LCELL_FOUR_STATE_PASS checks=%0d outputs=168",checks);$finish;
 end
endmodule
'''
    (proof/'four-state.sv').write_text(tb)
    for label, command in [('compile', ['iverilog', '-g2012', '-s', 'tb_tag_lcell', '-o', str(proof/'test.vvp'),
                                      str(proof/'positive/lcell.v'), str(proof/'positive/actual.sv'), str(proof/'four-state.sv')]),
                           ('test', ['vvp', str(proof/'test.vvp')])]:
        with (proof/(label+'.log')).open('x') as log:
            assert subprocess.run(command, stdout=log, stderr=subprocess.STDOUT).returncode == 0
    assert 'TAG_LCELL_FOUR_STATE_PASS checks=128 outputs=168' in (proof/'test.log').read_text()
    for name in p['source_hashes']:
        raw = changed.encode() if name == MEMC else (parent/name).read_bytes()
        if Path(name).suffix in ['.qsf', '.qpf', '.qip', '.tcl', '.sdc']:
            raw = raw.replace(p['project'].encode(), str(project).encode())
        dest = out/name
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(raw)
    model = out/SIM_MODEL
    model.write_text(v['model'])
    changes = [name for name in p['source_hashes'] if (out/name).read_bytes().replace(
        str(project).encode(), p['project'].encode()) != (parent/name).read_bytes()]
    assert changes == [MEMC]
    names = [*p['source_hashes'], SIM_MODEL]
    record = {**p, 'project': str(project), 'scope': __doc__, 'parent_stage': str(parent),
        'parent_manifest_sha256': sha(parent/'r-front-control-native.json'),
        'extra_RTL_sources': [*p['extra_RTL_sources'], SIM_MODEL],
        'cache_tag_lcell_buffers': dict(instances=len(buffers), affected_RAMs=21, compared_address_bits=168,
            logical_address_pins=168, cluster_size=cluster_size, maximum_RAMs_per_buffer=maximum_RAMs_per_buffer,
            bindings=bindings, original_output_audit=audited, original_wire_list_sha256=sha(wl),
            critical_positive_pin_buffered=critical_binding, inverse_pins=84, positive_pins=84,
            proofs=rows, four_state_checks=128, vendor_model=v,
            simulation_only_model=SIM_MODEL, model_excluded_from_Quartus=True,
            extra_saved_state_or_guest_cycles=0, SRAM_ports_or_capture_changes=False,
            measured_parent_setup_ns=-0.475, physical_replication_qualified=False),
        'source_hashes': {name: sha(out/name) for name in names},
        'physical_timing_qualified': False, 'firmware_qualified': False, 'speedup_qualified': False,
        'assembly_permitted': False, 'hardware_changed': False, 'prepared': time.time()}
    (out/'r-front-control-native.json').write_text(json.dumps(record, indent=2)+'\n')
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    print('STAGED_ALL168_VENDOR_LCELL_TAG_BUFFERS', len(names), 'frozen files', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'failed-fit', 'vendor', 'output', 'project', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--cluster-size', type=int, choices=[1, 2, 4], default=1)
    a = p.parse_args()
    stage(a.parent.resolve(), a.failed_fit.resolve(), a.vendor.resolve(), a.output.resolve(), a.project, a.yosys.resolve(), a.cluster_size)
