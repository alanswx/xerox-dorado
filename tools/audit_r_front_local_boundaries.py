#!/usr/bin/env python3
"""Audit spare Control-A package halves in the combined processor graph.

NextMacro and jcnt are observed original board crossings, so their two inputs
are live boundaries here. The smaller Control-A-only unused-input result does
not apply to them. Eleven other package-half inputs remain unused in this graph.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch

from audit_mir_ready_boundaries import audit_selected
from generate_conta_local_reference import plan
from inventory_mir_clock_boundaries import BOARD, ROOT


def audit(root, manifest):
    board = manifest['boards'][0]
    spec = dict(comb=sorted(set(board['packages'])-set(board['sequential'])),
                seq=board['sequential'], flag_taps=board['flag_taps'],
                clock_taps=board['clock_taps'])
    _, smaller_extras = plan(root)
    bindings = board['boundary_bindings']
    live = {"CBrOnCnt=0'": 'jcnt', "IFUNext'a": 'NextMacro'}
    extras = {n: bindings[n] for n in smaller_extras if n not in live}
    record, cells = audit_selected(root, spec, extras, 11)
    controls = []
    for net in live:
        try:
            audit_selected(root, spec, {net: bindings[net]}, 1)
        except AssertionError as error:
            assert 'OBSERVED OUTPUT' in str(error), error
            controls.append(dict(case='incorrectly_unused_'+net, diagnostic=str(error)))
        else:
            raise AssertionError('Live processor crossing classified unused: '+net)
    crossings = []
    for net, signal in live.items():
        consumers = []
        for cpu in manifest['boards'][1:]:
            bound = cpu['boundary_bindings']
            if signal in bound:
                consumers.append(dict(board=cpu['board'], binding=bound[signal]))
        assert consumers, signal
        crossings.append(dict(input=net, binding=bindings[net], output=signal,
                              processor_consumers=consumers))
    return record, cells, controls, crossings


def run(reference, out):
    assert not out.exists()
    manifest = json.loads((reference/'manifest.json').read_text())
    record, cells, controls, crossings = audit(ROOT, manifest)
    for label, kind, before, after in [
        ('SWm_into_preemption', 'MC1662', 'assign p14 = ~(p10 | p11);', 'assign p14 = ~(p10 | p11 | p5);'),
        ('Call13_into_branch', 'MC10102', 'assign p2 = ~(p4 | p5);', 'assign p2 = ~(p4 | p5 | p6);'),
        ('WantIfuHold_into_tasking', 'MC10109', 'assign p2  =  a;', 'assign p2  =  a | p12;')]:
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
                assert any(t in str(error) for t in ['LIVE SELECTED CONSUMER', 'OBSERVED OUTPUT']), error
                controls.append(dict(case=label, diagnostic=str(error)))
            else:
                raise AssertionError('Live dependency accepted: '+label)
    out.mkdir(parents=True)
    sources = {BOARD, BOARD.replace('.wl', '.lc'), *cells}
    for cpu in manifest['boards'][1:]:
        wirelist = 'chm/sil/'+cpu['board']+'.dm!1_/'+cpu['board']+'.wl'
        assert hashlib.sha256((ROOT/wirelist).read_bytes()).hexdigest() == cpu['source_sha256']
        sources.update([wirelist, wirelist.replace('.wl', '.lc')])
    for module in list(sys.modules.values()):
        name = getattr(module, '__file__', None)
        if name and Path(name).resolve().parent == ROOT/'tools':
            sources.add(str(Path(name).resolve().relative_to(ROOT)))
    hashes = {}
    for name in sorted(sources):
        p = out/'inputs'/name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes((ROOT/name).read_bytes())
        hashes[name] = hashlib.sha256(p.read_bytes()).hexdigest()
    (out/'reference-manifest.json').write_bytes((reference/'manifest.json').read_bytes())
    record.update(status='PASS_UNUSED_R_FRONT_LOCAL_GATE_BOUNDARIES', scope=__doc__,
                  inputs=hashes, rejected_controls=controls, live_crossings=crossings,
                  native_integrated=False)
    (out/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    print(record['status'], len(record['boundaries']), len(controls), len(crossings))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--reference', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.reference.resolve(), a.output.resolve())
