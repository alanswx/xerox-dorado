#!/usr/bin/env python3
"""Audit the full Ready-comparison candidate's source and sequential ancestry."""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

from prepare_ready_mode_compare import native_transform, transform
from qualify_native_ready_compare import CONTROL, READY
from review_baseboard_clock_consumer import verify
from stage_native_ready_mode import TARGET
from test_external_r_packet import sha


def audit(stage,parent_lint,lint,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
    assert sha(parent/'r-front-control-native.json')==m['parent_manifest_sha256']
    assert m['parallel_Ready_target_comparison_staged'] and not m['assembly_permitted']
    assert set(m['source_hashes'])-set(pm['source_hashes'])=={TARGET}
    changed={n for n in pm['source_hashes']if(stage/n).read_bytes().replace(m['project'].encode(),pm['project'].encode())!=(parent/n).read_bytes()}
    assert changed=={CONTROL,'core/Dorado.qsf'},changed
    assert(stage/CONTROL).read_text()==native_transform((parent/CONTROL).read_text(),(parent/READY).read_text())
    c=m['parallel_Ready_target_receipts']['component'];component=Path(c['directory'])
    assert sha(component/'results.json')==c['state_sha256']
    assert(stage/TARGET).read_text()==transform((parent/CONTROL).read_text())[1]+'\n'+(component/'compare.sv').read_text()
    for d,manifest in [(parent_lint,m['parent_manifest_sha256']),(lint,sha(stage/'r-front-control-native.json'))]:
        r=json.loads((d/'run-state.json').read_text());verify(d,r['files'])
        assert r['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT' and r['stage_manifest_sha256']==manifest
        assert len(r['actions'])==2 and all(a['returncode']==0 for a in r['actions'])
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
    assert count==3814 and len(cells[0])==12788
    added=set(cells[1])-set(cells[0]);assert added
    additions={}
    for path in sorted(added):
        new=modules[1][cells[1][path].get('submodname')]
        assert not sensitive(new),path
        assert '.ready_target_comparison' in path,path
        additions[path]=new.get('origName')
    warnings=[Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',(d/'lint.log').read_text(),re.M))for d in [parent_lint,lint]]
    delta=warnings[1].copy();delta.subtract(warnings[0]);delta={k:v for k,v in delta.items()if v}
    # The byte-preserved control body repeats its original integer-parameter
    # truth test in the new module. Account for that exact copied diagnostic;
    # do not suppress or allow unrelated width/clock/settling warnings.
    assert delta=={'WIDTHTRUNC':1},delta
    log=(lint/'lint.log').read_text()
    helper_warnings=re.findall(r'^%Warning-([^\n]+)',log,re.M)
    helper_warnings=[v for v in helper_warnings if str(stage/TARGET)+':' in v]
    assert len(helper_warnings)==1
    assert helper_warnings[0].startswith('WIDTHTRUNC: ')
    assert "If's VARREF 'CP_TPC_BLOCK_EARLY' generates 32 bits." in helper_warnings[0]
    out.mkdir(parents=True)
    result=dict(status='PASS_NATIVE_READY_MODE_STRUCTURAL_AUDIT',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        source_files_verified=len(m['source_hashes']),unchanged_original_sequential_processes=count,
        added_sequential_processes=0,instances_before=len(cells[0]),instances_after=len(cells[1]),
        added_combinational_instances=additions,lint_warning_delta=delta,
        warning_explanation='One copied original integer-parameter truth test in the exact control body; no other new warning.',
        all_original_clocks_storage_caller_and_capture_history_unchanged=True,
        original_readiness_guards_retained=True,additional_host_or_guest_cycles=0,
        native_timing_or_speed_or60ns_qualified=False,
        parent_lint_sha256=sha(parent_lint/'run-state.json'),lint_sha256=sha(lint/'run-state.json'))
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(additions),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','parent-lint','lint','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();audit(a.stage.resolve(),a.parent_lint.resolve(),a.lint.resolve(),a.output.resolve())
