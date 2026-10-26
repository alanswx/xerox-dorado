#!/usr/bin/env python3
"""Resolve actual Hold386 board crossings to state or explicit complex boundaries.

Use actual staged native board bindings and per-output primitive equations.
No arbitrary board input is silently treated as a register. Complex outputs,
vector expressions, external inputs and unconnected pins retain their scope.
This derives the source cut to prove, not a promise that a cut is stable early.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from audit_native_memc_hold_producer import BOARD, BACKPLANE, clean, identifiers
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

BOARDS = dict(MemC=BOARD,
    MemX='src/fpga/mister/snapshot_native_memx/MemX_full_snapshot.sv',
    ContA='src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv',
    ContB='src/fpga/mister/snapshot_native/ContB_full_snapshot.sv',
    ProcL='src/fpga/mister/snapshot_native_processor/ProcL_full_snapshot.sv',
    ProcH='src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv')


def primitive_model(raw):
    raw = clean(raw)
    directions = {p: d for d, p in re.findall(r'\b(input|output)\s+(?:wire\s+|reg\s+)?(p\d+)\b', raw)}
    inputs = {p for p, d in directions.items() if d == 'input'}
    equations = dict(re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', raw))
    equations.update(dict(re.findall(r'\bwire\s+(?:\[[^]]+\]\s*)?(\w+)\s*=\s*([^;]+);', raw)))
    sequential = bool(re.search(r'\b(?:always|always_ff|always_latch|initial|function|task)\b', raw))

    def deps(n, seen):
        if n in inputs:
            return {n}
        assert n not in seen and n in equations, n
        return set().union(*(deps(s, seen | {n}) for s in identifiers(equations[n])))

    support = {}
    if not sequential:
        for p, d in directions.items():
            if d == 'output' and p in equations:
                try:
                    support[p] = sorted(deps(p, set()))
                except AssertionError:
                    pass
    return directions, support


def run(stage, out):
    assert not out.exists()
    manifest = stage/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    verify(stage, m['source_hashes'])
    native = (stage/BACKPLANE).read_text().split('module dorado_native_backplane_ce #', 1)[1].split('endmodule', 1)[0]
    native_assignments = dict(re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', clean(native)))
    boards, kinds = {}, set()
    for name, path in BOARDS.items():
        raw = (stage/path).read_text()
        call = re.search(r' \w+(?: #\([^\n]*\))? b_'+name+r' \((.*?)\n  \);', native, re.S)
        assert call, name
        bindings = dict(re.findall(r'\.(\w+)\(([^()]*)\)', call[1]))
        header = clean(raw.split(');', 1)[0])
        directions = {p: d for d, p in re.findall(r'\b(input|output)\s+(?:wire\s+|reg\s+)?(?:\[[^]]+\]\s*)?(\w+)', header)}
        assignments = {}
        for lhs, rhs in re.findall(r'\bassign\s+(\w+)\s*=\s*([^;]+);', clean(raw)):
            assert lhs not in assignments, (name, lhs)
            assignments[lhs] = dict(kind='assignment', expression=rhs.strip())
        cells = {}
        for kind, instance, body in re.findall(r'^  (\w+)(?: #\([^\n]*\))? (u_\w+) \(\n(.*?)^  \);', raw, re.M | re.S):
            cells[instance] = dict(module=kind, pins=dict(re.findall(r'\.(p\d+)\(([^()]*)\)', body)))
            kinds.add(kind)
        boards[name] = dict(path=path, bindings=bindings, directions=directions, drivers=assignments, cells=cells)
    bank, owners = {}, {}
    for path in m['source_hashes']:
        if Path(path).suffix not in ['.v', '.sv']:
            continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule', (stage/path).read_text(), re.M | re.S):
            if match[1] in kinds:
                if match[1] in bank:
                    assert bank[match[1]] == match[0], match[1]
                bank[match[1]], owners[match[1]] = match[0], path
    models = {k: primitive_model(bank[k]) for k in kinds}
    output_owners = {}
    for name, b in boards.items():
        for port, expr in b['bindings'].items():
            if b['directions'].get(port) == 'output' and expr:
                assert expr not in output_owners, expr
                output_owners[expr] = name+'.'+port
        for instance, cell in b['cells'].items():
            directions, support = models[cell['module']]
            cell['expanded_output_inputs'] = support
            for p, net in cell['pins'].items():
                if directions.get(p) == 'output' and net:
                    assert re.fullmatch(r'\w+', net), net
                    assert net not in b['drivers'], (name, net)
                    b['drivers'][net] = dict(kind='cell', instance=instance, pin=p)
    visited, leaves, unwired, selected, crossings = {}, {}, [], set(), []

    def walk(key, stack):
        assert key not in stack, ('COMBINATIONAL_CYCLE', stack, key)
        if key in visited or key in leaves:
            return
        name, net = key.split('.', 1)
        b = boards[name]
        row = b['drivers'].get(net)
        if row is None:
            expr = b['bindings'].get(net)
            native_expr = native_assignments.get(expr, expr)
            if native_expr in output_owners:
                dep = output_owners[native_expr]
                row = dict(kind='board_crossing', native_binding=expr, native_driver=native_expr)
                deps = [dep]
                crossings.append(dict(source=key, target=dep, **row))
            else:
                leaves[key] = dict(kind='native_input_or_unresolved', binding=expr, driver=native_expr)
                return
        elif row['kind'] == 'assignment':
            expression = row['expression']
            if '[' in expression:
                leaves[key] = dict(kind='vector_expression', expression=expression,
                                   native_vector_bindings={s: b['bindings'].get(s) for s in identifiers(expression)})
                return
            deps = [name+'.'+n for n in sorted(identifiers(expression))]
        else:
            instance, pin = row['instance'], row['pin']
            cell = b['cells'][instance]
            selected.add((name, instance))
            if pin not in cell['expanded_output_inputs']:
                leaves[key] = dict(kind='sequential_or_complex_output', instance=instance, module=cell['module'], pin=pin)
                return
            deps = []
            for p in cell['expanded_output_inputs'][pin]:
                expr = cell['pins'].get(p)
                if expr is None or expr == '':
                    unwired.append(dict(board=name, instance=instance, module=cell['module'], input=p, output=pin))
                else:
                    deps += [name+'.'+n for n in sorted(identifiers(expr))]
        visited[key] = dict(**row, dependencies=sorted(set(deps)))
        for dep in sorted(set(deps)):
            walk(dep, stack+[key])

    walk('MemC.MemC15_sil_pl_2', [])
    required = {'MemC.EcWantsA', 'MemC.STfree_p_', 'MemC.CLKEnable_p_b', 'MemC.ASEL_0', 'MemC._u_MD', 'MemC._u_MDI'}
    assert required <= set(visited), required-set(visited)
    sources = {BACKPLANE, *(BOARDS[n] for n in boards), *(owners[boards[n]['cells'][i]['module']] for n, i in selected)}
    out.mkdir(parents=True)
    for p in sorted(sources):
        q = out/'inputs'/p
        q.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(stage/p, q)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    result = dict(status='RESOLVED_NATIVE_HOLD386_OWNER_BOUNDARIES', scope=__doc__, stage=str(stage),
                  stage_manifest_sha256=sha(manifest), source_hashes={p: sha(stage/p) for p in sorted(sources)},
                  visited_nets=visited, boundary_nets=leaves, crossings=crossings, unwired_inputs=unwired,
                  selected_instances={n+'.'+i: boards[n]['cells'][i] for n, i in sorted(selected)},
                  all_six_previous_external_boundaries_expanded=True,
                  timing_or_stability_or_retimation_qualified=False, native_RTL_changed=False)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    print(result['status'], len(visited), 'nets', len(leaves), 'boundaries', flush=True)
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['stage', 'output']:
        p.add_argument('--'+n, type=Path, required=True)
    a = p.parse_args()
    run(a.stage.resolve(), a.output.resolve())
