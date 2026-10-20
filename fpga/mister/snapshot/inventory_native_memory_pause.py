#!/usr/bin/env python3
"""Account actual QSF native memory-pause state, including task-written RAM.

The generic XML inventory deliberately flags subroutine writes for review.
Review global nonblocking task destinations reached from a positive-edge
process as required state. Review function scratch lifetime and definite writes.
Wrapper state remains ACCESS PENDING; top emu/UIO, external RAM and ARM are
outside this XML scope. This is not a complete machine snapshot claim.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess
import xml.etree.ElementTree as ET
import inventory_serial_chipwidth as prior
from classify_serial_boards import read_inventory,alias_controls
from sil_state_inventory import destinations

ROOT=prior.ROOT
P=prior.PREFIX


def sha(raw):return hashlib.sha256(raw).hexdigest()


def definitely_written_return(function):
    """Prove the simple static lookup functions overwrite return before use."""
    name=function.get('name')
    def reads(node,defined):
        assert defined or not any(v.get('name')==name for v in node.iter('varref')),'function reads old return value'
    def sequence(nodes,defined):
        for node in nodes:
            if node.tag=='var':continue
            if node.tag=='assign':
                reads(node[0],defined)
                assert node[1].tag=='varref'and node[1].get('name')==name,'unreviewed static function destination'
                defined=True
            elif node.tag=='begin':defined=sequence(node,defined)
            elif node.tag=='if':
                reads(node[0],defined)
                left=sequence([node[1]],defined)
                right=sequence([node[2]],defined)if len(node)>2 else defined
                defined=left and right
            elif node.tag=='case':
                reads(node[0],defined);branches=[];default=False
                for item in node.findall('./caseitem'):
                    statements=[n for n in item if n.tag in ('assign','if','case','begin')]
                    assert statements,'empty case branch'
                    default|=item[0].tag in ('assign','if','case','begin')
                    branches.append(sequence(statements,defined))
                defined=all(branches)and(default or defined)
            else:raise AssertionError('unreviewed static function statement: '+node.tag)
        return defined
    assert sequence(function,False),'function can retain old return value'


def review_subroutines(data,normalized):
    data=copy.deepcopy(data);tree=ET.fromstring(normalized)
    modules={m.get('name'):m for m in tree.findall('.//module')};required=[];scratch=[]
    for row in data['candidates']:
        if row['category']!='subroutine_review':continue
        module=modules[row['module']];name=row['name']
        if name.startswith('@'):
            loc,variable=name[1:].rsplit('.',1)
            functions=[f for f in module.findall('./func')if f.get('loc')==loc]
            assert len(functions)==1,'unreviewed function/task local'
            function=functions[0];locals_={v.get('name')for v in function.findall('.//var')}
            assert variable in locals_ and not function.findall('.//assigndly')
            for assign in function.iter():
                if assign.tag in ('assign','contassign'):
                    assert destinations(assign[1])<=locals_,'function writes persistent global state'
            source=Path(row['source']).read_text()
            automatic=bool(re.search(r'\bfunction\s+automatic\b[^;]*\b'+re.escape(function.get('name'))+r'\b',source))
            if not automatic:
                assert variable==function.get('name'),'unreviewed static function scratch'
                definitely_written_return(function)
            scratch.append(dict(path=row['path'],bits=row['bits'],reason=(
                'Automatic function-local lifetime; no global writes or nonblocking assignments.'if automatic else
                'Static return is assigned on every path before any use; no global writes or nonblocking assignments.'),source_sha256=sha(source.encode())))
            continue
        tasks=[]
        for task in module.findall('./task'):
            writes=[a for a in task.findall('.//assigndly')if name in destinations(a[1])]
            if writes:tasks.append(task.get('name'))
        assert tasks,'unreviewed global subroutine state: '+row['path']
        called=set()
        for process in module.findall('./always'):
            if any(s.get('edgeType')=='POS'for s in process.findall('./sentree/senitem')):
                called.update(t.get('name')for t in process.findall('.//taskref'))
        assert set(tasks)<=called,'task not reached from reviewed positive-edge process'
        row['category']='sequential';row['review']='REQUIRED: global nonblocking task destination reached from positive-edge process'
        required.append(dict(path=row['path'],bits=row['bits'],width=row['width'],dimensions=row['unpacked_dimensions'],tasks=tasks))
    assert {r['path']for r in required}=={P+'u_sdram.rows',P+'u_sdram.dq_out'},'new/unreviewed global task state'
    assert sum(r['bits']for r in required)==120
    return data,dict(required=required,required_bits=120,function_scratch_review=scratch,
                     function_scratch_bits=sum(r['bits']for r in scratch))


def expected_fields(reports):
    expected=prior.expected_fields(reports)
    expected[P+'u_memory_pause.phase']=(2,2,[],'WRAPPER ACCESS PENDING')
    expected[P+'u_sdram.rows']=(13,104,[[0,7]],'WRAPPER ACCESS PENDING')
    expected[P+'u_sdram.dq_out']=(16,16,[],'WRAPPER ACCESS PENDING')
    return expected


def classify(data,expected):
    remaining=dict(expected);decisions=[]
    for row in data['candidates']:
        assert not(row['category']=='subroutine_review'and not row['name'].startswith('@')),'unreviewed global task state'
        if row['category']!='sequential':continue
        wanted=remaining.pop(row['path'],None);assert wanted is not None,'extra/duplicate state: '+row['path']
        width,bits,dims,scope=wanted
        assert(row['width'],row['bits'],row['unpacked_dimensions'])==(width,bits,dims),'native state geometry differs: '+row['path']
        decisions.append(dict(path=row['path'],bits=bits,scope=scope))
    assert not remaining,'missing state: '+str(list(remaining)[:4])
    totals=Counter()
    for row in decisions:totals[row['scope']]+=row['bits']
    assert dict(totals)=={'PHYSICAL BOARD':1957048,'PRIVATE BOARD':4578,'PRIVATE TRANSPORT':69,'WRAPPER ACCESS PENDING':132606},dict(totals)
    wanted={P+'m.b_'+name:dict(SYSPER=4 if name in('DskEth','BaseBd')else 16)for name,*_ in prior.gen.fmt.SPECS}
    wanted[P+'m.b_MemD']['STORAGE_INPUT_DELAY']=2
    wanted.update({P+'m.'+name:dict(SYSPER=4)for name in prior.INSTANCES})
    wanted[P+'m.u_snapshot_fabric']=dict(RESPONSE_LIMIT=4096,SERVICE_DOMAINS=5,AGE_BITS=12)
    assert data['bindings']==wanted,'original board/transport parameters differ'
    return dict(status='PASS_NATIVE_MEMORY_PAUSE_STATE_ACCESS_PENDING',scope=__doc__,totals=dict(totals),
        sequential_exclusions=[],decisions=decisions,bindings=data['bindings'],
        xml_sha256=data['xml_sha256'],normalized_xml_sha256=data['normalized_xml_sha256'],resolved_module_aliases=data['resolved_module_aliases'])


def controls(data,expected):
    cases=['missing','extra','duplicate','width','dimensions','nonsequential','missing_phase','missing_rows','missing_dq_out','rows_geometry','missing_task_review','clock_binding','input_delay']
    results=[]
    for label in cases:
        bad=copy.deepcopy(data);rows=bad['candidates'];target=next(r for r in rows if r['path']==P+'m.b_msa.u_storage_0.cache_data')
        if label=='missing':rows.remove(target)
        elif label in ('extra','duplicate'):
            added=copy.deepcopy(target)
            if label=='extra':added['path']=P+'hidden_state'
            rows.append(added)
        elif label=='width':target['width']+=1
        elif label=='dimensions':target['unpacked_dimensions']=[[0,1]]
        elif label=='nonsequential':target['category']='initial_only_candidate'
        elif label.startswith('missing_')and label!='missing_task_review':
            suffix={'missing_phase':'u_memory_pause.phase','missing_rows':'u_sdram.rows','missing_dq_out':'u_sdram.dq_out'}[label]
            rows.remove(next(r for r in rows if r['path']==P+suffix))
        elif label=='rows_geometry':next(r for r in rows if r['path']==P+'u_sdram.rows')['unpacked_dimensions']=[[0,6]]
        elif label=='missing_task_review':next(r for r in rows if r['path']==P+'u_sdram.rows')['category']='subroutine_review'
        elif label=='clock_binding':bad['bindings'][P+'m.b_DispY']['SYSPER']=4
        else:bad['bindings'][P+'m.b_MemD']['STORAGE_INPUT_DELAY']=0
        try:classify(bad,expected)
        except AssertionError as e:results.append(dict(case=label,rejected=True,diagnostic=str(e)))
        else:raise AssertionError('damaged native inventory accepted: '+label)
    return results


def run(project,reports,out):
    record=json.loads((project/'memory-pause-integration.json').read_text());sources=[];defines=[]
    for line in(project/'core/Dorado.qsf').read_text().splitlines():
        words=shlex.split(line,comments=True)
        if words[:3]==['set_global_assignment','-name','VERILOG_MACRO']:defines.append(words[3])
        if words[:3]in(['set_global_assignment','-name','VERILOG_FILE'],['set_global_assignment','-name','SYSTEMVERILOG_FILE']):
            path=Path(words[3]);remote=Path(record['project'])
            path=project/path.relative_to(remote)if path.is_absolute()and path.is_relative_to(remote)else(project/'core'/path).resolve()
            if path.is_relative_to(project/'src'):sources.append(path)
    sources=list(dict.fromkeys(sources));defines=sorted(set(defines));assert all(p.is_file()for p in sources)
    assert'DORADO_STORAGE_INPUT_DELAY=2'in defines and'DORADO_HOST_CLOCK_HZ=50000000'in defines
    out.mkdir(parents=True,exist_ok=False)
    command=['verilator','--xml-only','--timing','--assert','--top-module','dorado_native_machine','-Wno-fatal',
        *[f'-I{project/"src"/p}'for p in('verilog/cells','verilog/generated','verilog/vendor/6502','verilog/vendor/riot','fpga/mister/snapshot_serial')],
        *['+define+'+d for d in defines],'--xml-output',str(out/'native.xml'),*map(str,sources)]
    (out/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    with(out/'elaborate.log').open('w')as log:subprocess.run(command,cwd=project/'src',stdout=log,stderr=subprocess.STDOUT,check=True)
    raw=(out/'native.xml').read_bytes();data,normalized=read_inventory(raw);data['bindings']=prior.bindings(normalized)
    data,review=review_subroutines(data,normalized);expected=expected_fields(reports);result=classify(data,expected)
    result['subroutine_review']=review;result['failure_controls']=controls(data,expected)+alias_controls(raw)
    provenance=dict(macros=defines,source_hashes={str(p.relative_to(project)):sha(p.read_bytes())for p in sources},qsf_sha256=sha((project/'core/Dorado.qsf').read_bytes()))
    (out/'native-normalized.xml').write_bytes(normalized)
    for name,value in [('inventory',data),('classification',result),('expected-fields',expected),('provenance',provenance)]:
        (out/(name+'.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],totals=result['totals'],subroutine_required_bits=review['required_bits'],function_scratch_bits=review['function_scratch_bits'],failure_controls=len(result['failure_controls']))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('project',type=Path);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.project.resolve(),a.reports.resolve(),a.output.resolve())
