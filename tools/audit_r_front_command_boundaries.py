#!/usr/bin/env python3
"""Trace unused command-graph boundaries through every selected gate function.

The newly selected k11 connects multiple gates within one package. Unused Call
inputs can therefore have several selected combinational consumers before an
unconsumed output. Reject any route to a selected register, clock or observed
flag. This is selected-graph evidence; full-machine Call producers remain live.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys
from unittest.mock import patch

from audit_task_control_stubs import pin_dependencies
from generate_conta_local_reference import plan
from inventory_mir_clock_boundaries import ROOT, BOARD
from sil_netlist import load_board
from generate_run_control_reference import model_ports


def cell_dependencies(root, kind):
    # Decoder outputs use vector wire declarations. Retain exact expansion
    # rather than dropping the dependency when the older scalar reader stops.
    raw = (root/'verilog/cells'/('cell_'+kind+'.v')).read_text()
    raw = re.sub(r'//[^\n]*|/\*.*?\*/', '', raw, flags=re.S)
    definitions = {}
    for name, expr in re.findall(r'\b(?:assign|wire)\s*(?:\[[^\]]+\]\s*)?([A-Za-z_]\w*)\s*=\s*([^;]+);', raw):
        assert name not in definitions, (kind, name)
        definitions[name] = expr
    ports = model_ports(root, kind)
    def expand(name, seen):
        if re.fullmatch('p[0-9]+', name) and ports.get(int(name[1:])) == 'input':
            return {int(name[1:])}
        assert name not in seen and name in definitions, (kind, 'unresolved/cyclic expression', name)
        expr = re.sub(r"\b[0-9]+'[sS]?[bBoOdDhH][0-9a-fA-FxXzZ_]+", '', definitions[name])
        result = set()
        for n in re.findall(r'\b[A-Za-z_]\w*\b', expr):
            result |= expand(n, seen|{name})
        return result
    result = {pin: expand('p'+str(pin), set()) for pin, direction in ports.items() if direction == 'output'}
    try:
        old = pin_dependencies(root, kind)
    except AssertionError as error:
        assert 'unresolved/cyclic expression' in str(error), error
    else:
        assert result == old, (kind, 'scalar/vector reader disagreement')
    return result


def audit(root, manifest, exported_call=False):
    board = load_board(str(root/BOARD))
    selected = manifest['boards'][0]
    seq = set(selected['sequential'])
    comb = set(selected['packages'])-seq
    observed = set(selected['flag_taps'])|set(selected['clock_taps'])
    if exported_call:
        observed.add('Call')
    _, old = plan(root)
    bindings = selected['boundary_bindings']
    extras = {n: bindings[n] for n in old if n in bindings}
    extras.update({n: bindings[n] for n in ['Call12', 'Call15']})
    assert len(extras) == 12
    pin_net = {(p['pkg'], p['pin']): n for n, r in board.nets.items() for p in r['pins']}
    cells = set()
    dependencies = {}
    records = []
    for boundary, binding in extras.items():
        paths = []
        def walk(net, route):
            assert net not in observed, ('OBSERVED OUTPUT', boundary, route, net)
            assert net not in route, ('CYCLIC DEPENDENCY', boundary, route, net)
            sinks = [p for p in board.nets[net]['pins'] if p['dir'] == 'in' and p['pkg'] in seq|comb]
            if not sinks:
                paths.append(dict(route=route+[net], terminal='NO_SELECTED_CONSUMER'))
            for p in sinks:
                assert p['pkg'] not in seq, ('SELECTED REGISTER', boundary, route, net, p)
                kind = board.packages[p['pkg']]['type']
                cells.add('verilog/cells/cell_'+kind+'.v')
                if kind not in dependencies:
                    dependencies[kind] = cell_dependencies(root, kind)
                outputs = [pin for pin, deps in dependencies[kind].items() if p['pin'] in deps]
                assert outputs, (boundary, p)
                for pin in outputs:
                    target = pin_net.get((p['pkg'], pin))
                    if target is None:
                        paths.append(dict(route=route+[net], package=p['pkg'], output_pin=pin, terminal='UNWIRED_OUTPUT'))
                    else:
                        walk(target, route+[net])
        walk(boundary, [])
        records.append(dict(boundary=boundary, binding=binding, paths=paths))
    return records, sorted(cells)


def run(reference, out):
    assert not out.exists()
    manifest = json.loads((reference/'manifest.json').read_text())
    records, cells = audit(ROOT, manifest)
    controls = []
    for label, kind, before, after in [
        ('SWm_into_preemption', 'MC1662', 'assign p14 = ~(p10 | p11);', 'assign p14 = ~(p10 | p11 | p5);'),
        ('Call13_into_branch', 'MC10102', 'assign p2 = ~(p4 | p5);', 'assign p2 = ~(p4 | p5 | p6);'),
        ('WantIfuHold_into_tasking', 'MC10109', 'assign p2  =  a;', 'assign p2  =  a | p12;'),
        ('Call12_into_read_command', 'MC10104', 'assign p9 = ~(p12 & p13);', 'assign p9 = ~(p12 & p13) | p6;')]:
        file = ROOT/'verilog/cells'/('cell_'+kind+'.v')
        raw = file.read_text()
        assert raw.count(before) == 1
        changed = raw.replace(before, after)
        original_read = Path.read_text
        def read(path, *args, **kwargs):
            return changed if path == file else original_read(path, *args, **kwargs)
        with patch.object(Path, 'read_text', read):
            try:
                audit(ROOT, manifest)
            except AssertionError as error:
                assert any(t in str(error) for t in ['OBSERVED OUTPUT', 'SELECTED REGISTER']), error
                controls.append(dict(case=label, diagnostic=str(error)))
            else:
                raise AssertionError('Live dependency accepted: '+label)
    try:
        audit(ROOT, manifest, exported_call=True)
    except AssertionError as error:
        assert 'OBSERVED OUTPUT' in str(error) and 'Call' in str(error), error
        controls.append(dict(case='Call_exported_after_multiple_gates', diagnostic=str(error)))
    else:
        raise AssertionError('Exported transitive output accepted')
    out.mkdir(parents=True)
    sources = {BOARD, BOARD.replace('.wl', '.lc'), *cells}
    for m in list(sys.modules.values()):
        n = getattr(m, '__file__', None)
        if n and Path(n).resolve().parent == ROOT/'tools':
            sources.add(str(Path(n).resolve().relative_to(ROOT)))
    hashes = {}
    for n in sorted(sources):
        p = out/'inputs'/n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes((ROOT/n).read_bytes())
        hashes[n] = hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'reference-manifest.json').write_bytes((reference/'manifest.json').read_bytes())
    result = dict(status='PASS_UNUSED_R_FRONT_COMMAND_GATE_BOUNDARIES', scope=__doc__,
                  boundaries=records, rejected_controls=controls, inputs=hashes,
                  full_machine_Call_signals_inactive=False, native_integrated=False)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    print(result['status'], len(records), len(controls))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--reference', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.reference.resolve(), a.output.resolve())
