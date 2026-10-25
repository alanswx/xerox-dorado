#!/usr/bin/env python3
"""Check every elaborated native process and every routed advance connection.

This is a structural audit of actual instantiated ASTs, not a sequential
equivalence proof or timing pass. Removing precisely one added enable guard
must recover each old guest process; every host process must be unchanged.
Check all instance identities, clock ports, and advance-port ancestry too.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import xml.etree.ElementTree as ET

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def audit(stage,before,inventory,after,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    old=json.loads((before/'run-state.json').read_text());verify(before,old['files'])
    new=json.loads((after/'run-state.json').read_text());verify(after,new['files'])
    assert new['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')and len(new['actions'])==2
    assert all(a['returncode']==0 for a in new['actions'])
    inv=json.loads((inventory/'results.json').read_text());assert sha(inventory/'results.json')==m['clock_enable_inventory_sha256']
    roots=[ET.parse(d/'native.xml').getroot()for d in [before,after]]
    assert sha(before/'native.xml')==m['clock_enable_XML_sha256']
    modules=[{v.get('name'):v for v in r.findall('.//module')}for r in roots]
    cells=[{c.get('hier'):c for c in r.findall('.//cells//cell')if c.get('hier').startswith(inv['native_root_path'])}for r in roots]
    assert cells[0].keys()==cells[1].keys()
    selected=set(m['converted_module_types']);counter=Counter();seen=set();bindings=[]
    def canonical(node,guard=None):
        if node.tag=='if'and len(node)>=2 and node[0].tag=='varref'and node[0].get('name')==guard:
            assert len(node)==2,ET.tostring(node)
            counter['removed_guards']+=1
            return canonical(node[1])
        children=[]
        for c in node:children.extend(canonical(c,guard))
        if node.tag=='begin'and not node.get('name'):return children
        attrs=[]
        for k,v in node.attrib.items():
            if k in ['loc','dtype_id']:continue
            if node.tag=='sformatf'and k=='name':v=re.sub(r'(\.(?:sv|v):)\d+(: Assertion failed)',r'\1LINE\2',v)
            attrs.append((k,v))
        attrs=tuple(sorted(attrs))
        return [(node.tag,attrs,tuple(children))]
    policy_by_process={(p['path'],p['loc']):p['classification']for p in inv['processes']}
    def formal(mod,port):
        name=port.get('name')
        if name.startswith('__pinNumber'):
            vv=[v.get('name')for v in mod.findall('var')if v.get('pinIndex')==port.get('portIndex')];assert len(vv)==1
            name=vv[0]
        return name
    for path in cells[0]:
        bm=modules[0][cells[0][path].get('submodname')];am=modules[1][cells[1][path].get('submodname')]
        original=bm.get('origName');expected=original+'_ce'if original in selected else original
        assert am.get('origName')==expected,(path,expected,am.get('origName'))
        # Verilator also emits synthetic combinational aliases as `always`;
        # their temporary names include a module-name hash. Audit edge-sensitive
        # processes here; source edit recovery covers the original equations.
        def sensitive(mod):
            return [a for a in mod.findall('.//always')if any(i.get('edgeType')not in ['COMBO','TRUE']for i in a.findall('./sentree/senitem'))]
        bp=sensitive(bm);ap=sensitive(am);assert len(bp)==len(ap),path
        for b,a in zip(bp,ap):
            policy=policy_by_process.get((path,b.get('loc')))
            guard=('contb_advance'if original=='dorado_native_machine'else'guest_advance')if policy=='VIRTUAL_CLOCK_ENABLE_REQUIRED'else None
            saved=counter['removed_guards'];assert canonical(b)==canonical(a,guard),(path,b.get('loc'),policy)
            assert counter['removed_guards']-saved==bool(guard),(path,b.get('loc'),policy)
            if policy:counter[policy]+=1
        # Check each source-bound child input uses the right parent enable.
        for child in cells[1][path].findall('cell'):
            cm=modules[1][child.get('submodname')]
            if cm.get('origName')not in {n+'_ce'for n in selected}:continue
            assert original in selected
            candidates=[i for i in am.findall('.//instance')if i.get('defName')==child.get('submodname')and i.get('loc')==child.get('loc')]
            assert candidates,(path,child.attrib)
            for instance in candidates:
                ports=[p for p in instance.findall('port')if formal(cm,p)=='guest_advance']
                assert len(ports)==1 and len(ports[0])==1
                expr=ports[0][0];expected='contb_advance'if original=='dorado_native_machine'else'guest_advance'
                assert expr.tag=='varref'and expr.get('name')==expected,(path,child.attrib,ET.tostring(expr))
            bindings.append(child.get('hier'))
    assert counter['VIRTUAL_CLOCK_ENABLE_REQUIRED']==1823 and counter['HOST_CLOCK_RETAINS_EXISTING_POLICY']==1975
    assert counter['removed_guards']==1823
    # Reject newly exposed optimization-loop diagnostics separately from XML.
    lint=(after/'lint.log').read_text();warnings=Counter()
    for warning in re.findall(r'%Warning-([A-Z0-9_]+)',lint):warnings[warning]+=1
    baseline=Path(old['stage']).parent/'native-operand-buffer-full15-lint-v1-20261025'/'lint.log'
    result=dict(status='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT',scope=__doc__,
        stage=str(stage),stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        inventory_sha256=sha(inventory/'results.json'),before_XML_sha256=sha(before/'native.xml'),after_state_sha256=sha(after/'run-state.json'),
        actual_instances=len(cells[0]),process_counts=dict(counter),advance_bindings_checked=len(bindings),
        lint_warning_counts=dict(warnings),combinational_loop_warnings=warnings['UNOPTFLAT'],
        full_equivalence_or_firmware_or_physical_timing_qualified=False,hardware_changed=False)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],dict(counter),'UNOPTFLAT',warnings['UNOPTFLAT'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','before','inventory','after','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();audit(a.stage.resolve(),a.before.resolve(),a.inventory.resolve(),a.after.resolve(),a.output.resolve())
