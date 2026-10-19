#!/usr/bin/env python3
"""Full display original/RAM-oracle restore test with compact FF scan words.

Retain independent physical memory models and the full board oracle. Derive
only administrative mapping, completion waits and local fault/reset checks.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_dispy_ff_scan as gen
import generate_dispy_full as reference
import test_dispy_full as qualified
from generate_conta_ff_scan import once

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(root,manifest):
    old = reference.generate(root)[1]
    top,old_header,metadata = qualified.fixture(root,old)
    top = once(top,'input wire host_clk,run,run_ref,input wire','input wire host_clk,run,run_ref,system_reset,input wire')
    top = once(top,'output wire advance,ref_advance);','output wire advance,ref_advance,scan_hold_required,scan_fault);')
    top = once(top,'.state_host_clk(host_clk)',
               '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    words = [dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    words += metadata['state_words'][140:]
    assert sum(r['width'] for r in words) == 13993
    header = old_header[:old_header.index('struct StateWord')].replace(f"WORDS={len(metadata['state_words'])}",f'WORDS={len(words)}')
    header += 'constexpr unsigned SCAN_BITS=1439,SCAN_WORDS=80;\n'
    header += 'struct StateWord{unsigned word,width,offset;};\nconstexpr StateWord state_words[]={'+','.join('{'+f"{r['word']},{r['width']},{r['offset']}"+'}' for r in words)+'};\n'
    rams = [dict(r,start_word=r['start_word']-60) for r in metadata['rams']]
    header += 'struct Ram{unsigned package,words,width,start_word;};\nconstexpr Ram rams[]={'+','.join('{'+f"{r['package']},{r['words']},{r['width']},{r['start_word']}"+'}' for r in rams)+'};\n'
    return top,header,dict(metadata,state_words=words,rams=rams)


def driver(manifest):
    original = (HERE/'test_dispy_full.cpp').read_text()
    start,end = original.index('static unsigned access('),original.index('\nstatic auto state()')
    window = next(r['word']|16 for r in manifest['memories'] if r['kind']=='cell_F10145A')
    access = f'''static unsigned access(unsigned index,bool write,unsigned data=0,bool invalid=false,bool compare=true){{
    d.state_index=index;d.request=1;d.write=write;d.state_data=data&0x3ffff;
    cycle(false,false,false,false);++requests;unsigned waited=0;
    while(!d.done){{
        d.request=1;d.write=1;d.state_index={window};d.state_data=0x3ffff;
        cycle(false,false,false,false);check(++waited<=SCAN_BITS+1,"scan completion timeout");
    }}
    d.request=0;
    if(index<SCAN_WORDS&&!invalid)check(waited==SCAN_BITS,"scan rotation length differs");
    check(!d.scan_hold_required&&!d.scan_fault,"completed access still held/faulted");
    check(bool(d.error)==invalid,"invalid index accepted");
    if(compare)equal(!(d.freeze&&d.held));
    return d.result;
}}
'''
    text = once(original,original[start:end],access)
    reset = f'''    d.system_reset=0;d.eval();equal();
    d.freeze=d.held=1;d.request=0;cycle(false,false);
    d.system_reset=1;d.request=d.write=1;d.state_index={window};d.state_data=0x3ffff;
    cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset admitted access");equal(false);
    d.system_reset=0;cycle(false,false);check(d.frozen&&!d.done,"reset reentry accepted request");
    d.request=d.write=d.freeze=d.held=0;cycle(false,false);
'''
    text = once(text,'    d.eval();equal();',reset)
    quarantine = f'''    d.request=d.write=0;cycle(false,false);
    d.freeze=d.held=1;cycle(false,false);
    d.request=d.write=1;d.state_index=0;d.state_data=0x12345;cycle(false,false,false,false);
    d.request=0;cycle(false,false,false,false);cycle(false,false,false,false);
    check(d.scan_hold_required,"partial scan lost hold");
    d.freeze=d.held=0;cycle(false,false,false,false);
    check(d.scan_fault&&d.scan_hold_required&&d.done&&d.error,"lost boundary not quarantined");
    auto quarantined=state();d.request=d.write=1;d.state_index={window};d.state_data=0x3ffff;
    for(unsigned i=0;i<30;++i){{pins();cycle(true,false,false,false);
        check(d.scan_fault&&d.scan_hold_required&&d.frozen,"quarantine released");physical(quarantined);}}
    d.system_reset=1;d.freeze=d.held=1;cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
    d.system_reset=0;cycle(false,false,false,false);check(d.frozen&&!d.done,"fault reset reentry accepted request");
    d.request=0;cycle(false,false,false,false);
'''
    marker = '    for(unsigned i=0;i<RAMS;++i){std::printf("writer ram%u=%u\\n",i,writes[i]);'
    text = once(text,marker,quarantine+marker)
    return original,text


def run(root,out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones = gen.generate(root)
    top,header,metadata = fixture(root,manifest)
    old_cpp,cpp = driver(manifest)
    inputs = out/'executed-inputs'
    inputs.mkdir()
    (inputs/'original-display-test.cpp').write_text(old_cpp)
    (inputs/'test_dispy_ff_scan.cpp').write_text(cpp)
    names = ['test_dispy_ff_scan.py','generate_dispy_ff_scan.py','test_dispy_full.py','test_dispy_full.cpp',
             'generate_dispy_full.py','generate_conta_ff_scan.py','generate_conta_full.py','generate_contb_full.py',
             'generate_contb.py','generate_snapshot_ff.py','generate_jk.py','generate_f10145.py',
             'generate_f10145_shared_write.py','generate_f10145_zero_write.py','generate_async256.py',
             'generate_oscillator.py','f10415_word_bank.sv','ff_scan_controller.sv']
    paths = [HERE/n for n in names]+[root/gen.SOURCE]+[root/n for n in manifest['original_sources']]+[root/p['file'] for p in manifest['immutable_proms']]
    hashes = {str(p):sha(p) for p in paths}
    for p in paths:
        (inputs/p.name).write_bytes(p.read_bytes())
    (out/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__,source_hashes=hashes,derived_cpp_sha256=sha(inputs/'test_dispy_ff_scan.cpp')),indent=2)+'\n')
    cases = [('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_chain_bit','board','scan_bits[1438:1]',"{scan_bits[1438:2],1'b0}",'board state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board state differs'),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required','(state_freeze && state_guest_held)','quarantine released'),
            ('reset_access','board',' && !state_system_reset;',';','system reset admitted access'),
            ('lost_word_hold','board','.freeze(state_hold),.guest_held(state_hold)',
             '.freeze(state_freeze),.guest_held(state_guest_held)','physical write differs'),
        ]
    results = []
    for name,target,before,after,expected in cases:
        case = out/name
        case.mkdir()
        text,copies = board,dict(clones)
        if target:
            if target == 'board':
                assert before in text
                text = text.replace(before,after,1)
            else:
                copies[target] = once(copies[target],before,after)
        for n,s in {'board.sv':text,'fixture.sv':top,'ports.h':header,**copies}.items():
            (case/n).write_text(s)
        command = ['verilator','--cc','--exe','--build','-j','4','--top-module','dispy_full_fixture','-Wno-fatal',
                   '-DDORADO_F10145_LOGIC','-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                   str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n) for n in copies],str(root/gen.SOURCE),
                   *[str(root/n) for n in manifest['original_sources']],str(inputs/'test_dispy_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
        result = subprocess.run([str(case/'obj/Vdispy_full_fixture')],cwd=root,capture_output=True,text=True,timeout=1200)
        log = result.stdout+result.stderr
        (case/'test.txt').write_text(log)
        print(name+': '+log.strip(),flush=True)
        if expected:
            assert result.returncode and expected in log,(name,log)
        else:
            assert result.returncode==0 and 'FULL DISPLAY RESTORE PASS' in log,log
        results.append(dict(case=name,returncode=result.returncode,expected_failure=expected))
    assert all(sha(Path(p))==h for p,h in hashes.items()), 'input changed during test'
    for name,data in [('manifest',manifest),('fixture-metadata',metadata),('results',dict(status='PASS',scope=__doc__,cases=results,source_hashes=hashes))]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true')
    a = p.parse_args()
    run(a.source_root.resolve(),a.output.resolve(),a.mutations)
