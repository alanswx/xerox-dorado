#!/usr/bin/env python3
"""Count actual elaborated caller SRAMs; calibrate with an observable duplicate.

The library source list contains an unused SRAM-owning module. Count reachable
instances and their unpacked arrays rather than occurrences in source text.
Both the exported packet and the combined tagged-read caller are supported.
The positive must contain one original four-bank9216-bit SRAM. The deliberate
duplicate must be detected as two SRAMs/eight banks18432bits. No timing claim.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time
import xml.etree.ElementTree as ET

from test_external_r_packet import sha


def structure(path):
    root = ET.parse(path).getroot()
    modules = {m.get('name'):m for m in root.iter('module')}
    types = {d.get('id'):d for d in root.findall('.//typetable/*')}
    def shape(ident):
        d = types[ident]
        if d.tag == 'basicdtype':
            return abs(int(d.get('left','0'))-int(d.get('right','0')))+1, []
        assert d.tag == 'unpackarraydtype', (ident, d.tag)
        bounds = [int(c.get('name').split('h',1)[1],16) for c in d.findall('range/const')]
        assert len(bounds) == 2
        width, dims = shape(d.get('sub_dtype_id'))
        return width, [abs(bounds[0]-bounds[1])+1, *dims]
    cells = root.findall('.//cells//cell');arrays = [];srams = []
    for c in cells:
        m = modules[c.get('submodname')]
        if m.get('origName') == 'dorado_r_file_ports':
            srams.append(c.get('hier'))
        for v in m.findall('var'):
            width, dims = shape(v.get('dtype_id'))
            if dims:
                bits = width
                for n in dims:
                    bits *= n
                arrays.append(dict(path=c.get('hier')+'.'+v.get('name'),
                                   width=width, dimensions=dims, bits=bits))
    top = next(m for m in modules.values() if m.get('topModule') == '1')
    widths = {v.get('name'):shape(v.get('dtype_id'))[0] for v in top.findall('var')}
    assert [widths[n] for n in ['r_front_lower_q','r_front_upper_q','r_front_before','r_front_clock_before']] == [572,256,401,1]
    assert [widths[n] for n in ['r_front_visible_r_ProcH','r_front_visible_r_ProcL']] == [8,8]
    assert all(v['width']==9 and v['dimensions']==[256] for v in arrays)
    assert all(any(v['path'].startswith(s+'.') for s in srams) for v in arrays)
    return dict(SRAM_instances=srams, arrays=arrays, total_array_bits=sum(v['bits'] for v in arrays),
                stored_owner_bits=828, external_visible_R_bits=16, stored_history_bits=402)


def run(source, out):
    assert not out.exists()
    p = json.loads((source/'run-state.json').read_text())
    assert p['status'] in ['PASS_EXTERNAL_FULL_PROCESSOR_PACKETS','PASS_COMBINED_CP_PACKET_SINGLE_FALLBACK',
                           'PASS_COMBINED_CP_COHERENT_PACKETS']
    for name, digest in p['files'].items():
        assert sha(source/name) == digest
    paths = [source/r['path'] for r in p['source_files'] if r['path'] != 'tb.sv']
    out.mkdir(parents=True);shutil.copy2(Path(__file__), out/Path(__file__).name)
    s = dict(status='RUNNING', scope=__doc__, started=time.time(), driver_pid=os.getpid(),
             source=str(source), source_state_sha256=sha(source/'run-state.json'),
             source_files={str(v.relative_to(source)):sha(v) for v in paths}, cases=[],
             native_integrated=False, hardware_changed=False, timing_qualified=False)
    def save():
        (out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    save()
    for bad in [False,True]:
        d = out/('duplicate-control' if bad else 'positive');d.mkdir()
        actual = list(paths)
        if bad:
            old = (source/'caller.sv').read_text();raw = old
            header = 'output wire [843:0] r_front_held);'
            new_header = 'output wire [843:0] r_front_held,output wire [17:0] duplicate_probe);'
            if header not in raw:
                header='output wire owner_commit);'
                new_header='output wire owner_commit,output wire [17:0] duplicate_probe);'
            assert raw.count(header) == 1
            raw = raw.replace(header, new_header, 1)
            probe = ''' // Observable duplicate used ONLY to calibrate the structural audit.
 dorado_r_file_ports duplicate_files(storage_clk,owner_commit,memory_write_n,memory_address,memory_select_n,
  memory_write_data,memory_address,memory_select_n,duplicate_probe[15:0],duplicate_probe[17:16]);
'''
            assert raw.count('endmodule') == 1
            raw = raw.replace('endmodule',probe+'endmodule',1)
            assert raw.replace(probe,'',1).replace(new_header,header,1) == old
            (d/'caller.sv').write_text(raw)
            actual = [d/'caller.sv' if v.name=='caller.sv' else v for v in paths]
        cmd = ['verilator','--xml-only','--timing','-Wno-fatal','--top-module','dorado_r_front_owned_packet',
               '--xml-output',str(d/'hierarchy.xml'),'--Mdir',str(d/'obj'),*map(str,actual)]
        with (d/'elaborate.log').open('x') as log:
            child = subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            row = dict(bad=bad, command=cmd, pid=child.pid, started=time.time())
            s['cases'].append(row);save();code = child.wait()
        row.update(returncode=code, finished=time.time(), log_sha256=sha(d/'elaborate.log'));save()
        if code:
            break
        a = structure(d/'hierarchy.xml')
        row.update(structure=a, XML_sha256=sha(d/'hierarchy.xml'))
        count = 2 if bad else 1
        row['passed'] = (len(a['SRAM_instances'])==count and len(a['arrays'])==4*count and
                         a['total_array_bits']==9216*count)
        save()
        if not row['passed']:
            break
    passed = len(s['cases'])==2 and all(v.get('passed') for v in s['cases'])
    s.update(status='PASS_EXTERNAL_PACKET_ONE_SRAM_AND_DUPLICATE_CONTROL' if passed else
             'TERMINAL_EXTERNAL_PACKET_STRUCTURE_NOT_ESTABLISHED', finished=time.time())
    save();print(s['status'],flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a = p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
