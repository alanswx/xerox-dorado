#!/usr/bin/env python3
"""Fresh qualified/shared F10145 XML: same74physical+13transport bits."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_f10145 as original
import generate_f10145_shared_write as candidate

sys.path.insert(0, str(candidate.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report, source):
    expected = dict(mem=(4, 64, [[0, 15]]), we_d=(1, 1, []), a_lat=(4, 4, []),
                    d_lat=(4, 4, []), ce_lat=(1, 1, []), state_result=(10, 10, []),
                    state_frozen=(1, 1, []), state_done=(1, 1, []), state_error=(1, 1, []))
    decisions = []
    for row in report['candidates']:
        if row['module'] != 'cell_F10145A_snapshot' or Path(row['source']).resolve() != source.resolve():
            raise ValueError('field declaration source differs')
        if row['category'] != 'sequential':
            continue
        name = row['name']
        if name not in expected or (row['width'], row['bits'], row['unpacked_dimensions']) != expected.pop(name):
            raise ValueError('extra/duplicate/wrong sequential field: '+name)
        decisions.append(dict(name=name, bits=row['bits'],
                              scope='PRIVATE TRANSPORT' if name.startswith('state_') else 'PHYSICAL FILE'))
    if expected:
        raise ValueError('missing sequential fields: '+','.join(expected))
    assert sum(r['bits'] for r in decisions) == 87
    return decisions


def run(out):
    out.mkdir(parents=True, exist_ok=False)
    results, schemas = {}, []
    for name, gen in [('qualified', original), ('shared', candidate)]:
        d = out/name
        d.mkdir()
        text, manifest = gen.generate()
        schemas.append(manifest)
        source = d/'cell.sv'
        source.write_text(text)
        command = ['verilator', '--xml-only', '--top-module', 'cell_F10145A_snapshot',
                   '-DDORADO_F10145_LOGIC', '-Wno-fatal', '--xml-output', str(d/'cell.xml'), str(source)]
        (d/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (d/'elaborate.log').open('w') as f:
            subprocess.run(command, stdout=f, stderr=subprocess.STDOUT, check=True)
        report = inventory((d/'cell.xml').read_bytes())
        decisions = classify(report, source)
        (d/'inventory.json').write_text(json.dumps(report, indent=2)+'\n')
        results[name] = dict(xml_sha256=report['xml_sha256'], generated_sha256=manifest['generated_sha256'], decisions=decisions)
    assert results['qualified']['decisions'] == results['shared']['decisions']
    assert all(schemas[1][k] == v for k, v in schemas[0].items() if k != 'generated_sha256')
    controls = []
    for name in ('lost_ram', 'lost_we', 'lost_result', 'ram_width', 'ram_depth', 'extra_state', 'duplicate'):
        bad = copy.deepcopy(report)
        if name.startswith('lost_'):
            field = dict(lost_ram='mem', lost_we='we_d', lost_result='state_result')[name]
            bad['candidates'] = [r for r in bad['candidates'] if r['name'] != field]
        elif name == 'ram_width':
            next(r for r in bad['candidates'] if r['name'] == 'mem')['width'] = 3
        elif name == 'ram_depth':
            next(r for r in bad['candidates'] if r['name'] == 'mem')['unpacked_dimensions'] = [[0,14]]
        else:
            row = copy.deepcopy(next(r for r in bad['candidates'] if r['name'] == 'we_d'))
            if name == 'extra_state':
                row['name'] = 'hidden'
            bad['candidates'].append(row)
        try:
            classify(bad, source)
        except ValueError as e:
            controls.append(dict(case=name, rejected=True, diagnostic=str(e)))
        else:
            raise AssertionError('damaged F10145 inventory accepted: '+name)
    (out/'classification.json').write_text(json.dumps(dict(status='PASS', physical_bits=74,
        transport_bits=13, sequential_exclusions=[], results=results, controls=controls,
        scope=__doc__, tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()), indent=2)+'\n')
    print('SHARED F10145 CLASSIFICATION PASS: same74physical+13transport bits;7bad inventories reject')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
