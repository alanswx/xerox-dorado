#!/usr/bin/env python3
"""Fresh complete ContB serial-state inventory with no sequential exclusions.

Each saved_q is the canonical original IM read pipeline at a frozen boundary.
Borrowed raw read registers, replies and scan-control state are private.
"""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import generate_contb_ff_scan as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,root):
    old=gen.reference(root)[1]
    assert manifest==gen.generate(root)[1], 'unreviewed ContB scan schema/source'
    prefix=gen.MODULE+'.';wanted={}
    def add(path,width,dimensions=(),physical=True):
        assert path not in wanted
        wanted[path]=dict(width=width,bits=width*(dimensions[0][1]-dimensions[0][0]+1 if dimensions else 1),
                          dimensions=list(dimensions),physical=physical)
    offset,layout,expected_ff=0,[],[]
    for row in old['ff']:
        package='u_'+row['package']
        layout.append(dict(package=package,kind=row['kind'],lsb=offset,width=row['width'],fields=row['fields']))
        expected=copy.deepcopy(row);expected['legacy_parallel_word']=expected.pop('word');expected_ff.append(expected)
        for field in row['fields']:add(prefix+package+'.'+field['name'],field['width'])
        offset+=row['width']
    assert offset==209 and manifest['ff']==expected_ff
    assert manifest['state_access_schema']=='compact-ff-v1'
    assert manifest['scan']==dict(state_bits=209,words=12,base_word=16384,word_bits=18,layout=layout,
        word_map=[dict(index=16384+i,lsb=18*i,width=min(18,209-18*i)) for i in range(12)],
        last_word_bits=11,completion_edges=209), 'compact FF map differs'
    assert manifest['groups']==old['groups'], 'IM grouping/pins differ'
    for group in old['groups']:
        package='snapshot_bank_'+str(group['index'])
        add(prefix+package+'.saved_q',18)
        for bit in range(18):
            add(prefix+package+f'.bits[{bit}].mem',1,[[0,1023]])
            add(prefix+package+f'.bits[{bit}].read_q',1,physical=False)
        for name in ('pending','response_pipeline','frozen','done','error'):
            add(prefix+package+'.'+name,1,physical=False)
    for name,width in dict(state_response_bank=3,state_response_aux=1,state_response_ff=1,
                           state_aux_done=1,state_aux_error=1,state_boundary_ready=1).items():
        add(prefix+name,width,physical=False)
    for name,width in dict(frozen=1,writing=1,payload=18,position=8,first_bit=8,busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    decisions=[]
    for row in report['candidates']:
        if row['category']!='sequential':continue
        field=wanted.pop(row['path'],None)
        assert field, 'extra/duplicate sequential state: '+row['path']
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(
            field['width'],field['bits'],field['dimensions']), 'state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=row['bits'],scope='PHYSICAL' if field['physical'] else 'PRIVATE TRANSPORT'))
    assert not wanted, 'state absent from XML: '+str(list(wanted)[:3])
    physical=sum(r['bits'] for r in decisions if r['scope']=='PHYSICAL')
    private=sum(r['bits'] for r in decisions if r['scope']=='PRIVATE TRANSPORT')
    assert (physical,private)==(147809,250), (physical,private)
    return dict(status='PASS',scope=__doc__,physical_bits=physical,private_transport_bits=private,
                sequential_exclusions=[],decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report,manifest,root):
    results=[]
    for case in ('missing_history','ram_depth','extra_state','duplicate','missing_pipeline','missing_parity',
                 'wrong_counter','missing_boundary','wrong_offset','bad_word_base','legacy_alias',
                 'wrong_group','missing_precapture','lost_raw_read'):
        data,schema=copy.deepcopy(report),copy.deepcopy(manifest)
        suffix=None
        if case=='missing_history':suffix='.u_'+manifest['ff'][0]['package']+'.'+manifest['ff'][0]['fields'][-1]['name']
        elif case=='ram_depth':next(r for r in data['candidates'] if r['path'].endswith('.mem'))['unpacked_dimensions']=[[0,511]]
        elif case in ('extra_state','duplicate'):
            row=copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if case=='extra_state':row['path']+='_extra'
            data['candidates'].append(row)
        elif case=='missing_pipeline':suffix='.snapshot_bank_0.saved_q'
        elif case=='missing_parity':
            group=manifest['groups'][0];bit=next(b['bit'] for b in group['bits'] if b['field']=='dIMLH')
            suffix=f'.snapshot_bank_0.bits[{bit}].mem'
        elif case=='wrong_counter':next(r for r in data['candidates'] if r['path'].endswith('.u_ff_scan.position'))['width']=7
        elif case=='missing_boundary':suffix='.state_boundary_ready'
        elif case=='wrong_offset':schema['scan']['layout'][1]['lsb']+=1
        elif case=='bad_word_base':schema['scan']['word_map'][0]['index']=0
        elif case=='legacy_alias':schema['ff'][0]['word']=schema['ff'][0]['legacy_parallel_word']
        elif case=='wrong_group':schema['groups'][0]['bits'][0]['package']=schema['groups'][1]['bits'][0]['package']
        elif case=='missing_precapture':suffix='.u_'+manifest['ff'][0]['package']+'.d_pre'
        else:suffix='.snapshot_bank_0.bits[0].read_q'
        if suffix:
            before=len(data['candidates']);data['candidates']=[r for r in data['candidates'] if not r['path'].endswith(suffix)]
            assert len(data['candidates'])<before, 'control target absent: '+case
        try:classify(data,schema,root)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged ContB inventory accepted: '+case)
    return results


def run(root,out):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(root)
    (out/'board.sv').write_text(board)
    for name,text in clones.items():(out/name).write_text(text)
    command=['verilator','--xml-only','--top-module',gen.MODULE,'-Wno-fatal','-DDORADO_F10415_M10K',
             '-GSYSPER=4','--xml-output',str(out/'board.xml'),str(out/'board.sv'),
             *[str(out/n) for n in clones],*[str(root/n) for n in manifest['grouping_sources']]]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w') as log:subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'board.xml').read_bytes());result=classify(report,manifest,root)
    result['failure_controls']=controls(report,manifest,root)
    paths=[Path(__file__),Path(gen.__file__),gen.ROOT/'tools/sil_state_inventory.py',root/gen.SOURCE,
           *[root/n for n in manifest['grouping_sources']]]
    result['input_hashes']={str(p):gen.sha(p) for p in paths}
    for name,data in [('manifest',manifest),('inventory',report),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')
    print('CONTB SCAN CLASSIFICATION PASS:147809physical+250private bits;14bad controls reject')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.source_root.resolve(),a.output.resolve())
