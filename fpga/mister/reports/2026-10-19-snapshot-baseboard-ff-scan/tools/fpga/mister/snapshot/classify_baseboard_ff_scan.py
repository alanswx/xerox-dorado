#!/usr/bin/env python3
"""Fresh BaseBoard serial-access XML: complete physical and private state.

EPROM logical read bytes select raw d or restored_d; the extra restore byte
and pending selector implement the already-qualified canonical pipeline.
Empty sockets retain the original uninitialized-array semantics.
"""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import generate_baseboard_ff_scan as gen

sys.path.insert(0,str(gen.ROOT/'tools'))
from sil_state_inventory import inventory


def classify(report,manifest,root):
    old=gen.reference(root)[1]
    assert manifest==gen.generate(root)[1], 'unreviewed BaseBoard source/schema'
    expected={};roms={};prefix=gen.MODULE+'.'
    def add(path,width,dims=(),physical=True):
        assert path not in expected
        expected[path]=dict(width=width,bits=width*(dims[0][1]-dims[0][0]+1 if dims else 1),
                            dimensions=list(dims),physical=physical)
    def fields(base,fs):
        for f in fs:add(prefix+base+'.'+f['name'],f['width'])
    layout=[];offset=0;packages=[]
    for row in old['packages']:
        r=copy.deepcopy(row);p=r['package'];kind=r['type']
        if kind in ('FF','CPU'):
            fs=r['fields'] if kind=='FF' else r['schema']['fields']
            layout.append(dict(package=p,kind=r['kind'],lsb=offset,width=r['logical_state_bits'],fields=fs))
            offset+=r['logical_state_bits'];r['legacy_parallel_word']=r.pop('word')
            fields(p if kind=='FF' else p+'.u_cpu',fs)
        elif kind=='SRAM':
            base=prefix+p+'.memory.'
            add(base+'mem',8,[[0,1023]]);add(base+'saved_q',8)
            add(base+'ram_q',8,physical=False)
            for name in ('pending','response_pipeline','frozen','done','error'):add(base+name,1,physical=False)
        elif kind=='EPROM':
            add(prefix+p+'.d',8);add(prefix+p+'.en_q',1)
            for name,width in dict(restored_d=8,restore_pending=1,state_frozen=1,state_done=1,state_error=1,state_result=18).items():
                add(prefix+p+'.'+name,width,physical=False)
            if r['init_file']:roms[prefix+p+'.mem']=r
        else:
            add(prefix+p+'.phase_d',1)
            add(prefix+p+'.u_riot.riot_ram',8,[[0,127]])
            fields(p+'.u_riot',r['schema']['child_manifest']['fields'])
            for name,width in dict(phase_frozen=1,response_vendor=1,phase_result=18,state_done=1,state_error=1).items():
                add(prefix+p+'.'+name,width,physical=False)
            for name,width in dict(state_frozen=1,state_done=1,state_error=1,state_result=18).items():
                add(prefix+p+'.u_riot.'+name,width,physical=False)
        packages.append(r)
    assert offset==2004 and manifest['packages']==packages
    assert manifest['state_access_schema']=='compact-ff-cpu-v1'
    assert manifest['scan']==dict(state_bits=2004,words=112,word_bits=18,base_word=0,layout=layout,
        word_map=[dict(index=i,lsb=18*i,width=min(18,2004-18*i)) for i in range(112)],last_word_bits=6,completion_edges=2004)
    for name,width in dict(regular_done=1,regular_error=1,state_boundary_ready=1,
                           state_response_package=6,state_response_child=1,state_response_scan=1).items():
        add(prefix+name,width,physical=False)
    for name,width in dict(frozen=1,writing=1,payload=18,position=11,first_bit=11,busy=1,done=1,error=1,fault=1,result=18).items():
        add(prefix+'u_ff_scan.'+name,width,physical=False)
    decisions=[];immutable=0
    for row in report['candidates']:
        path=row['path']
        if path in roms:
            rom=roms.pop(path)
            assert (row['category'],row['width'],row['bits'],row['unpacked_dimensions'])==('initial_only_candidate',8,16384,[[0,2047]]), 'EPROM shape/mutability differs'
            assert gen.sha(root/rom['init_file'])==rom['sha256'], 'EPROM firmware differs'
            decisions.append(dict(path=path,bits=16384,scope='IMMUTABLE BUILD ASSET',sha256=rom['sha256']))
            immutable+=16384;continue
        if row['category']!='sequential':continue
        entry=expected.pop(path,None)
        assert entry,'extra/duplicate sequential state: '+path
        assert (row['width'],row['bits'],row['unpacked_dimensions'])==(
            entry['width'],entry['bits'],entry['dimensions']), 'state geometry differs: '+path
        decisions.append(dict(path=path,bits=row['bits'],scope='PHYSICAL' if entry['physical'] else 'PRIVATE TRANSPORT'))
    assert not expected and not roms, 'missing state/assets: '+str(list(expected)[:3])
    physical=sum(r['bits'] for r in decisions if r['scope']=='PHYSICAL')
    private=sum(r['bits'] for r in decisions if r['scope']=='PRIVATE TRANSPORT')
    assert (physical,private,immutable)==(40366,582,65536),(physical,private,immutable)
    return dict(status='PASS',scope=__doc__,physical_bits=physical,private_transport_bits=private,
        immutable_image_bits=immutable,sequential_exclusions=[],decisions=decisions,xml_sha256=report['xml_sha256'])


