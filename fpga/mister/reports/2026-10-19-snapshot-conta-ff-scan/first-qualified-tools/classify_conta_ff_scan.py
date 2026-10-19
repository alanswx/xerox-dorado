#!/usr/bin/env python3
"""Account for every candidate ContA sequential bit and its compact FF layout.

Physical coverage comes from the qualified parallel board schema; all extra
state must match the explicit private transport declarations. No exclusions.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
import generate_conta_full as reference
import generate_conta_ff_scan as candidate

sys.path.insert(0,str(reference.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report, manifest, source):
    _, old, _ = reference.generate(source)
    prefix = candidate.MODULE+'.'
    wanted = {}
    layout, offset = [], 0

    def add(path,width,dimensions=(),physical=True):
        assert path not in wanted, 'duplicate declared state'
        wanted[path] = dict(width=width, dimensions=list(dimensions),
                            bits=width*(dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1),
                            physical=physical)

    for row in old['ff']:
        layout.append(dict(package=row['package'],kind=row['kind'],lsb=offset,
                           width=row['width'],fields=row['fields']))
        for field in row['fields']:
            add(prefix+row['package']+'.'+field['name'],field['width'])
        offset += row['width']
    assert offset == 681
    assert manifest['scan']['layout'] == layout, 'compact FF offsets/fields differ'
    assert manifest['scan']['state_bits'] == 681 and manifest['scan']['words'] == 38
    assert manifest['scan']['word_bits'] == 18 and manifest['scan']['last_word_bits'] == 15
    for row in old['files']:
        add(prefix+row['package']+'.mem',4,[[0,15]])
        for field in row['write_window']['fields']:
            add(prefix+row['package']+'.'+field['name'],field['width'])
        for name,width in dict(state_frozen=1,state_done=1,state_error=1,state_result=10).items():
            add(prefix+row['package']+'.'+name,width,physical=False)
    for name in old['phase']['fields']:
        add(prefix+name,1)
    for name,width in dict(state_frozen=1,state_response_ff=1,state_response_ram=1,
                           state_response_ram_index=4,state_aux_result=18,
                           regular_done=1,regular_error=1).items():
        add(prefix+name,width,physical=False)
    for name,width in dict(frozen=1,writing=1,payload=18,position=10,first_bit=10,
                           busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    decisions = []
    for row in report['candidates']:
        if row['category'] != 'sequential':
            continue
        field = wanted.pop(row['path'],None)
        assert field is not None, 'extra/duplicate sequential state: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions']) == (
            field['width'],field['bits'],field['dimensions']), 'state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=row['bits'],
                              scope='PHYSICAL' if field['physical'] else 'PRIVATE TRANSPORT'))
    assert not wanted, 'missing sequential state: '+','.join(wanted)
    assert sum(r['bits'] for r in decisions if r['scope']=='PHYSICAL') == 1353
    assert sum(r['bits'] for r in decisions if r['scope']=='PRIVATE TRANSPORT') == 206
    return dict(status='PASS',scope=__doc__,physical_bits=1353,private_transport_bits=206,
                sequential_exclusions=[],decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report, manifest, source):
    records = []
    for case in ('missing_history','wrong_ram_depth','extra_state','duplicate','missing_phase',
                 'wrong_layout_offset','wrong_layout_width','wrong_counter_width'):
        data, schema = copy.deepcopy(report),copy.deepcopy(manifest)
        if case == 'missing_history':
            data['candidates'] = [r for r in data['candidates'] if r['path'] != candidate.MODULE+'.'+schema['ff'][0]['package']+'.ck_d']
        elif case == 'wrong_ram_depth':
            next(r for r in data['candidates'] if r['name']=='mem')['unpacked_dimensions'] = [[0,14]]
        elif case in ('extra_state','duplicate'):
            row = copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if case == 'extra_state':
                row['path'] += '_unexported'
            data['candidates'].append(row)
        elif case == 'missing_phase':
            path = candidate.MODULE+'.'+reference.PHASES[0]
            data['candidates'] = [r for r in data['candidates'] if r['path'] != path]
        elif case == 'wrong_layout_offset':
            schema['scan']['layout'][1]['lsb'] += 1
        elif case == 'wrong_layout_width':
            schema['scan']['layout'][0]['fields'][0]['width'] -= 1
        else:
            next(r for r in data['candidates'] if r['path'].endswith('u_ff_scan.position'))['width'] = 9
        try:
            classify(data,schema,source)
        except AssertionError as e:
            records.append(dict(case=case,rejected=True,diagnostic=str(e)))
        else:
            raise AssertionError('damaged FF-scan coverage accepted: '+case)
    return records


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--directory',type=Path,required=True)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    root,source = a.directory.resolve(),a.source.resolve()
    manifest = json.loads((root/'manifest.json').read_text())
    board, expected, helpers = candidate.generate(source,async_files=manifest['async_files'])
    assert manifest == expected, 'candidate schema differs'
    assert (root/'ContA_snapshot.sv').read_bytes() == board.encode(), 'candidate source differs'
    for name,text in helpers.items():
        assert (root/name).read_bytes() == text.encode(), 'helper source differs: '+name
    report = inventory((root/'board.xml').read_bytes())
    result = classify(report,manifest,source)
    result['failure_controls'] = controls(report,manifest,source)
    result['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    a.output.write_text(json.dumps(result,indent=2)+'\n')
    print('CONTA SCAN CLASSIFICATION PASS:1353physical+206private bits;no exclusions;8bad controls reject')
