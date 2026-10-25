#!/usr/bin/env python3
"""Inventory actual native virtual-clock processes and clock-as-data uses.

Walk the unflattened instantiated hierarchy from native.sys_clk (VIRTUAL) and
native.clk_host (HOST). Trace direct/inverted aliases and clock port bindings.
List every sensitive process and non-event clock use. Derived clocks remain
explicitly unclassified; this audit is not a clock-enable conversion proof.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import xml.etree.ElementTree as ET

from test_external_r_packet import sha
from review_baseboard_clock_consumer import verify


def audit(source,out):
    assert not out.exists();out.mkdir(parents=True)
    s=json.loads((source/'run-state.json').read_text());verify(source,s['files'])
    assert s['status']=='ELABORATED_NATIVE_CLOCK_ENABLE_AUDIT_INPUT'and s['source_files']==200
    root=ET.parse(source/'native.xml').getroot()
    modules={m.get('name'):m for m in root.findall('.//module')}
    filenames={f.get('id'):f.get('filename')for f in root.findall('./files/file')}
    native=[c for c in root.findall('.//cells//cell')if c.get('submodname')=='dorado_native_machine']
    assert len(native)==1
    processes=[];data_uses=[];ambiguities=[];instances=[]
    def location(loc):
        parts=loc.split(',');return dict(source=filenames.get(parts[0],parts[0]),line=int(parts[1])if len(parts)>1 else None,loc=loc)
    def expression(node,domains):
        if node is None:return None
        if node.tag=='varref':return domains.get(node.get('name'))
        if node.tag in ['not','cast','castsize','extend','extends']and len(node)==1:
            value=expression(node[0],domains)
            return (value[0],value[1]^(node.tag=='not'))if value else None
        return None
    def walk(cell,incoming):
        m=modules[cell.get('submodname')];path=cell.get('hier');domains=dict(incoming)
        aliases=m.findall('.//contassign')
        for unused in range(len(aliases)+1):
            changed=False
            for a in aliases:
                if len(a)!=2 or a[1].tag!='varref':continue
                value=expression(a[0],domains);name=a[1].get('name')
                if value and name not in domains:domains[name]=value;changed=True
            if not changed:break
        instances.append(dict(path=path,module=m.get('name'),original_module=m.get('origName'),clocks={n:list(v)for n,v in domains.items()}))
        parents={c:p for p in m.iter()for c in p}
        for a in m.findall('.//always'):
            events=[i for i in a.findall('./sentree/senitem')if i.get('edgeType')not in ['COMBO','TRUE']]
            if not events:continue
            event_rows=[]
            for item in events:
                exp=item[0]if len(item)else None;domain=expression(exp,domains)
                event_rows.append(dict(edge=item.get('edgeType'),domain=domain[0]if domain else'DERIVED_OR_UNCLASSIFIED',
                    inverted=bool(domain[1])if domain else None,expression=ET.tostring(exp,encoding='unicode')if exp is not None else None))
            targets=set()
            def target(node):
                if node.tag=='varref':return{node.get('name')}
                if node.tag in ['sel','arraysel','wordsel']and len(node):return target(node[0])
                if node.tag=='concat':return set().union(*(target(c)for c in node))
                return {'UNCLASSIFIED:'+node.tag}
            for assignment in a.iter():
                if assignment.tag in ['assign','assigndly']and len(assignment)==2:targets|=target(assignment[1])
            processes.append(dict(path=path,module=m.get('name'),original_module=m.get('origName'),
                **location(a.get('loc','')),events=event_rows,written_variables=sorted(targets),
                classification='VIRTUAL_CLOCK_ENABLE_REQUIRED'if any(v['domain']=='VIRTUAL'for v in event_rows)else
                  'HOST_CLOCK_RETAINS_EXISTING_POLICY'if all(v['domain']=='HOST'for v in event_rows)else'DERIVED_CLOCK_REVIEW_REQUIRED'))
        for var in m.findall('.//varref'):
            name=var.get('name')
            if name not in domains:continue
            ancestors=[];cursor=var
            while cursor in parents:
                cursor=parents[cursor];ancestors.append(cursor)
            if any(p.tag=='senitem'for p in ancestors):continue
            if any(p.tag=='port'for p in ancestors):continue
            # Direct alias forwarding is separately included in propagation.
            if any(p.tag=='contassign'and len(p)==2 and p[1].tag=='varref'and expression(p[0],domains)for p in ancestors):continue
            if any(p.tag=='var'and p.get('param')=='true'for p in ancestors):continue
            data_uses.append(dict(path=path,module=m.get('name'),clock=name,domain=domains[name][0],
                **location(var.get('loc','')),context=ET.tostring(ancestors[0],encoding='unicode')[:1200]if ancestors else''))
        candidates=m.findall('.//instance')
        for child in cell.findall('cell'):
            matches=[i for i in candidates if i.get('defName')==child.get('submodname')and i.get('loc')==child.get('loc')]
            if not matches:matches=[i for i in candidates if i.get('defName')==child.get('submodname')and i.get('name')==child.get('name')]
            assert matches,(path,child.attrib)
            maps=[]
            for instance in matches:
                values={}
                for p in instance.findall('port'):
                    if p.get('direction')!='in' or len(p)!=1:continue
                    value=expression(p[0],domains)
                    if value:
                        child_module=modules[instance.get('defName')]
                        formal={v.get('pinIndex'):v.get('name')for v in child_module.findall('var')if v.get('dir')}
                        name=p.get('name')
                        if name.startswith('__pinNumber'):
                            assert p.get('portIndex')in formal,(path,instance.attrib,p.attrib)
                            name=formal[p.get('portIndex')]
                        values[name]=value
                maps.append(values)
            if any(v!=maps[0]for v in maps):
                ambiguities.append(dict(path=child.get('hier'),candidate_maps=maps))
                common={n:v for n,v in maps[0].items()if all(mp.get(n)==v for mp in maps)}
            else:common=maps[0]
            walk(child,common)
    walk(native[0],{'sys_clk':('VIRTUAL',False),'clk_host':('HOST',False)})
    counts=Counter(p['classification']for p in processes)
    virtual=[p for p in processes if p['classification']=='VIRTUAL_CLOCK_ENABLE_REQUIRED']
    assert virtual and not ambiguities
    virtual_types=Counter(p['original_module']for p in virtual)
    result=dict(status='INVENTORIED_ACTUAL_NATIVE_CLOCK_ENABLE_SINKS',scope=__doc__,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),native_XML_sha256=sha(source/'native.xml'),
        native_root_path=native[0].get('hier'),instance_count=len(instances),process_count=len(processes),
        process_class_counts=dict(counts),virtual_processes_by_original_module=dict(sorted(virtual_types.items())),
        clock_as_data_uses=data_uses,ambiguous_clock_port_bindings=ambiguities,
        processes=processes,instances=instances,clock_enable_conversion_qualified=False,
        all_clock_domains_or_derived_clocks_qualified=False,hardware_or_sources_changed=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');shutil.copy2(Path(__file__),out/Path(__file__).name)
    print(result['status'],dict(counts),'clock-as-data uses',len(data_uses),flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();audit(a.source.resolve(),a.output.resolve())
