#!/usr/bin/env python3
"""Check the instantiated grant parameter and every native sequential process.

The one changed owner guard is normalized only using the source-bound grant
proof. Every other sensitive process must match the already-audited common-
clock parent. This composes that parent audit; it is not a new timing pass.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def run(stage, parent_lint, parent_audit, lint, proof, out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_NATIVE_SHARED_PUBLICATION_GRANT'and p['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    pa=json.loads((parent_audit/'results.json').read_text())
    assert pa['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'
    assert pa['stage_manifest_sha256']==m['parent_manifest_sha256']
    before=json.loads((parent_lint/'run-state.json').read_text());verify(parent_lint,before['files'])
    after=json.loads((lint/'run-state.json').read_text());verify(lint,after['files'])
    assert before['stage_manifest_sha256']==m['parent_manifest_sha256']
    assert after['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert before['status']==after['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'
    assert all(a['returncode']==0 for a in after['actions'])and len(after['actions'])==2
    roots=[ET.parse(d/'native.xml').getroot()for d in [parent_lint,lint]]
    modules=[{v.get('name'):v for v in r.findall('.//module')}for r in roots]
    native_root='tb_native_cedar.dut'
    cells=[{c.get('hier'):c for c in r.findall('.//cells//cell')
            if c.get('hier')==native_root or c.get('hier').startswith(native_root+'.')}for r in roots]
    assert cells[0]and cells[0].keys()==cells[1].keys()
    def canonical(node):
        attrs=[]
        for k,v in node.attrib.items():
            if k in ['loc','dtype_id']:continue
            if node.tag=='sformatf'and k=='name':v=re.sub(r'(\.(?:sv|v):)\d+(: Assertion failed)',r'\1LINE\2',v)
            attrs.append((k,v))
        return node.tag,tuple(sorted(attrs)),tuple(canonical(c)for c in node)
    counts=Counter();changed=[]
    for path in cells[0]:
        old=modules[0][cells[0][path].get('submodname')]
        new=modules[1][cells[1][path].get('submodname')]
        assert old.get('origName')==new.get('origName'),path
        def sensitive(mod):
            return [a for a in mod.findall('.//always')if any(i.get('edgeType')not in ['COMBO','TRUE']for i in a.findall('./sentree/senitem'))]
        aa,bb=sensitive(old),sensitive(new);assert len(aa)==len(bb),path
        for a,b in zip(aa,bb):
            counts['sensitive_processes']+=1
            if canonical(a)==canonical(b):continue
            assert old.get('origName')=='dorado_r_front_owned_packet',path
            old_if=a.find('if');new_if=b.find('if')
            assert old_if is not None and new_if is not None
            assert old_if[0].tag=='and'and [x.get('name')for x in old_if[0]]==['contb_advance','control_ready']
            assert new_if[0].tag=='varref'and new_if[0].get('name')=='owner_commit'
            new_if.remove(new_if[0]);new_if.insert(0,old_if[0])
            assert canonical(a)==canonical(b),(path,'guard-normalized process differs')
            param=new.find("var[@name='EXTERNAL_READY_GRANT']/const")
            assert param is not None and param.get('name')=="32'sh1"
            changed.append(path)
    assert len(changed)==1,changed
    warnings=[]
    for d in [parent_lint,lint]:
        warnings.append(Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',(d/'lint.log').read_text(),re.M)))
    # The integer-valued parameter is elaborated to literal 1 above. Verilator
    # additionally reports its 32-bit conditional test; no runtime data is
    # truncated. Account for precisely that diagnostic, and no other change.
    expected=warnings[0].copy();expected['WIDTHTRUNC']+=1
    assert warnings[1]==expected,warnings
    messages=(lint/'lint.log').read_text()
    assert messages.count("Conditional Test's VARREF 'EXTERNAL_READY_GRANT' generates 32 bits.")==1
    out.mkdir(parents=True)
    result=dict(status='PASS_NATIVE_SHARED_GRANT_STRUCTURAL_AUDIT',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                parent_audit_sha256=sha(parent_audit/'results.json'),parent_process_counts=pa['process_counts'],
                proof_state_sha256=sha(proof/'run-state.json'),instances=len(cells[0]),
                sensitive_processes=counts['sensitive_processes'],proved_guard_changes=changed,
                actual_caller_parameter=1,warning_counts=dict(warnings[1]),
                physical_timing_or_firmware_or_hardware_speed_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['instances'],result['sensitive_processes'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','parent-lint','parent-audit','lint','proof','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args()
    run(a.stage.resolve(),a.parent_lint.resolve(),a.parent_audit.resolve(),a.lint.resolve(),a.proof.resolve(),a.output.resolve())
