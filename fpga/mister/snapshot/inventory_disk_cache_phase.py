#!/usr/bin/env python3
"""Fresh original/prototype disk XML; preserve every original field plus18bits.

Compare with the actual native Serial64 disk inventory, not a list inferred
from the generator. All extra fields remain REQUIRED wrapper snapshot state.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET
from generate_serial_boards import ROOT,raw_receipt
from classify_serial_boards import read_inventory
import generate_disk_cache_phase as gen
import audit_disk_cache_phase as source_audit

PREFIX='dorado_pack_hps.'
EXTRA={PREFIX+'cache_source_cyl':(12,12,[]),PREFIX+'cache_source_head':(6,6,[])}


def geometry(data):
    result={}
    for r in data['candidates']:
        if r['category']!='sequential':continue
        assert r['path']not in result,'duplicate state'
        result[r['path']]=(r['width'],r['bits'],r['unpacked_dimensions'])
    return result


def expected(reports):
    data=json.loads(raw_receipt(reports/'2026-10-19-snapshot-serial-chipwidth','classification/inventory.json'))
    rows=[r for r in data['candidates']if r['category']=='sequential'and r['path'].startswith('dorado_native_machine.u_media.')]
    result={PREFIX+r['path'].split('.')[-1]:(r['width'],r['bits'],r['unpacked_dimensions'])for r in rows}
    assert len(result)==55 and sum(v[1]for v in result.values())==131816
    return result


def classify(data,wanted,candidate):
    have=geometry(data);expect=dict(wanted)
    if candidate:expect.update(EXTRA)
    assert have==expect,'original/extra disk state geometry differs'
    return dict(status='PASS_DISK_GEOMETRY_ACCESS_PENDING',original_fields=55,original_bits=131816,
                total_fields=len(have),total_bits=sum(v[1]for v in have.values()),extra_required_snapshot_bits=18 if candidate else 0,
                sequential_exclusions=[],limits='Complete wrapper snapshot access is not implemented.')


def controls(data,wanted):
    out=[]
    for name in ('missing_original','missing_buffer','missing_extra','wrong_extra_width','extra_hidden','duplicate','nonsequential'):
        bad=copy.deepcopy(data);rows=bad['candidates'];extra=next(r for r in rows if r['path']==PREFIX+'cache_source_cyl')
        if name=='missing_original':rows.remove(next(r for r in rows if r['path']==PREFIX+'cyl'))
        elif name=='missing_buffer':rows.remove(next(r for r in rows if r['path']==PREFIX+'buffer'))
        elif name=='missing_extra':rows.remove(extra)
        elif name=='wrong_extra_width':extra['width']-=1
        elif name=='extra_hidden':r=copy.deepcopy(extra);r['path']+='2';rows.append(r)
        elif name=='duplicate':rows.append(copy.deepcopy(extra))
        else:extra['category']='initial_only_candidate'
        try:classify(bad,wanted,True)
        except AssertionError as e:out.append(dict(case=name,rejected=True,reason=str(e)))
        else:raise AssertionError('damaged disk inventory accepted: '+name)
    return out


def edge_bindings(normalized,candidate):
    root=ET.fromstring(normalized);edges=[]
    for node in root.findall('.//always'):
        writes={a[-1].get('name'):a[0].get('name')for a in node.findall('.//assigndly')if a[-1].tag=='varref'}
        if {'cache_source_cyl','cache_source_head'}&set(writes):
            sensitivity=[(x.get('edgeType'),x.find('varref').get('name'))for x in node.findall('./sentree/senitem')]
            assert sensitivity==[('NEG','clk_sys')],sensitivity
            assert writes=={'cache_source_cyl':'cyl','cache_source_head':'head'},writes
            edges.append(sensitivity)
    assert len(edges)==(1 if candidate else 0)
    return edges


def edge_controls(normalized):
    out=[]
    for name in ('edge','clock','source'):
        root=ET.fromstring(normalized)
        node=next(n for n in root.findall('.//always')if any(a[-1].get('name')=='cache_source_cyl'for a in n.findall('.//assigndly')))
        if name=='edge':node.find('./sentree/senitem').set('edgeType','POS')
        elif name=='clock':node.find('./sentree/senitem/varref').set('name','machine_clk')
        else:node.find('.//assigndly/varref').set('name','cached_cyl')
        try:edge_bindings(ET.tostring(root),True)
        except AssertionError as e:out.append(dict(case='wrong_'+name,rejected=True,reason=str(e)))
        else:raise AssertionError('damaged disk clock binding accepted: '+name)
    return out


def run(out,reports):
    out.mkdir(parents=True,exist_ok=False);original=source_audit.reference(reports);candidate=gen.transform(original.decode()).encode()
    source=source_audit.audit(candidate,reports);source['failure_controls']=source_audit.controls(candidate,reports)
    wanted=expected(reports);results={}
    for label,raw in [('original',original),('candidate',candidate)]:
        case=out/label;case.mkdir();path=case/'medium.sv';path.write_bytes(raw)
        # LABEL_WORDS10 and the 50MHz host match the native Cedar medium.
        command=['verilator','--xml-only','--timing','--assert','--top-module','dorado_pack_hps','-Wno-fatal',
                 '-GLABEL_WORDS=10','-GHOST_CLOCK_HZ=50000000','--xml-output',str(case/'disk.xml'),str(path)]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with(case/'elaborate.log').open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        xml=(case/'disk.xml').read_bytes();data,normalized=read_inventory(xml)
        result=classify(data,wanted,label=='candidate')
        if label=='candidate':result['failure_controls']=controls(data,wanted)
        (case/'inventory.json').write_text(json.dumps(data,indent=2)+'\n');(case/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
        # Keep actual edge binding evidence for the new implementation state.
        result['extra_state_edge_bindings']=edge_bindings(normalized,label=='candidate')
        if label=='candidate':result['edge_failure_controls']=edge_controls(normalized)
        (case/'classification.json').write_text(json.dumps(result,indent=2)+'\n')
        results[label]=result
    (out/'source-audit.json').write_text(json.dumps(source,indent=2)+'\n')
    result=dict(status='PASS',scope=__doc__,cases=results,source_controls=len(source['failure_controls']),new_required_wrapper_snapshot_bits=18)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports');a=p.parse_args();run(a.output.resolve(),a.reports.resolve())
