#!/usr/bin/env python3
"""Compose the native CE audit with five source-bound RIOT RAM replacements.

Every other instantiated sequential process and every original RIOT input
connection must match the audited compact parent. New memory processes must
use the same clock and virtual advance. All component/ROM proof receipts are
required. This does not establish full-core physical timing or firmware boot.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

from prepare_riot_blockram import candidate,RIOT
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from stage_native_riot_blockram import CELL,MODULE,RAM
from test_external_r_packet import sha


def audit(stage,parent_lint,parent_audit,lint,out):
    assert not out.exists()
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    parent=Path(m['parent_stage']);pm=json.loads((parent/'r-front-control-native.json').read_text());verify(parent,pm['source_hashes'])
    assert sha(parent/'r-front-control-native.json')==m['parent_manifest_sha256']
    assert m['native_RIOT_block_ram_staged']and len(m['source_hashes'])==580
    assert set(m['source_hashes'])-set(pm['source_hashes'])=={MODULE,RAM}
    changed={n for n,h in pm['source_hashes'].items()if Path(n).suffix in ['.v','.sv']and m['source_hashes'][n]!=h}
    assert changed=={CELL}
    original=(parent/CELL).read_text()
    a='M6532_ce u_riot (.guest_advance(guest_advance),'
    b='dorado_riot_blockram_ce u_riot (.guest_advance(guest_advance),'
    assert original.count(a)==1 and (stage/CELL).read_text()==original.replace(a,b,1)
    vendor=(parent/RIOT).read_text()
    assert (stage/MODULE).read_text()==vendor[:vendor.index('module M6532')]+candidate(module(vendor,'M6532_ce'))
    receipts={}
    for name,receipt in m['RIOT_block_ram_receipts'].items():
        d=Path(receipt['directory']);state=d/('results.json'if name in ['component','mapping']else'run-state.json')
        assert sha(state)==receipt['state_sha256']
        r=json.loads(state.read_text());verify(d,r['files']);receipts[name]=r
    assert receipts['proof']['status']=='PASS_RESET_HISTORY_RIOT_RAM_INDUCTION'
    assert receipts['tests']['status']=='PASS_COMPLETE_NATIVE_RIOT_BLOCK_RAM_COMPONENT'
    assert receipts['baseboard']['status']=='PASS_ORIGINAL_ROM_BASEBOARD_RIOT_BLOCK_RAM'
    bb=Path(m['RIOT_block_ram_receipts']['baseboard']['directory'])
    assert sha(stage/CELL)==sha(bb/'cell_MCS6532.v')
    assert sha(stage/MODULE)==sha(bb/'candidate.sv')and sha(stage/RAM)==sha(bb/'ram.sv')
    pa=json.loads((parent_audit/'results.json').read_text())
    assert pa['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'and pa['stage_manifest_sha256']==m['parent_manifest_sha256']
    for d,manifest in [(parent_lint,m['parent_manifest_sha256']),(lint,sha(stage/'r-front-control-native.json'))]:
        l=json.loads((d/'run-state.json').read_text());verify(d,l['files'])
        assert l['status']=='ELABORATED_NATIVE_COMMON_CLOCK_ENABLE_EXPERIMENT'and l['stage_manifest_sha256']==manifest
        assert len(l['actions'])==2 and all(v['returncode']==0 for v in l['actions'])
    roots=[ET.parse(d/'native.xml').getroot()for d in [parent_lint,lint]]
    mods=[{v.get('name'):v for v in r.findall('.//module')}for r in roots]
    cells=[{c.get('hier'):c for c in r.findall('.//cells//cell')if c.get('hier')=='tb_native_cedar.dut'or c.get('hier').startswith('tb_native_cedar.dut.')}for r in roots]
    assert set(cells[0])<=set(cells[1])
    def canonical(node):
        attrs=[]
        for k,v in node.attrib.items():
            if k in ['loc','dtype_id']:continue
            if node.tag=='sformatf'and k=='name':v=re.sub(r'(\.(?:sv|v):)\d+(: Assertion failed)',r'\1LINE\2',v)
            attrs.append((k,v))
        return node.tag,tuple(sorted(attrs)),tuple(canonical(c)for c in node)
    def sensitive(mod):
        return [a for a in mod.findall('.//always')if any(v.get('edgeType')not in ['COMBO','TRUE']for v in a.findall('./sentree/senitem'))]
    def formal(mod,port):
        if not port.get('name').startswith('__pinNumber'):return port.get('name')
        names=[v.get('name')for v in mod.findall('var')if v.get('pinIndex')==port.get('portIndex')];assert len(names)==1
        return names[0]
    count=Counter();replaced=[]
    for path in cells[0]:
        old=mods[0][cells[0][path].get('submodname')];new=mods[1][cells[1][path].get('submodname')]
        if old.get('origName')=='M6532_ce':
            assert new.get('origName')=='dorado_riot_blockram_ce'
            param=new.find("var[@name='init_7800']/const");assert param is not None and param.get('name')=="32'sh0"
            for process in sensitive(new):
                sen=process.find('./sentree/senitem/varref');guard=process.find('./if/varref')
                assert sen.get('name')=='clk'and guard.get('name')=='guest_advance'
            assert len(sensitive(old))==len(sensitive(new))==2
            # Compare the complete actual parent-to-chip input wiring.
            par=path.rsplit('.',1)[0]
            mm=[mods[i][cells[i][par].get('submodname')]for i in range(2)]
            inst=[next(v for v in mm[i].findall('.//instance')if v.get('defName')==cells[i][path].get('submodname'))for i in range(2)]
            ports=[{formal(chip,p):p for p in instance.findall('port')}for chip,instance in zip([old,new],inst)]
            assert ports[0].keys()==ports[1].keys()
            for name in ports[0]:assert canonical(ports[0][name][0])==canonical(ports[1][name][0]),(path,name)
            replaced.append(path);count['old_processes']+=2;count['new_processes']+=2
        else:
            assert old.get('origName')==new.get('origName'),path
            aa,bb=sensitive(old),sensitive(new);assert len(aa)==len(bb)
            for a,b in zip(aa,bb):assert canonical(a)==canonical(b),(path,a.get('loc'))
            count['unchanged_processes']+=len(aa);count['old_processes']+=len(aa);count['new_processes']+=len(bb)
    added=set(cells[1])-set(cells[0]);assert added=={p+'.native_ram'for p in replaced}and len(replaced)==5
    for path in added:
        mod=mods[1][cells[1][path].get('submodname')];assert mod.get('origName')=='dorado_riot_ram'
        assert len(sensitive(mod))==3
        for process in sensitive(mod):
            assert process.find('./sentree/senitem/varref').get('name')=='clk'
            guard=process.find('if')[0]
            assert (guard.tag=='varref'and guard.get('name')=='advance')or(guard.tag=='and'and guard[0].get('name')=='advance')
        owner=mods[1][cells[1][path.rsplit('.',1)[0]].get('submodname')]
        instance=next(v for v in owner.findall('.//instance')if v.get('defName')==mod.get('name'))
        ports={formal(mod,p):p for p in instance.findall('port')}
        assert ports['clk'][0].get('name')=='clk'and ports['advance'][0].get('name')=='guest_advance'
        count['new_processes']+=3
    assert len(cells[0])==12782 and len(cells[1])==12787
    assert count['old_processes']==3798 and count['new_processes']==3813 and count['unchanged_processes']==3788
    warnings=[Counter(re.findall(r'^%Warning-([A-Z0-9_]+):',(d/'lint.log').read_text(),re.M))for d in [parent_lint,lint]]
    assert warnings[0]==warnings[1],warnings
    out.mkdir(parents=True)
    result=dict(status='PASS_NATIVE_RIOT_BLOCK_RAM_STRUCTURAL_AUDIT',stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
                parent_audit_sha256=sha(parent_audit/'results.json'),parent_process_counts=pa['process_counts'],
                instances_before=12782,instances_after=12787,process_counts=dict(count),
                replaced_RIOTs=replaced,new_memory_instances=sorted(added),all_original_chip_input_wiring_retained=True,
                all_new_processes_same_clock_and_advance=True,warning_counts=dict(warnings[1]),
                complete_physical_timing_or_firmware_or_hardware_speed_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','parent-lint','parent-audit','lint','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();audit(a.stage.resolve(),a.parent_lint.resolve(),a.parent_audit.resolve(),a.lint.resolve(),a.output.resolve())
