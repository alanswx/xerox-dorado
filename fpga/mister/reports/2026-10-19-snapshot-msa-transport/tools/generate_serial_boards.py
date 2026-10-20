#!/usr/bin/env python3
"""Namespace qualified serial boards and add forced-hold/separate-abort hooks.

Original pin equations, clocks, state maps and helper behavior are preserved.
This creates board components, not native backend/ARM barriers or a core build.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import serial_state_format as fmt
from generate_snapshot_ff import masked_comments

ROOT=Path(__file__).resolve().parents[3]
CLASSIFICATION={
 'ContA':'classification-v2','ContB':'classification','ProcH':'ProcH/classification',
 'ProcL':'ProcL/classification','IFU':'classification','MemC':'classification',
 'MemD':'classification','MemX':'classification','DskEth':'classification',
 'DispY':'classification','BaseBd':'classification-v2'}


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def raw_receipt(root,name):
    import gzip
    row=fmt.receipt_index(root)[name];p=root/row['stored']
    raw=gzip.decompress(p.read_bytes()) if p.suffix=='.gz' else p.read_bytes()
    if len(raw)!=row['raw_bytes'] or sha(raw)!=row['raw_sha256']:raise ValueError('receipt bytes differ: '+name)
    return raw


def once(text,before,after):
    if text.count(before)!=1:raise ValueError('unreviewed qualified source: '+before[:90])
    return text.replace(before,after,1)


def rename(text,names):
    # Preserve strings, comments, whitespace, CRLF and all other code bytes.
    pattern=r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|[A-Za-z_$][A-Za-z0-9_$]*'
    return re.sub(pattern,lambda m:names.get(m[0],m[0]),text,flags=re.S)


def modules(text):
    return re.findall(r'(?m)^\s*module\s+(\w+)\b',masked_comments(text))


def qualified(reports,name):
    spec=next(s for s in fmt.SPECS if s[0]==name)
    _,_,_,folder,version=spec
    root=reports/('2026-10-19-snapshot-'+folder+'-ff-scan')
    prefix='reset-fit/' if name=='ContA' else (name+'/' if name in ('ProcH','ProcL') else '')+'fit/'
    record=json.loads(raw_receipt(root,prefix+'provenance.json'))
    manifest=record['manifest'];sources={};paths={}
    candidates=[]
    for path in record['compiled_files']:
        if path.startswith('snapshot/') and path.endswith(('.sv','.v','.mem')):
            raw=raw_receipt(root,prefix+path)
            if sha(raw)!=record['compiled_files'][path]:raise ValueError('compiled receipt differs: '+path)
            relative=path[len('snapshot/'):];sources[relative]=raw;paths[relative]=prefix+path
            if manifest['module'] in modules(raw.decode()):candidates.append(relative)
    if len(candidates)!=1:raise ValueError('qualified board source missing/ambiguous')
    board=candidates[0]
    if sha(sources[board])!=manifest['generated_sha256']:raise ValueError('qualified board body differs')
    for path,digest in manifest['cloned_sources'].items():
        if sha(sources[path])!=digest:raise ValueError('qualified helper differs: '+path)
    classification=CLASSIFICATION[name]
    inventory=json.loads(raw_receipt(root,classification+'/inventory.json'))
    decisions=json.loads(raw_receipt(root,classification+'/classification.json'))
    if decisions['status']!='PASS' or decisions['physical_bits']!=spec[2]:raise ValueError('unqualified physical state coverage')
    cpp_command=json.loads(raw_receipt(root,version+'/candidate/command.json'))
    top=raw_receipt(root,prefix+'snapshot/top.sv').decode()
    binding=re.search(r'\b'+manifest['module']+r'\s*(#\(.*?\))?\s+\w+\s*\(',top,re.S)
    if not binding:raise ValueError('actual measured parameter binding missing')
    metadata=None
    if name=='ContB':
        value=json.loads(raw_receipt(root,version+'/ports.json'))
        ports=[dict(name=p,width=1,direction=direction) for direction,key in [('input','inputs'),('output','outputs')] for p in value[key]]
    else:
        metadata=json.loads(raw_receipt(root,version+'/fixture-metadata.json'));ports=metadata['ports']
    return dict(root=root,prefix=prefix,manifest=manifest,sources=sources,source_paths=paths,board=board,
                inventory=inventory,classification=decisions,ports=ports,parameters=binding[1] or '',
                simulation_defines=[a for a in cpp_command if a.startswith('-D')])


def adapt_board(text):
    text=once(text,'input wire state_system_reset,',
              'input wire state_global_hold,state_global_abort,\n    input wire state_system_reset,')
    hold=list(re.finditer(r'\bwire\s+state_hold\s*=\s*(.*?);',text));accept=list(re.finditer(r'\bwire\s+state_accept\s*=\s*(.*?);',text))
    if len(hold)!=1 or len(accept)!=1:raise ValueError('unreviewed hold/access owner')
    m=hold[0];expression=m[1]
    if not re.fullmatch(r'\(state_freeze\s*&&\s*state_guest_held\)\s*\|\|\s*(?:state_)?scan_hold_required',expression):raise ValueError('unreviewed local hold expression')
    text=text[:m.start(1)]+'state_global_hold || ('+expression+')'+text[m.end(1):]
    m=re.search(r'\bwire\s+state_accept\s*=\s*(.*?);',text)
    if 'state_request' not in m[1] or '!state_system_reset' not in m[1]:raise ValueError('unreviewed admission/reset gate')
    text=text[:m.end(1)]+' && !state_global_abort'+text[m.end(1):]
    pattern=r'\bdorado_ff_scan_controller\s*#\(.*?\)\s+u_ff_scan\s*\((.*?)\)\s*;'
    m=re.search(pattern,text,re.S)
    if not m:raise ValueError('missing physical scan controller')
    body=once(m[1],'.freeze(state_freeze)', '.freeze(state_freeze && !state_global_abort)')
    return text[:m.start(1)]+body+text[m.end(1):]


def generate(reports,name):
    original=qualified(reports,name);manifest=original['manifest']
    helpers={n:original['sources'][n] for n in manifest['cloned_sources']}
    controller=(ROOT/'fpga/mister/snapshot/ff_scan_controller.sv').read_bytes()
    normalization=None
    if helpers['ff_scan_controller.sv']!=controller:
        old=helpers['ff_scan_controller.sv']
        if name!='ContA' or old.count(b'first_bit <= index*18;')!=1 or old.replace(
            b'first_bit <= index*18;',b"first_bit <= COUNT_BITS'(index*18);",1)!=controller:
            raise ValueError('unreviewed scan-controller normalization')
        bits=manifest['scan']['state_bits'];words=(bits+17)//18;width=bits.bit_length()
        assert (bits,words,width)==(681,38,10)
        assert all(i*18<1<<width for i in range(words))
        normalization=dict(previous_sha256=sha(old),chosen_sha256=sha(controller),
            proof='The only change is explicit destination-width truncation; valid indices0..37 assign0..666, fitting10bits. Invalid indices do not assign first_bit.')
        helpers['ff_scan_controller.sv']=controller
    names={manifest['module']:'dorado_serial_'+name}
    for raw in helpers.values():
        for module in modules(raw.decode()):
            if module in names:raise ValueError('duplicate qualified module definition')
            names[module]='dorado_serial_'+name+'__'+module
    board=adapt_board(original['sources'][original['board']].decode())
    port_names={'scan_hold_required':'state_scan_hold_required','scan_fault':'state_scan_fault'} if name=='ContA' else {}
    generated={name+'__board.sv':rename(rename(board,port_names),names).encode()}
    for path,raw in helpers.items():generated[name+'__'+path]=rename(raw.decode(),names).encode()
    record=dict(board=name,module=names[manifest['module']],state_bits=manifest['logical_state_bits'],
        parameters=original['parameters'],namespaces=names,administrative_port_renames=port_names,
        qualified_board_sha256=sha(original['sources'][original['board']]),
        original_sources={n:sha(s) for n,s in original['sources'].items()},
        source_paths=original['source_paths'],normalization=normalization,
        generated_sources={n:sha(s) for n,s in generated.items()},
        scope=__doc__)
    return generated,record,original


def prepare(reports,out):
    out.mkdir(parents=True,exist_ok=False);records=[];seen=set()
    for name,*_ in fmt.SPECS:
        generated,record,_=generate(reports,name)
        for path,raw in generated.items():
            if path in seen:raise ValueError('generated filename conflict')
            seen.add(path);(out/path).write_bytes(raw)
        records.append(record)
    catalog=fmt.build_catalog(reports)
    (out/'catalog.json').write_text(json.dumps(catalog,indent=2)+'\n')
    (out/'assets.json').write_text(json.dumps(dict(scope=__doc__,boards=records,board_bits=sum(r['state_bits'] for r in records),
        schema_id=catalog['schema_id'],generator_sha256=sha(Path(__file__).read_bytes())),indent=2)+'\n')
    print('Prepared11serial boards with separate hold/abort, unique module names;',sum(r['state_bits'] for r in records),'bits;not integrated/qualified')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reports',type=Path,default=ROOT/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();prepare(a.reports.resolve(),a.output.resolve())
