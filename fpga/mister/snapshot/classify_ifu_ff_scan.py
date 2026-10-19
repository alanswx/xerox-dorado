#!/usr/bin/env python3
"""Fresh complete IFU scan inventory, including canonical held RAM pipelines.

At the frozen boundary saved_q is the original guest pipeline. Other RAM read
and response registers belong to private transport; no sequential exclusions.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import generate_ifu_full as reference
import generate_ifu_ff_scan as candidate

sys.path.insert(0,str(candidate.ROOT/'tools'))
from sil_state_inventory import inventory


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def classify(report,manifest,root):
    # Pin-cone expressions use Python tuples in memory and JSON arrays on
    # disk. Compare the serialized schema without weakening any field check.
    manifest = json.loads(json.dumps(manifest))
    old = json.loads(json.dumps(reference.generate(root)[1]))
    assert manifest==json.loads(json.dumps(candidate.generate(root)[1])), 'unreviewed IFU scan schema/source'
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
    assert offset == 974 and manifest['ff']==expected_ff, 'physical FF fields/legacy aliases differ'
    assert manifest['state_access_schema']=='compact-ff-v1'
    assert manifest['scan'] == dict(state_bits=974,words=55,word_bits=18,layout=layout,
        word_map=[dict(index=i,lsb=18*i,width=min(18,974-18*i)) for i in range(55)],
        last_word_bits=2,completion_edges=974), 'compact FF layout differs'
    for key in ('banks','phase','grouping','immutable_proms'):
        assert manifest[key]==old[key], 'non-FF schema differs: '+key
    for row in old['banks']:
        package='snapshot_bank_'+str(row['bank'])
        add(prefix+package+'.mem',row['width'],[[0,1023]])
        add(prefix+package+'.saved_q',row['width'])
        private=dict(ram_q=row['width'],pending=1,response_pipeline=1,frozen=1,done=1,error=1)
        for name,width in private.items():
            add(prefix+package+'.'+name,width,physical=False)
    for name in old['phase']['fields']:
        add(prefix+name,1)
    for name,width in dict(state_frozen=1,state_response_ff=1,state_response_memory=1,state_response_memory_index=4,
                           state_aux_result=18,regular_done=1,regular_error=1).items():
        add(prefix+name,width,physical=False)
    for name,width in dict(frozen=1,writing=1,payload=18,position=10,first_bit=10,busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    roms = {prefix+r['package']+'.mem':r for r in old['immutable_proms']}
    decisions = []
    for row in report['candidates']:
        if row['path'] in roms:
            rom = roms.pop(row['path'])
            assert (row['category'],row['width'],row['bits'],row['unpacked_dimensions']) == (
                'initial_only_candidate',8,256,[[0,31]]), 'immutable IFU PROM differs'
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
    assert physical==28653 and private==171, (physical,private)
    return dict(status='PASS',scope=__doc__,physical_bits=physical,private_transport_bits=private,
                sequential_exclusions=[],immutable_prom_bits=256,decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report,manifest,root):
    result = []
    for case in ('missing_ff_history','wrong_ram_depth','extra_state','duplicate','missing_phase','wrong_offset',
                 'missing_layout_bit','legacy_alias','wrong_counter','missing_pipeline','wrong_bank_width','mutable_prom',
                 'merged_bank','ce_tie'):
        data,schema = copy.deepcopy(report),copy.deepcopy(manifest)
        if case=='missing_ff_history':
            path = manifest['module']+'.'+manifest['ff'][0]['package']+'.'+manifest['ff'][0]['fields'][-1]['name']
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif case=='wrong_ram_depth':
            next(r for r in data['candidates'] if r['name']=='mem' and r['unpacked_dimensions']==[[0,1023]])['unpacked_dimensions'] = [[0,511]]
        elif case in ('extra_state','duplicate'):
            row = copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if case=='extra_state':
                row['path'] += '_unexported'
            data['candidates'].append(row)
        elif case=='missing_phase':
            path = manifest['module']+'.'+manifest['phase']['fields'][0]
            data['candidates'] = [r for r in data['candidates'] if r['path']!=path]
        elif case=='wrong_offset':
            schema['scan']['layout'][1]['lsb'] += 1
        elif case=='missing_layout_bit':
            schema['scan']['word_map'][-1]['width'] -= 1
        elif case=='legacy_alias':
            schema['ff'][0]['word'] = schema['ff'][0]['legacy_parallel_word']
        elif case=='wrong_counter':
            next(r for r in data['candidates'] if r['path'].endswith('u_ff_scan.position'))['width'] -= 1
        elif case=='missing_pipeline':
            data['candidates'] = [r for r in data['candidates'] if not r['path'].endswith('.snapshot_bank_0.saved_q')]
        elif case=='wrong_bank_width':
            next(r for r in data['candidates'] if r['path'].endswith('.snapshot_bank_1.mem'))['width'] -= 1
        elif case=='merged_bank':
            schema['banks'][1]['index']=schema['banks'][0]['index']
        elif case=='ce_tie':
            schema['banks'][0]['ce']="1'b0"
        else:
            next(r for r in data['candidates'] if r['path'].endswith('.u_a06.mem'))['category'] = 'sequential'
        try:
            classify(data,schema,root)
        except AssertionError as e:
            result.append(dict(case=case,rejected=True,diagnostic=str(e)))
        else:
            raise AssertionError('damaged IFU scan inventory accepted: '+case)
    return result


def run(root,out):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones = candidate.generate(root)
    (out/'board.sv').write_text(board)
    for name,text in clones.items():
        (out/name).write_text(text)
    originals = [root/n for n in manifest['original_sources']]
    command = ['verilator','--xml-only','--top-module',manifest['module'],'-Wno-fatal',
               '-DDORADO_F10415_M10K','--xml-output',str(out/'board.xml'),str(out/'board.sv'),*[str(out/n) for n in clones],*map(str,originals)]
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
    print('IFU SCAN CLASSIFICATION PASS:28653physical+171private bits;14bad controls reject')


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a = p.parse_args()
    run(a.source_root.resolve(),a.output.resolve())
