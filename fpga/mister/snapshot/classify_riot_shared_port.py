#!/usr/bin/env python3
"""Fresh old/new RIOT field geometry comparison, with no sequential exclusions."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_riot as original
import generate_riot_shared_port as candidate

sys.path.insert(0, str(candidate.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report, source):
    wanted = {name: (width, width, []) for name, width in original.FIELDS}
    wanted.update(riot_ram=(8, 1024, [[0, 127]]), state_result=(18, 18, []),
                  state_frozen=(1, 1, []), state_done=(1, 1, []), state_error=(1, 1, []))
    decisions = []
    for row in report['candidates']:
        if row['module'] != 'M6532_snapshot' or Path(row['source']).resolve() != source.resolve():
            raise ValueError('field declaration source differs')
        if row['category'] != 'sequential':
            continue
        name = row['name']
        if name not in wanted or (row['width'], row['bits'], row['unpacked_dimensions']) != wanted.pop(name):
            raise ValueError('extra/duplicate/wrong sequential field: '+name)
        decisions.append(dict(name=name, bits=row['bits'],
                              scope='PRIVATE TRANSPORT' if name.startswith('state_') else 'PHYSICAL RIOT'))
    if wanted:
        raise ValueError('missing sequential fields: '+','.join(wanted))
    if sum(r['bits'] for r in decisions) != 1118:
        raise ValueError('total mutable bits differ')
    return decisions


def run(out):
    out.mkdir(parents=True, exist_ok=False)
    results = {}
    for variant, gen in [('qualified', original), ('shared', candidate)]:
        case = out/variant
        case.mkdir()
        text, manifest = gen.generate()
        source = case/'M6532_snapshot.sv'
        source.write_text(text)
        command = ['verilator', '--xml-only', '--top-module', 'M6532_snapshot',
                   '-Ginit_7800=0', '-Wno-fatal', '--xml-output', str(case/'riot.xml'), str(source)]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'elaborate.log').open('w') as f:
            subprocess.run(command, stdout=f, stderr=subprocess.STDOUT, check=True)
        report = inventory((case/'riot.xml').read_bytes())
        decisions = classify(report, source)
        (case/'inventory.json').write_text(json.dumps(report, indent=2)+'\n')
        results[variant] = dict(generated_sha256=manifest['generated_sha256'],
                                xml_sha256=report['xml_sha256'], decisions=decisions)
    assert results['qualified']['decisions'] == results['shared']['decisions']
    controls = []
    for name in ('lost_ram', 'lost_pa7', 'lost_dout', 'ram_width', 'ram_depth', 'extra_state', 'duplicate'):
        bad = copy.deepcopy(report)
        if name.startswith('lost_'):
            field = dict(lost_ram='riot_ram', lost_pa7='old_pa7', lost_dout='d_out')[name]
            bad['candidates'] = [r for r in bad['candidates'] if r['name'] != field]
        elif name == 'ram_width':
            next(r for r in bad['candidates'] if r['name'] == 'riot_ram')['width'] = 7
        elif name == 'ram_depth':
            next(r for r in bad['candidates'] if r['name'] == 'riot_ram')['unpacked_dimensions'] = [[0,126]]
        else:
            row = copy.deepcopy(next(r for r in bad['candidates'] if r['name'] == 'old_pa7'))
            if name == 'extra_state':
                row['name'] = 'hidden'
            bad['candidates'].append(row)
        try:
            classify(bad, source)
        except ValueError as e:
            controls.append(dict(case=name, rejected=True, diagnostic=str(e)))
        else:
            raise AssertionError('damaged RIOT fields accepted: '+name)
    (out/'classification.json').write_text(json.dumps(dict(status='PASS', physical_bits=1097,
        transport_bits=21, sequential_exclusions=[], results=results, controls=controls,
        scope=__doc__, tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()), indent=2)+'\n')
    print('SHARED RIOT CLASSIFICATION PASS: same1097physical+21transport bits;7damaged inventories reject')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.output.resolve())
