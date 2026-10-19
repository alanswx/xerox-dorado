#!/usr/bin/env python3
"""Fresh complete DskEth serial-access inventory with no state exclusions.

CRC, both signed timers, all histories, passive receive monitor and oscillator
remain physical state. Administrative response/scan state is classified apart.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_dsketh_full as reference
import generate_dsketh_ff_scan as candidate

sys.path.insert(0,str(candidate.ROOT/'tools'))
from sil_state_inventory import inventory


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def classify(report,manifest,root):
    old = reference.generate(root)[1]
    assert manifest == candidate.generate(root)[1], 'unreviewed DskEth scan schema/source'
    prefix = old['module']+'.'
    wanted = {}

    def add(path,width,dimensions=(),physical=True):
        assert path not in wanted
        wanted[path] = dict(width=width,bits=width*(dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1),
                            dimensions=list(dimensions),physical=physical)

    layout,offset,expected_ff = [],0,[]
    for row in old['ff']:
        layout.append(dict(package=row['package'],kind=row['kind'],lsb=offset,width=row['width'],fields=row['fields']))
        r = copy.deepcopy(row)
        r['legacy_parallel_word'] = r.pop('word')
        expected_ff.append(r)
        for field in row['fields']:
            add(prefix+row['package']+'.'+field['name'],field['width'])
        offset += row['width']
    assert offset == 1070 and manifest['ff']==expected_ff, 'physical FF fields/legacy aliases differ'
    assert manifest['state_access_schema']=='compact-ff-v1'
    assert manifest['scan'] == dict(state_bits=1070,words=60,word_bits=18,layout=layout,
        word_map=[dict(index=i,lsb=18*i,width=min(18,1070-18*i)) for i in range(60)],
        last_word_bits=8,completion_edges=1070), 'compact FF layout differs'
    for key in ('memories','phase','parameters','immutable_proms'):
        assert manifest[key]==old[key], 'non-FF schema differs: '+key
    for row in old['memories']:
        kind = row['kind']
        package = row['package']
        if kind=='cell_F10145A':
            add(prefix+package+'.mem',4,[[0,15]])
            for field in row['write_window']['fields']:
                add(prefix+package+'.'+field['name'],field['width'])
        else:
            for field in row['fields']:
                add(prefix+package+'.'+field['name'],field['width'])
        private = dict(state_frozen=1,state_done=1,state_error=1,
                       state_result=10 if kind=='cell_F10145A' else 18)
        for name,width in private.items():
            add(prefix+package+'.'+name,width,physical=False)
    for field in old['phase']['fields']:
        add(prefix+field['name'],field['width'])
    for name,width in dict(state_frozen=1,state_response_ff=1,state_response_memory=1,state_response_memory_index=5,
                           state_aux_result=18,regular_done=1,regular_error=1).items():
        add(prefix+name,width,physical=False)
    for name,width in dict(frozen=1,writing=1,payload=18,position=11,first_bit=11,busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    roms = {prefix+r['package']+'.mem':r for r in old['immutable_proms']}
    decisions = []
    for row in report['candidates']:
        if row['path'] in roms:
            rom = roms.pop(row['path'])
            assert (row['category'],row['width'],row['bits'],row['unpacked_dimensions']) == (
                'initial_only_candidate',rom['width'],rom['words']*rom['width'],[[0,rom['words']-1]]), 'immutable DskEth PROM differs'
        if row['category']!='sequential':
            continue
        field = wanted.pop(row['path'],None)
        assert field, 'extra/duplicate sequential state: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions']) == (
            field['width'],field['bits'],field['dimensions']), 'state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=row['bits'],scope='PHYSICAL' if field['physical'] else 'PRIVATE TRANSPORT'))
    assert not wanted and not roms, 'state or immutable PROM absent from XML'
    physical = sum(r['bits'] for r in decisions if r['scope']=='PHYSICAL')
    private = sum(r['bits'] for r in decisions if r['scope']=='PRIVATE TRANSPORT')
    assert physical==2656 and private==452
    return dict(status='PASS',scope=__doc__,physical_bits=physical,private_transport_bits=private,
                sequential_exclusions=[],immutable_prom_bits=12032,decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report,manifest,root):
    result = []
    for case in ('missing_ff_history','wrong_ram_depth','extra_state','duplicate','missing_phase','wrong_offset',
                 'missing_layout_bit','legacy_alias','wrong_counter','lost_crc','lost_timer','lost_monitor',
                 'wrong_osc_width','mutable_prom'):
        data,schema = copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_ff_history':
            path = manifest['module']+'.'+manifest['ff'][0]['package']+'.'+manifest['ff'][0]['fields'][-1]['name']
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif case=='wrong_ram_depth':
            next(r for r in data['candidates'] if r['name']=='mem' and r['unpacked_dimensions']==[[0,15]])['unpacked_dimensions'] = [[0,7]]
        elif case in ('extra_state','duplicate'):
            row = copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if case=='extra_state':
                row['path'] += '_unexported'
            data['candidates'].append(row)
        elif case=='missing_phase':
            path = manifest['module']+'.'+manifest['phase']['fields'][0]['name']
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif case=='wrong_offset':
            schema['scan']['layout'][1]['lsb'] += 1
        elif case=='missing_layout_bit':
            schema['scan']['word_map'][-1]['width'] -= 1
        elif case=='legacy_alias':
            schema['ff'][0]['word'] = schema['ff'][0]['legacy_parallel_word']
        elif case=='wrong_counter':
            next(r for r in data['candidates'] if r['path'].endswith('u_ff_scan.position'))['width'] -= 1
        elif case in ('lost_crc','lost_timer','lost_monitor'):
            suffix = dict(lost_crc='.u_j05.cwe_pre',lost_timer='.u_a03.t1',lost_monitor='.u_receive_monitor.write_d')[case]
            data['candidates'] = [r for r in data['candidates'] if not r['path'].endswith(suffix)]
        elif case=='wrong_osc_width':
            next(r for r in data['candidates'] if r['name']=='acc')['width'] = 31
        else:
            next(r for r in data['candidates'] if r['path'].endswith('.'+manifest['immutable_proms'][0]['package']+'.mem'))['category'] = 'sequential'
        try:
            classify(data,schema,root)
        except AssertionError as e:
            result.append(dict(case=case,rejected=True,diagnostic=str(e)))
        else:
            raise AssertionError('damaged DskEth scan inventory accepted: '+case)
    return result


def run(root,out):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones = candidate.generate(root)
    (out/'board.sv').write_text(board)
    for name,text in clones.items():
        (out/name).write_text(text)
    originals = [root/n for n in manifest['original_sources']]
    command = ['verilator','--xml-only','--top-module',manifest['module'],'-Wno-fatal','-DDORADO_F10145_LOGIC',
               '-GSYSPER=4','--xml-output',str(out/'board.xml'),str(out/'board.sv'),*[str(out/n) for n in clones],*map(str,originals)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w') as log:
        subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
    report = inventory((out/'board.xml').read_bytes())
    result = classify(report,manifest,root)
    result['failure_controls'] = controls(report,manifest,root)
    paths = [Path(__file__),Path(candidate.__file__),candidate.ROOT/'tools/sil_state_inventory.py',root/candidate.SOURCE,*originals]
    result['input_hashes'] = {str(p):sha(p) for p in paths}
    for name,data in [('manifest',manifest),('inventory',report),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')
    print('DSKETH SCAN CLASSIFICATION PASS:2656physical+452private bits;14bad controls reject')


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    run(a.source_root.resolve(),a.output.resolve())
