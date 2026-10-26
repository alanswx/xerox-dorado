#!/usr/bin/env python3
"""Check the actual staged prepared-Hold wiring and every old sequential process."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

from audit_native_memc_hold_producer import BOARD, BACKPLANE
from stage_native_hold_buffer import TARGET
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def audit(stage,parent_lint,lint,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
    assert sha(parent/'r-front-control-native.json')==m['parent_manifest_sha256']
    assert m['prepared_MemC_Hold_staged']and not m['assembly_permitted']
    assert set(m['source_hashes'])-set(pm['source_hashes'])=={TARGET}
    changed={n for n in pm['source_hashes']if(stage/n).read_bytes().replace(m['project'].encode(),pm['project'].encode())!=(parent/n).read_bytes()}
    assert changed=={BOARD,BACKPLANE,'core/Dorado.qsf'},changed
    board=(stage/BOARD).read_text();header=m['prepared_MemC_Hold_header_original']
    assert board.count('    output wire [131:0] hold_source_context,\n')==1
    board=board.replace('    output wire [131:0] hold_source_context,\n'+header,header,1)
    assert board.count(m['prepared_MemC_Hold_export'])==1
    assert board.replace(m['prepared_MemC_Hold_export'],'',1)==(parent/BOARD).read_text()
    native=(stage/BACKPLANE).read_text()
    for a,b in reversed(m['prepared_MemC_Hold_backplane_reversible_edits']):
        assert native.count(b)==1,b
        native=native.replace(b,a,1)
    assert native==(parent/BACKPLANE).read_text()
    states=[]
    for d,manifest in [(parent_lint,m['parent_manifest_sha256']),(lint,sha(stage/'r-front-control-native.json'))]:
        r=json.loads((d/'run-state.json').read_text());verify(d,r['files'])
        assert r['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'and r['stage_manifest_sha256']==manifest
        assert len(r['actions'])==2 and all(a['returncode']==0 for a in r['actions']);states.append(r)
    roots=[ET.parse(d/'native.xml').getroot()for d in [parent_lint,lint]]
    modules=[{v.get('name'):v for v in r.findall('.//module')}for r in roots]
    cells=[{v.get('hier'):v for v in r.findall('.//cells//cell')if v.get('hier')=='tb_native_cedar.dut'or v.get('hier').startswith('tb_native_cedar.dut.')}for r in roots]
    def canonical(node):
        attrs=[]
        for k,v in node.attrib.items():
            if k in ['loc','dtype_id']:continue
            if node.tag=='sformatf'and k=='name':v=re.sub(r'(\.(?:sv|v):)\d+(: Assertion failed)',r'\1LINE\2',v)
            attrs.append((k,v))
        return node.tag,tuple(sorted(attrs)),tuple(canonical(c)for c in node)
    def sensitive(mod):return [a for a in mod.findall('.//always')if any(v.get('edgeType')not in ['COMBO','TRUE']for v in a.findall('./sentree/senitem'))]
    assert set(cells[0])<=set(cells[1]);count=0
    for path in cells[0]:
        old,new=[modules[i][cells[i][path].get('submodname')]for i in range(2)]
        assert old.get('origName')==new.get('origName'),path
        aa,bb=sensitive(old),sensitive(new);assert len(aa)==len(bb),path
        for a,b in zip(aa,bb):assert canonical(a)==canonical(b),(path,a.get('loc'))
        count+=len(aa)
    added=set(cells[1])-set(cells[0]);assert len(added)==1
    path=next(iter(added));assert path.endswith('.m.hold_frontier'),path
    new=modules[1][cells[1][path].get('submodname')]
    assert new.get('origName')=='dorado_memc_hold_buffer'and len(sensitive(new))==1
    assert sensitive(new)[0].find('./sentree/senitem/varref').get('name')=='clk'
    assert count==3813 and len(cells[0])==12787 and len(cells[1])==12788
    warnings=[Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',(d/'lint.log').read_text(),re.M))for d in [parent_lint,lint]]
    delta=warnings[1].copy();delta.subtract(warnings[0]);delta={k:v for k,v in delta.items()if v}
    assert delta=={},delta
    out.mkdir(parents=True)
    result=dict(status='PASS_NATIVE_PREPARED_HOLD_STRUCTURAL_AUDIT',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                source_files_verified=len(m['source_hashes']),unchanged_original_sequential_processes=count,
                added_host_preparation_processes=1,instances_before=len(cells[0]),instances_after=len(cells[1]),
                new_host_instance=path,all_original_guest_clock_processes_and_storage_files_unchanged=True,
                all_original_readiness_guards_retained_with_one_additional_context_barrier=True,
                lint_warning_delta=delta,warning_explanation='Actual optimized native lint has unchanged warnings; CE instance connects the new context output.',
                raw_MemC_Hold_export_retained=True,native_timing_preparation_waits_60ns_or_firmware_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','parent-lint','lint','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();audit(a.stage.resolve(),a.parent_lint.resolve(),a.lint.resolve(),a.output.resolve())
