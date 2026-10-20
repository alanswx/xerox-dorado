#!/usr/bin/env python3
"""Inverse-source proof of eleven-board namespace and global hold/abort conversion.

Erase exactly four approved administrative edits and invert the bijective name
map: every original board/helper byte must return. No ordinary pin, clock,
memory map, process or source change is permitted. Timing remains separate.
"""
import argparse
import copy
import json
from pathlib import Path
import re
import generate_serial_boards as gen


LITERALS=r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"'


def masked(text):
    return re.sub(LITERALS,lambda m:''.join('\n' if c=='\n' else ' ' for c in m[0]),text,flags=re.S)


def undo(text,mapping):
    # Use identifier locations in independently masked source, leaving all
    # comments/literals and nonidentifier bytes untouched.
    hits=list(re.finditer(r'\b[A-Za-z_$][A-Za-z0-9_$]*\b',masked(text)))
    for m in reversed(hits):
        if m[0] in mapping:text=text[:m.start()]+mapping[m[0]]+text[m.end():]
    return text


def erase_hooks(text):
    fragment='input wire state_global_hold,state_global_abort,\n    '
    assert text.count(fragment)==1,'missing/duplicate global interface'
    text=text.replace(fragment,'',1)
    hold=list(re.finditer(r'\bwire\s+state_hold\s*=\s*(.*?);',text))
    assert len(hold)==1
    m=hold[0];expr=m[1];prefix='state_global_hold || ('
    assert expr.startswith(prefix) and expr.endswith(')'),'forced hold missing'
    text=text[:m.start(1)]+expr[len(prefix):-1]+text[m.end(1):]
    accept=list(re.finditer(r'\bwire\s+state_accept\s*=\s*(.*?);',text));assert len(accept)==1
    m=accept[0];suffix=' && !state_global_abort'
    assert m[1].endswith(suffix),'abort admission guard missing'
    text=text[:m.end(1)-len(suffix)]+text[m.end(1):]
    fragment='.freeze(state_freeze && !state_global_abort)'
    assert text.count(fragment)==1,'scan abort hook missing/duplicated'
    return text.replace(fragment,'.freeze(state_freeze)',1)


def audit_board(reports,name,files,record):
    original=gen.qualified(reports,name);manifest=original['manifest']
    names={manifest['module']:'dorado_serial_'+name}
    for path in manifest['cloned_sources']:
        code=masked(original['sources'][path].decode())
        for module in re.findall(r'(?m)^\s*module\s+(\w+)\b',code):names[module]='dorado_serial_'+name+'__'+module
    assert record['namespaces']==names and len(set(names.values()))==len(names),'namespace map not bijective/reviewed'
    inverse={v:k for k,v in names.items()}
    assert record['administrative_port_renames']==({'scan_hold_required':'state_scan_hold_required','scan_fault':'state_scan_fault'} if name=='ContA' else {})
    assert set(files)=={name+'__board.sv',*[name+'__'+p for p in manifest['cloned_sources']]},'missing/extra board/helper file'
    all_defs=[]
    for path,raw in files.items():
        text=raw.decode();tokens=set(re.findall(r'\b[A-Za-z_$][A-Za-z0-9_$]*\b',masked(text)))
        assert not(tokens&names.keys()),'unconverted original helper reference'
        all_defs+=re.findall(r'(?m)^\s*module\s+(\w+)\b',masked(text))
        restored=undo(text,inverse)
        if path==name+'__board.sv':
            restored=undo(restored,{v:k for k,v in record['administrative_port_renames'].items()})
            restored=erase_hooks(restored)
            expected=original['sources'][original['board']]
        else:
            helper=path[len(name+'__'):];expected=original['sources'][helper]
            if helper=='ff_scan_controller.sv':
                chosen=(gen.ROOT/'fpga/mister/snapshot/ff_scan_controller.sv').read_bytes()
                if expected!=chosen:
                    assert name=='ContA' and expected.count(b'first_bit <= index*18;')==1
                    assert expected.replace(b'first_bit <= index*18;',b"first_bit <= COUNT_BITS'(index*18);",1)==chosen,'unreviewed controller delta'
                    assert all((i*18)&1023==i*18 for i in range(38))
                    expected=chosen
        assert restored.encode()==expected,'ordinary source/process/map differs: '+path
        assert re.findall(LITERALS,text,re.S)==re.findall(LITERALS,expected.decode(),re.S),'comment/literal/asset bytes changed'
        assert record['generated_sources'][path]==gen.sha(raw),'asset source hash differs'
    assert len(all_defs)==len(set(all_defs)) and set(all_defs)==set(names.values()),'missing/duplicate module definitions'
    return dict(board=name,status='PASS',state_bits=manifest['logical_state_bits'],modules=len(all_defs),
        exact_source_identity_after_inverse=True,normal_behavior_when_global_inputs_zero='Boolean identities and bijective alpha rename; unchanged ordinary source bytes',
        parameters=original['parameters'],source_hashes={p:gen.sha(raw) for p,raw in files.items()})


def controls(reports,name,files,record):
    results=[];board=name+'__board.sv'
    cases=[('hold',board,'state_global_hold || (','('),
        ('abort_admission',board,' && !state_global_abort;',';'),
        ('scan_abort',board,'.freeze(state_freeze && !state_global_abort)', '.freeze(state_freeze)'),
        ('ordinary_advance',board,'.state_advance(state_guest_advance)',".state_advance(1'b1)"),
        ('missing_interface',board,'state_global_hold,state_global_abort','state_global_abort'),
        ('helper_binding',board,record['namespaces']['dorado_ff_scan_controller'],'dorado_ff_scan_controller')]
    for case,path,before,after in cases:
        copies=dict(files);raw=copies[path];assert before.encode() in raw,(name,case)
        copies[path]=raw.replace(before.encode(),after.encode(),1)
        schema=copy.deepcopy(record)
        # Do not let a trivial receipt digest detect the corruption: the
        # independent inverse proof must reject even with a matching digest.
        schema['generated_sources'][path]=gen.sha(copies[path])
        try:audit_board(reports,name,copies,schema)
        except AssertionError as error:results.append(dict(case=case,rejected=True,diagnostic=str(error)))
        else:raise AssertionError('damaged board conversion accepted: '+name+'/'+case)
    return results


def run(reports,assets,out):
    records=json.loads((assets/'assets.json').read_text());results=[];definitions=set()
    assert records['board_bits']==1944744 and len(records['boards'])==11
    for record in records['boards']:
        files={n:(assets/n).read_bytes() for n in record['generated_sources']}
        result=audit_board(reports,record['board'],files,record)
        result['failure_controls']=controls(reports,record['board'],files,record)
        for target in record['namespaces'].values():
            assert target not in definitions,'cross-board duplicate module';definitions.add(target)
        results.append(result)
    out.write_text(json.dumps(dict(status='PASS',scope=__doc__,boards=results,board_bits=sum(r['state_bits'] for r in results),
        module_definitions=len(definitions),failure_controls=sum(len(r['failure_controls']) for r in results)),indent=2)+'\n')
    print('SERIAL BOARD SOURCE AUDIT PASS:11boards/1944744bits;',len(definitions),'unique modules;66damaged controls reject')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('assets',type=Path)
    p.add_argument('--reports',type=Path,default=gen.ROOT/'fpga/mister/reports');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();run(a.reports.resolve(),a.assets.resolve(),a.output.resolve())
