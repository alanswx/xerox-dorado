#!/usr/bin/env python3
"""Bind the 844-Q processor/control prototype to the frozen 5c65 native inputs.

Check every frozen native-run input before mapping original package Qs and
functional RAM. This is an integration inventory, not a native replacement,
arrival/timing proof, state import or hardware change.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from inventory_r_front_ready_native import indices,source_bits

ROOT=Path(__file__).resolve().parents[1]
PATHS={
 'ContA':'src/fpga/mister/snapshot_native_conta/ContA_full_snapshot.sv',
 'ProcH':'src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv',
 'ProcL':'src/fpga/mister/snapshot_native_processor/ProcL_full_snapshot.sv',
 'ContB':'src/fpga/mister/snapshot_native/ContB_full_snapshot.sv',
 'MemC':'src/fpga/mister/snapshot_native_memc/MemC_full_snapshot.sv',
}


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def global_mapping(reference):
    raw=reference.read_text();maps={k:{}for k in PATHS}
    m=re.search(r'c\(fixture_load,(\{[^}]+\}),bases\[2\]',raw);assert m
    maps['ContA']=dict(enumerate(source_bits(m[1])))
    m=re.search(r' for\(genvar i=0;i<16;i=i\+1\) begin: map_t_file\n(.*?)\n end\n',raw,re.S);assert m
    expanded='\n'.join(re.sub(r'\bi\b',str(i),m[1])for i in range(16))
    raw=raw[:m.start()]+expanded+raw[m.end():]
    for owner,left,right in re.findall(r'assign\s+(ih|il)\[([^\]]+)\]\s*=\s*([^;]+);',raw):
        board='ProcH'if owner=='ih'else'ProcL';a,b=indices(left),source_bits(right);assert len(a)==len(b)
        for local,global_bit in zip(a,b):assert local not in maps[board];maps[board][local]=global_bit
    assert 'fixture_state[843:798]'in raw
    maps['ContB']={i:798+i for i in range(40)};maps['MemC']={i:838+i for i in range(6)}
    assert [len(maps[k])for k in PATHS]==[110,280,408,40,6]
    flat=[v for m in maps.values()for v in m.values()];assert sorted(flat)==list(range(844))
    return maps


def native_instance(text,pkg,kind):
    matches=list(re.finditer(r'^  (cell_\w+)([^\n]*) u_'+re.escape(pkg)+r'\s*\(\n(.*?)^  \);',text,re.M|re.S))
    assert len(matches)==1,(pkg,len(matches));m=matches[0];assert m[1]=='cell_'+kind+'_snapshot',(pkg,kind,m[1])
    pairs=re.findall(r'\.p(\d+)\(([^()]*)\)',m[3]);pins={int(k):v.strip()for k,v in pairs};assert len(pins)==len(pairs)
    return m,pins


def inventory(native_run,reference,ram_reference,out):
    assert not out.exists();provenance=native_run/'provenance.json.gz'
    source=json.loads(gzip.decompress(provenance.read_bytes()));native=native_run/'inputs'
    assert source['host_clock_hz']==57142857 and 'DORADO_NATIVE'in source['defines']
    for n,h in source['inputs'].items():assert sha(native/n)==h,('Changed frozen native input',n)
    manifest=json.loads((reference/'manifest.json').read_text());assert manifest['physical_bits']==844 and manifest['ram_bits']==9216
    maps=global_mapping(reference/'r_front_control_reference.sv');assets={};texts={};identities={}
    asset_paths=set()
    for board,path in PATHS.items():
        texts[board]=(native/path).read_text();identities[path]=sha(native/path)
        a=str(Path(path).parent/'assets.json');asset_paths.add(a);r=json.loads((native/a).read_text())
        assets[board]=r['manifest']if board=='ContA'else r['board_manifest']if board=='ContB'else r['manifests'][board]
    rows=[];old_history=0
    for board in manifest['boards']:
        name=board['board'].split('-Rev-')[0];metadata={p['package'].removeprefix('u_'):p for key in ['ff','files']for p in assets[name].get(key,[])}
        for pkg,(start,width)in board['sequential'].items():
            kind=board['packages'][pkg];m,pins=native_instance(texts[name],pkg,kind);item=metadata[pkg]
            assert item['kind']=='cell_'+kind
            if kind=='F10145A':
                assert item['ram']['words']*item['ram']['width']==width==64;extra=item['write_window']['width']
            else:
                fields=[p for p in item['fields']if p['name']in ['q','qa','qb']]
                assert sum(p['width']for p in fields)==width,(name,pkg,width,fields)
                extra=item['width']-width
            old_history+=extra
            rows.append(dict(board=name,package=pkg,kind=kind,physical_width=width,
                global_Q_bits_lsb_first=[maps[name][i]for i in range(start,start+width)],
                native_source=PATHS[name],native_instance='u_'+pkg,native_parameters=m[2].strip(),
                physical_pin_connections=pins,old_nonarchitectural_history_bits=extra,existing_metadata=item))
    assert sum(r['physical_width']for r in rows)==844
    flat=[v for r in rows for v in r['global_Q_bits_lsb_first']];assert sorted(flat)==list(range(844))
    retained=[r for r in rows if r['board']in ['ProcH','ProcL']and r['package']in ['g06','g07']]
    assert len(retained)==4 and sorted(v for r in retained for v in r['global_Q_bits_lsb_first'])==list(range(572,588))
    for row in retained:
        assert row['kind']=='MC10173'
        for pin,bit in zip([1,2,15,14],row['global_Q_bits_lsb_first']):assert row['physical_pin_connections'][pin]==f'R_{bit-572:02d}'
    ram_manifest=json.loads((ram_reference/'manifest.json').read_text());assert ram_manifest['physical_bits']==9216
    roles={0:('rm',0,4),1024:('rm',4,4),2048:('stk',0,4),3072:('stk',4,4),4096:('rm',8,1),4352:('stk',8,1)}
    ram_rows=[]
    for half,board in enumerate(ram_manifest['boards']):
        name=board['board'].split('-Rev-')[0];metadata={p['package'].removeprefix('u_'):p for p in assets[name]['rams']}
        for pkg,(start,width)in board['sequential'].items():
            kind=board['packages'][pkg];m,pins=native_instance(texts[name],pkg,kind);item=metadata[pkg]
            prefix,bit_start,word_width=roles[start];assert width==256*word_width and item['words']==256 and item['width']==word_width
            ram_rows.append(dict(board=name,package=pkg,kind=kind,native_source=PATHS[name],native_instance='u_'+pkg,
                physical_pin_connections=pins,reference_RAM_bit_range=[4608*half+start,4608*half+start+width-1],
                component_array=prefix+('_h'if not half else'_l'),component_word_bits=list(range(bit_start,bit_start+word_width)),
                words=256,physical_bits=width,parity=word_width==1,existing_metadata=item))
    assert len(ram_rows)==12 and sum(r['physical_bits']for r in ram_rows)==9216
    out.mkdir(parents=True);frozen=out/'inputs';files={
        'native-provenance.json.gz':provenance,'reference/manifest.json':reference/'manifest.json',
        'reference/r_front_control_reference.sv':reference/'r_front_control_reference.sv',
        'ram-reference/manifest.json':ram_reference/'manifest.json','tools/'+Path(__file__).name:Path(__file__),
        'tools/inventory_r_front_ready_native.py':ROOT/'tools/inventory_r_front_ready_native.py'}
    files.update({n:native/n for n in set(PATHS.values())|asset_paths})
    hashes={}
    for n,p in sorted(files.items()):
        target=frozen/n;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes());hashes[n]=sha(target)
    record=dict(status='PASS_PROCESSOR_CONTROL_NATIVE_INVENTORY',scope=__doc__,native_run=str(native_run),
        inputs=hashes,verified_native_inputs=len(source['inputs']),native_board_identities=identities,
        physical_Q_bits=844,RAM_bits=9216,packages=rows,RAM_packages=ram_rows,
        selected_packages=len(rows),old_history_setup_window_bits=old_history,
        retained_visible_R_Qs=list(range(572,588)),evaluator_owned_Q_bits=828,
        native_replacement_generated=False,arrival_contract_qualified=False,physical_timing_qualified=False,hardware_changed=False)
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(record['status'],len(rows),'packages/844Qs;',old_history,'old history bits;12RAM packages/9216bits;',len(source['inputs']),'native inputs verified')
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--native-run',type=Path,required=True)
    p.add_argument('--reference',type=Path,required=True);p.add_argument('--ram-reference',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();inventory(a.native_run.resolve(),a.reference.resolve(),a.ram_reference.resolve(),a.output.resolve())
