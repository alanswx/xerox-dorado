#!/usr/bin/env python3
"""Fresh complete processor scan inventory against the qualified physical schema.

Every candidate sequential field must be physical state or an explicitly sized
private transport field. Check compact offsets and remove legacy FF addresses.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_processor_full as reference
import generate_processor_ff_scan as candidate

sys.path.insert(0,str(reference.ROOT/'tools'))
from sil_state_inventory import inventory


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def classify(report,manifest,source,kind):
    old = reference.generate(kind,source)[1]
    prefix = old['module']+'.'
    wanted = {}

    def add(path,width,dimensions=(),physical=True):
        assert path not in wanted
        wanted[path] = dict(width=width,dimensions=list(dimensions),
                            bits=width*(dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1),physical=physical)

    layout,offset,expected_ff = [],0,[]
    for row in old['ff']:
        layout.append(dict(package=row['package'],kind=row['kind'],lsb=offset,width=row['width'],fields=row['fields']))
        expected = copy.deepcopy(row)
        expected['legacy_parallel_word'] = expected.pop('word')
        expected_ff.append(expected)
        for field in row['fields']:
            add(prefix+row['package']+'.'+field['name'],field['width'])
        offset += row['width']
    assert manifest['ff'] == expected_ff, 'legacy FF word aliases or physical fields differ'
    assert manifest['state_access_schema'] == 'compact-ff-v1'
    assert manifest['scan'] == dict(state_bits=offset,words=(offset+17)//18,word_bits=18,layout=layout,
        word_map=[dict(index=i,lsb=18*i,width=min(18,offset-18*i)) for i in range((offset+17)//18)],
        last_word_bits=offset-18*((offset+17)//18-1),completion_edges=offset), 'compact FF schema differs'
    for key in ('files','rams','phase','immutable_proms'):
        assert manifest[key] == old[key], 'non-FF schema changed: '+key
    for row in old['files']:
        add(prefix+row['package']+'.mem',4,[[0,15]])
        for field in row['write_window']['fields']:
            add(prefix+row['package']+'.'+field['name'],field['width'])
        for name,width in dict(state_frozen=1,state_done=1,state_error=1,state_result=10).items():
            add(prefix+row['package']+'.'+name,width,physical=False)
    for row in old['rams']:
        add(prefix+row['package']+'.mem',row['width'],[[0,255]])
        for name,width in dict(state_frozen=1,state_done=1,state_error=1,state_result=4).items():
            add(prefix+row['package']+'.'+name,width,physical=False)
    for name in old['phase']['fields']:
        add(prefix+name,1)
    for name,width in dict(state_frozen=1,state_response_ff=1,state_response_memory=1,
                           state_response_memory_index=4,state_aux_result=18,regular_done=1,regular_error=1).items():
        add(prefix+name,width,physical=False)
    count_bits = offset.bit_length()
    for name,width in dict(frozen=1,writing=1,payload=18,position=count_bits,first_bit=count_bits,
                           busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    decisions = []
    for row in report['candidates']:
        if row['category'] != 'sequential':
            continue
        field = wanted.pop(row['path'],None)
        assert field, 'extra/duplicate sequential field: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions']) == (field['width'],field['bits'],field['dimensions']), 'state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=row['bits'],scope='PHYSICAL' if field['physical'] else 'PRIVATE TRANSPORT'))
    assert not wanted, 'missing sequential fields: '+','.join(wanted)
    physical = sum(r['bits'] for r in decisions if r['scope']=='PHYSICAL')
    private = sum(r['bits'] for r in decisions if r['scope']=='PRIVATE TRANSPORT')
    assert physical == old['logical_state_bits']
    assert private == {'ProcH':233,'ProcL':209}[kind]
    return dict(status='PASS',scope=__doc__,kind=kind,physical_bits=physical,private_transport_bits=private,
                sequential_exclusions=[],decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report,manifest,source,kind):
    result = []
    for name in ('missing_history','wrong_ram_depth','extra_state','duplicate','missing_phase',
                 'wrong_offset','missing_layout_bit','legacy_alias','wrong_counter'):
        data,schema = copy.deepcopy(report),copy.deepcopy(manifest)
        if name == 'missing_history':
            path = manifest['module']+'.'+manifest['ff'][0]['package']+'.'+manifest['ff'][0]['fields'][-1]['name']
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif name == 'wrong_ram_depth':
            next(r for r in data['candidates'] if r['name']=='mem' and r['unpacked_dimensions']==[[0,255]])['unpacked_dimensions'] = [[0,254]]
        elif name in ('extra_state','duplicate'):
            row = copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if name == 'extra_state':
                row['path'] += '_unexported'
            data['candidates'].append(row)
        elif name == 'missing_phase':
            path = manifest['module']+'.'+manifest['phase']['fields'][0]
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif name == 'wrong_offset':
            schema['scan']['layout'][1]['lsb'] += 1
        elif name == 'missing_layout_bit':
            schema['scan']['word_map'][-1]['width'] -= 1
        elif name == 'legacy_alias':
            schema['ff'][0]['word'] = schema['ff'][0]['legacy_parallel_word']
        else:
            next(r for r in data['candidates'] if r['path'].endswith('u_ff_scan.position'))['width'] -= 1
        try:
            classify(data,schema,source,kind)
        except AssertionError as e:
            result.append(dict(case=name,rejected=True,diagnostic=str(e)))
        else:
            raise AssertionError('damaged processor scan coverage accepted: '+name)
    return result


def run(kind,root,out):
    out.mkdir(parents=True,exist_ok=False)
    source = root/'verilog/generated'/{'ProcH':'ProcH-Rev-Ce.v','ProcL':'ProcL-Rev-Ci.v'}[kind]
    board,manifest,clones = candidate.generate(kind,source)
    (out/'board.sv').write_text(board)
    for name,text in clones.items():
        (out/name).write_text(text)
    originals = [root/'verilog/cells'/(k+'.v') for k in manifest['original_sources']]
    command = ['verilator','--xml-only','--top-module',manifest['module'],'-GSYSPER=4','-DDORADO_F10145_LOGIC',
               '-Wno-fatal','--xml-output',str(out/'board.xml'),str(out/'board.sv'),*[str(out/n) for n in clones],*map(str,originals)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w') as log:
        subprocess.run(command,cwd=candidate.ROOT,stdout=log,stderr=subprocess.STDOUT,check=True)
    report = inventory((out/'board.xml').read_bytes())
    result = classify(report,manifest,source,kind)
    result['failure_controls'] = controls(report,manifest,source,kind)
    paths = [Path(__file__),Path(candidate.__file__),reference.ROOT/'tools/sil_state_inventory.py',source,*originals]
    result['input_hashes'] = {str(p):sha(p) for p in paths}
    for name,data in [('manifest',manifest),('inventory',report),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')
    print(f"{kind} SCAN CLASSIFICATION PASS:{result['physical_bits']}physical+{result['private_transport_bits']}private bits;9bad controls reject")


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--kind',choices=reference.SPECS,required=True)
    p.add_argument('--frozen-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    run(a.kind,a.frozen_root.resolve(),a.output.resolve())
