#!/usr/bin/env python3
"""Full frozen processor-board restore oracle with compact serial FF access.

Derive only administrative access/reset/fault handling from the qualified ContA
scan test. Preserve the original processor's physical/output/ROM/writer tests.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_processor_ff_scan as gen
import generate_processor_full as reference
import test_processor_full as qualified
import test_conta_ff_scan as scan_test
from generate_conta_ff_scan import once

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(source,manifest):
    old_manifest = reference.generate(manifest['board'],source)[1]
    top,header,metadata = qualified.fixture(source,old_manifest)
    top = once(top,'input wire host_clk,run,run_ref,input wire',
               'input wire host_clk,run,run_ref,system_reset,input wire')
    marker = f" output wire advance,ref_advance,output wire [{len(manifest['files'])+len(manifest['rams'])-1}:0] ref_write);"
    top = once(top,marker,marker.replace('advance,ref_advance,','advance,ref_advance,scan_hold_required,scan_fault,'))
    top = once(top,'.state_host_clk(host_clk)',
               '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    bits = manifest['scan']['state_bits']
    words = [dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    words += metadata['state_words'][len(manifest['ff']):]
    assert sum(r['width'] for r in words) == manifest['logical_state_bits']
    old_count = len(metadata['state_words'])
    header = header[:header.index('struct StateWord')].replace(f'WORDS={old_count}',f'WORDS={len(words)}')
    header += f"constexpr unsigned SCAN_BITS={bits},SCAN_WORDS={manifest['scan']['words']};\n"
    header += 'struct StateWord { unsigned word,width,offset; };\nconstexpr StateWord state_words[]={\n'
    header += ',\n'.join('{'+f"{r['word']},{r['width']},{r['offset']}"+'}' for r in words)+'};\n'
    metadata = dict(metadata,state_words=words,compact_ff_bits=bits)
    return top,header,metadata


def driver(manifest):
    original = (HERE/'test_processor_full.cpp').read_text()
    _,conta = scan_test.driver()
    begin,end = 'static unsigned access(', '\nint main('
    access = conta[conta.index(begin):conta.index(end)]
    access = access.replace('word<38','word<SCAN_WORDS').replace('waited<=682','waited<=SCAN_BITS+1').replace('waited==681','waited==SCAN_BITS')
    text = once(original,original[original.index(begin):original.index(end)],access)
    text = once(text,'    d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;',
                '    d.system_reset=0;d.host_clk=1;d.run=0;d.run_ref=0;d.freeze=0;d.held=0;')
    first = '    for (unsigned i=0;i<20000;++i) {\n        pins();bool advance=(random_word()&3)!=0;\n        cycle(advance,advance,i%3==0);'
    start = conta.index('    d.freeze=1;d.held=1;d.request=0;cycle(false,false);\n    d.system_reset=1;')
    reset_entry = conta[start:conta.index(first,start)]
    text = once(text,first,reset_entry+first)
    start = conta.index('    d.request=0;cycle(false,false);\n    d.freeze=1;d.held=1;cycle(false,false);\n    d.request=1;d.write=1;')
    stop = conta.index('    check(walked_bits==1353',start)
    anchor = '    check(walked_bits==STATE_BITS&&late_changes>10000&&holds>10000,"insufficient coverage");'
    text = once(text,anchor,conta[start:stop]+anchor)
    text = text.replace('(75u<<5)|16',str(manifest['files'][0]['word']|16))
    return original,text


def run(kind,root,output,mutations):
    output.mkdir(parents=True,exist_ok=False)
    source = root/'verilog/generated'/{'ProcH':'ProcH-Rev-Ce.v','ProcL':'ProcL-Rev-Ci.v'}[kind]
    board,manifest,clones = gen.generate(kind,source)
    manifest['board'] = kind
    top,header,metadata = fixture(source,manifest)
    original_cpp,cpp = driver(manifest)
    inputs = output/'executed-inputs'
    inputs.mkdir()
    (inputs/'original-processor-test.cpp').write_text(original_cpp)
    (inputs/'test_processor_ff_scan.cpp').write_text(cpp)
    originals = [root/'verilog/cells'/(k+'.v') for k in manifest['original_sources']]
    deltas = []
    for p in originals:
        if sha(p) != manifest['original_sources'][p.stem]:
            assert p.name == 'cell_MC10170.v'
            current = gen.ROOT/'verilog/cells'/p.name
            assert sha(current) == manifest['original_sources'][p.stem]
            tokens = lambda p:re.sub(r'\s+','',re.sub(r'/\*.*?\*/|//[^\n]*','',p.read_text(),flags=re.S))
            assert '"' not in tokens(p) and tokens(p) == tokens(current)
            deltas.append(dict(path=p.name,actual_sha256=sha(p),workspace_sha256=sha(current)))
    for prom in manifest['immutable_proms']:
        assert sha(root/prom['file']) == sha(gen.ROOT/prom['file']) == prom['sha256']
    names = ['test_processor_ff_scan.py','generate_processor_ff_scan.py','test_processor_full.py','test_processor_full.cpp',
             'generate_processor_full.py','generate_processor_ff.py','generate_conta_ff_scan.py','test_conta_ff_scan.py',
             'test_conta_full.cpp','generate_snapshot_ff.py','generate_contb_full.py','generate_contb.py',
             'generate_f10145.py','generate_f10145_shared_write.py','generate_f10145_zero_write.py',
             'generate_async256.py','ff_scan_controller.sv']
    paths = [HERE/n for n in names]+originals+[source]+[root/p['file'] for p in manifest['immutable_proms']]
    hashes = {str(p):sha(p) for p in paths}
    for p in paths:
        (inputs/p.name).write_bytes(p.read_bytes())
    (output/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__,kind=kind,source_hashes=hashes,
        derived_cpp_sha256=sha(inputs/'test_processor_ff_scan.cpp'),comment_only_frozen_deltas=deltas),indent=2)+'\n')
    cases = [('candidate',None,None,None,None)]
    if mutations:
        bits = manifest['scan']['state_bits']
        cases += [
            ('lost_chain_bit','board',f'scan_bits[{bits-1}:1]',f"{{scan_bits[{bits-1}:2],1'b0}}",'board physical state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board physical state differs'),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required','(state_freeze && state_guest_held)','quarantine released'),
            ('reset_access','board',' && !state_system_reset;',';','system reset admitted access'),
        ]
    results = []
    for name,target,before,after,diagnostic in cases:
        case = output/name
        case.mkdir()
        text,copies = board,dict(clones)
        if target:
            if target == 'board':
                assert before in text
                text = text.replace(before,after,1)
            else:
                copies[target] = once(copies[target],before,after)
        (case/'board.sv').write_text(text)
        (case/'fixture.sv').write_text(top)
        (case/'ports.h').write_text(header)
        for n,s in copies.items():
            (case/n).write_text(s)
        command = ['verilator','--cc','--exe','--build','-j','4','--top-module','processor_full_fixture','-Wno-fatal',
                   '-DDORADO_F10145_LOGIC','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),str(case/'fixture.sv'),
                   str(case/'board.sv'),*[str(case/n) for n in copies],str(source),*map(str,originals),str(inputs/'test_processor_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        result = subprocess.run([str(case/'obj/Vprocessor_full_fixture')],cwd=gen.ROOT,capture_output=True,text=True,timeout=180)
        log = result.stdout+result.stderr
        (case/'test.txt').write_text(log)
        print(kind+'/'+name+': '+log.strip(),flush=True)
        if diagnostic:
            assert result.returncode and diagnostic in log,(name,log)
        else:
            assert result.returncode == 0 and 'FULL PROCESSOR RESTORE PASS' in log,log
        results.append(dict(case=name,returncode=result.returncode,expected_failure=diagnostic))
    assert all(sha(Path(p)) == h for p,h in hashes.items()), 'source changed during test'
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    (output/'fixture-metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
    (output/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,kind=kind,cases=results,source_hashes=hashes),indent=2)+'\n')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--kind',choices=gen.SPECS,required=True)
    p.add_argument('--frozen-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true')
    a = p.parse_args()
    run(a.kind,a.frozen_root.resolve(),a.output.resolve(),a.mutations)