def controls(report,manifest,root):
    targets=dict(lost_cpu='.u_f61.u_cpu.q',lost_counter='.u_g21.cnt',lost_history='.u_g21.ck_d',
        lost_clock='.u_clockgen.phase',lost_riot_ram='.u_c62.u_riot.riot_ram',lost_pa7='.u_c62.u_riot.old_pa7',
        lost_riot_phase='.u_c62.phase_d',lost_sram='.snapshot_sram_0.memory.mem',
        lost_sram_pipeline='.snapshot_sram_0.memory.saved_q',lost_borrowed_pipeline='.snapshot_sram_0.memory.ram_q',
        lost_eprom='.u_b61.d',lost_eprom_restored='.u_b61.restored_d',lost_eprom_enable='.u_b61.en_q',
        lost_boundary='.state_boundary_ready')
    results=[]
    for case in [*targets,'ram_depth','extra_state','duplicate','wrong_counter','wrong_offset','legacy_alias','wrong_image','mutable_rom','wrong_map']:
        data,schema=copy.deepcopy(report),copy.deepcopy(manifest)
        if case in targets:
            before=len(data['candidates']);data['candidates']=[r for r in data['candidates'] if not r['path'].endswith(targets[case])]
            assert len(data['candidates'])<before, 'missing damaged-control target: '+case
        elif case=='ram_depth':next(r for r in data['candidates'] if r['path'].endswith(targets['lost_sram']))['unpacked_dimensions']=[[0,511]]
        elif case in ('extra_state','duplicate'):
            row=copy.deepcopy(next(r for r in data['candidates'] if r['category']=='sequential'))
            if case=='extra_state':row['path']+='_hidden'
            data['candidates'].append(row)
        elif case=='wrong_counter':next(r for r in data['candidates'] if r['path'].endswith('.u_ff_scan.position'))['width']=10
        elif case=='wrong_offset':schema['scan']['layout'][-1]['lsb']+=1
        elif case=='legacy_alias':schema['packages'][0]['word']=schema['packages'][0]['legacy_parallel_word']
        elif case=='wrong_image':next(r for r in schema['packages'] if r['type']=='EPROM' and r['init_file'])['sha256']='0'*64
        elif case=='mutable_rom':next(r for r in data['candidates'] if r['path'].endswith('.u_b61.mem'))['category']='sequential'
        else:schema['scan']['word_map'][-1]['width']=18
        try:classify(data,schema,root)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged BaseBoard state accepted: '+case)
    return results


def run(root,out):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(root);(out/'board.sv').write_text(board)
    for n,s in clones.items():(out/n).write_bytes(gen.raw_bytes(s))
    command=['verilator','--xml-only','--top-module',gen.MODULE,'-GSYSPER=4','-Wno-fatal',
        '-DDORADO_F10415_M10K','--xml-output',str(out/'board.xml'),str(out/'board.sv'),
        *[str(out/n) for n in clones],*[str(root/n) for n in manifest['original_sources']]]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with (out/'elaborate.log').open('w') as log:subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
    report=inventory((out/'board.xml').read_bytes());result=classify(report,manifest,root)
    result['failure_controls']=controls(report,manifest,root)
    for name,data in [('manifest',manifest),('inventory',report),('classification',result)]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')
    print(f"BASEBOARD SCAN CLASSIFICATION PASS:40366 physical+582 private+65536 immutable bits;{len(result['failure_controls'])} damaged controls reject")


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.source_root.resolve(),a.output.resolve())
