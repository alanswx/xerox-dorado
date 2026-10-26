#!/usr/bin/env python3
"""Trace staged MemC Hold386 to actual combinational pins and state boundaries.

Dependency information comes from each primitive's actual output equation,
not all inputs on a package. Sequential/complex modules stop the walk. Vector
sources retain their complete expression as a conservative boundary. This is
a source audit for preparing a timing cut, not a state-stability proof.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

BOARD = 'src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv'
BACKPLANE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'


def clean(raw):
    return re.sub(r'/\*.*?\*/|//[^\n]*', '', raw, flags=re.S)


def identifiers(expr):
    expr = re.sub(r"\d*'[sS]?[bBdDhHoO][0-9a-fA-FxXzZ_?]+", '', expr)
    return set(re.findall(r'\b[A-Za-z_]\w*\b', expr))


def run(stage, out):
    assert not out.exists()
    m = json.loads((stage/'r-front-control-native.json').read_text())
    verify(stage, m['source_hashes'])
    raw = (stage/BOARD).read_text()
    board_instances = re.findall(r'^  (\w+)(?: #\([^\n]*\))? (u_\w+) \(\n(.*?)^  \);', raw, re.M | re.S)
    needed = {kind for kind, _, _ in board_instances}
    bank, owners = {}, {}
    for path in m['source_hashes']:
        if Path(path).suffix not in ['.v', '.sv']:
            continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule', (stage/path).read_text(), re.M | re.S):
            if match[1] not in needed:
                continue
            if match[1] in bank:
                assert bank[match[1]] == match[0], match[1]
            bank[match[1]], owners[match[1]] = match[0], path
    board = clean(raw)
    drivers, instances, primitive_support = {}, {}, {}
    for lhs, rhs in re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', board):
        assert lhs not in drivers, lhs
        drivers[lhs] = dict(kind='assignment', expression=rhs.strip())
    for kind, name, body in board_instances:
        assert name not in instances and kind in bank, (kind, name)
        pins = dict(re.findall(r'\.(p\d+)\(([^()]*)\)', body))
        primitive = clean(bank[kind])
        directions = dict((p, d) for d, p in re.findall(r'\b(input|output)\s+(?:wire\s+|reg\s+)?(p\d+)\b', primitive))
        # Only primitives implemented entirely by scalar assignments are expanded.
        sequential = bool(re.search(r'\b(?:always|always_ff|always_latch|initial|function|task)\b', primitive))
        equations = dict(re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', primitive))
        equations.update(dict(re.findall(r'\bwire\s+(?:\[[^]]+\]\s*)?(\w+)\s*=\s*([^;]+);', primitive)))
        inputs = {p for p, d in directions.items() if d == 'input'}

        def deps(signal, seen):
            if signal in inputs:
                return {signal}
            assert signal not in seen and signal in equations, (kind, signal)
            return set().union(*(deps(s, seen | {signal}) for s in identifiers(equations[signal])))

        support = {}
        for pin, direction in directions.items():
            if direction == 'output' and not sequential and pin in equations:
                try:
                    support[pin] = sorted(deps(pin, set()))
                except AssertionError:
                    pass  # Preserve complex outputs as explicit unresolved boundaries.
        instances[name] = dict(module=kind, pins=pins, expanded_output_inputs=support)
        primitive_support[kind] = support
        for pin, net in pins.items():
            if directions.get(pin) != 'output' or not net:
                continue
            assert re.fullmatch(r'\w+', net), (name, pin, net)
            assert net not in drivers, net
            drivers[net] = dict(kind='cell', instance=name, pin=pin)
    # Close the actual board's exported feedback through native backplane
    # aliases. Other boards remain explicit boundaries for subsequent audit.
    backplane = (stage/BACKPLANE).read_text().split('module dorado_native_backplane_ce #', 1)[1].split('endmodule', 1)[0]
    call = re.search(r' MemC_full_snapshot_experiment b_MemC \((.*?)\n  \);', backplane, re.S)[1]
    bindings = dict(re.findall(r'\.(\w+)\(([^()]*)\)', call))
    outputs = set(re.findall(r'\boutput\s+wire\s+(\w+)\b', clean(raw.split(');', 1)[0])))
    external_to_output = {expr: port for port, expr in bindings.items() if port in outputs}
    native_assignments = dict(re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', clean(backplane)))
    aliases = {}
    for port, native_net in bindings.items():
        if port in outputs or port in drivers:
            continue
        expression = native_assignments.get(native_net, '')
        output = external_to_output.get(expression.strip())
        if output:
            assert output in drivers, (port, output)
            drivers[port] = dict(kind='assignment', expression=output)
            aliases[port] = dict(native_net=native_net, native_driver=expression.strip(), output_port=output)
    visited, leaves, unwired, selected = {}, {}, [], set()

    def walk(net, stack):
        assert net not in stack, ('COMBINATIONAL_CYCLE', stack, net)
        if net in visited:
            return
        row = drivers.get(net)
        if row is None:
            leaves[net] = dict(kind='board_input_or_unresolved_net')
            return
        if row['kind'] == 'assignment':
            expression = row['expression']
            if '[' in expression:
                leaves[net] = dict(kind='vector_expression', expression=expression)
                return
            deps = sorted(identifiers(expression))
        else:
            name, pin = row['instance'], row['pin']
            instance = instances[name]
            selected.add(name)
            if pin not in instance['expanded_output_inputs']:
                leaves[net] = dict(kind='sequential_or_complex_cell_output', instance=name,
                                   module=instance['module'], pin=pin)
                return
            deps = []
            for input_pin in instance['expanded_output_inputs'][pin]:
                value = instance['pins'].get(input_pin)
                if value is None or value == '':
                    unwired.append(dict(instance=name, module=instance['module'], pin=input_pin, output=pin))
                else:
                    deps.extend(identifiers(value))
        visited[net] = dict(**row, dependencies=sorted(set(deps)))
        for dep in deps:
            walk(dep, stack+[net])

    walk('MemC15_sil_pl_2', [])
    assert drivers['MemC15_sil_pl_2']['kind'] == 'assignment'
    for net in ['NewRef', 'dVA_11', 'WantCondHold']:
        assert net in visited, net
    # The Hold input crosses the board boundary. Its actual backplane producer
    # must be included when deriving a native preparation descriptor.
    assert 'Hold' in aliases and 'Hold' in visited
    assert any(row.get('expression') == 'r_front_q[842]' for row in leaves.values())
    out.mkdir(parents=True)
    source_paths = {BOARD, BACKPLANE, *(owners[instances[n]['module']] for n in selected)}
    for path in source_paths:
        target = out/'inputs'/path
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(stage/path, target)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    result = dict(status='AUDITED_ACTUAL_MEMC_HOLD386_COMBINATIONAL_BOUNDARIES', scope=__doc__,
                  stage=str(stage), stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                  source_hashes={p: sha(stage/p) for p in sorted(source_paths)},
                  visited_nets=visited, boundary_nets=leaves, selected_instances={n: instances[n] for n in sorted(selected)},
                  primitive_output_support=primitive_support, unwired_primitive_inputs=unwired,
                  native_feedback_aliases=aliases,
                  residual_board_input_drivers={n: dict(binding=bindings.get(n), driver=native_assignments.get(bindings.get(n)))
                                               for n, row in leaves.items() if row['kind']=='board_input_or_unresolved_net'},
                  original_physical_unwired_pulldowns_not_new_four_state_assumptions=True,
                  whole_native_descriptor_or_earlier_phase_stability_proved=False,
                  next_action='Expand native backplane input drivers, especially Hold and address/IFU inputs; derive a descriptor from stable upstream state and actual registered cache read words. Prove preparation validity at the original capture edge before integrating a register.',
                  native_RTL_or_guard_changed=False, physical_timing_or_speed_gain_qualified=False)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    print(result['status'], len(visited), 'nets', len(leaves), 'boundaries', len(selected), 'instances', flush=True)
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    run(a.stage.resolve(), a.output.resolve())
