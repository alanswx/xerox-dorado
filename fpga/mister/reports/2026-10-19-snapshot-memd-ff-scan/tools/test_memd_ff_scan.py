#!/usr/bin/env python3
"""Full original MemD/RAM-oracle restore test with compact serial FF access.

Keep the original ordinary operation, independent RAM model and raw probes.
Derive only state-word mapping/waits and reset/quarantine observations.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_memd_ff_scan as gen
import generate_memd_full as reference
import test_memd_full as qualified
from generate_conta_ff_scan import once

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(root,manifest):
    old = reference.generate(root)[1]
    top,header,metadata = qualified.fixture(root,old)
    top = once(top,'input wire host_clk,run,run_ref,input wire',
               'input wire host_clk,run,run_ref,system_reset,input wire')
    top = once(top,'output wire [17:0] storage_data,output wire storage_clock);',
               'output wire [17:0] storage_data,output wire storage_clock,scan_hold_required,scan_fault,output wire [21:0] helper_frozen);')
    top = once(top,'.state_host_clk(host_clk)',
               '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    rows = [dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    rows += metadata['state_words'][92:]
    assert sum(r['width'] for r in rows)==1607
    header = once(header,f"WORDS={len(metadata['state_words'])}",f"WORDS={len(rows)}")
    start,end=header.index('struct StateWord'),header.index('constexpr unsigned BANK_WIDTHS')
    header = header[:start]+'struct StateWord{unsigned word,width,offset;};\nconstexpr StateWord state_words[]={'+','.join(
        '{'+f"{r['word']},{r['width']},{r['offset']}"+'}' for r in rows)+'};\n'+header[end:]
    header += 'constexpr unsigned SCAN_BITS=968,SCAN_WORDS=54;\n'
    probes = [f"assign helper_frozen[{8+r['bank']}]=candidate.snapshot_bank_{r['bank']}.frozen;"
              for r in manifest['banks']]
    probes += [f"assign helper_frozen[{r['memory_index']}]=candidate.{r['package']}.state_frozen;" for r in manifest['files']]
    top = once(top,'endmodule','\n'.join(probes)+'\nendmodule')
    return top,header,dict(metadata,state_words=rows)


def driver(manifest):
    original = (HERE/'test_memd_full.cpp').read_text()
    start,end = original.index('static unsigned access('),original.index('\nstatic unsigned ram(')
    pipeline = manifest['banks'][0]['word']|4096
    access = f'''static unsigned access(unsigned index,bool write,unsigned data=0,bool error=false,bool compare=true){{
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;
    cycle(false,false,false,false);++requests;unsigned waited=0;
    while(!d.done){{
        d.request=1;d.write=1;d.state_index={pipeline};d.state_data=transport_mask;
        cycle(false,false,false,false);check(++waited<=SCAN_BITS+1,"scan completion timeout");
    }}
    d.request=0;
    if(index<SCAN_WORDS&&!error)check(waited==SCAN_BITS,"scan rotation length differs");
    check(!d.scan_hold_required&&!d.scan_fault,"completed access still held/faulted");
    check(bool(d.error)==error,"invalid state index accepted or valid rejected");
    if(compare)equal();
    return d.result;
}}
'''
    text = once(original,original[start:end],access)
    text = once(text,'    Verilated::commandArgs(argc,argv);',
                '    Verilated::commandArgs(argc,argv);d.system_reset=0;')
    marker = '    d.request=0;cycle(false,false);d.freeze=1;d.held=0;d.request=1;d.write=1;'
    reset = f'''    d.request=0;cycle(false,false);d.freeze=d.held=1;cycle(false,false);
    d.system_reset=1;d.request=d.write=1;d.state_index={pipeline};d.state_data=transport_mask;
    cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset admitted access");equal();
    d.system_reset=0;cycle(false,false);check(d.frozen&&!d.done,"reset reentry accepted request");
    d.request=d.write=d.freeze=d.held=0;cycle(false,false);
'''
    text = once(text,marker,reset+marker)
    quarantine = f'''    d.request=d.write=0;cycle(false,false);
    d.freeze=d.held=1;cycle(false,false);
    d.request=d.write=1;d.state_index=0;d.state_data=0x12345;cycle(false,false,false,false);
    d.request=0;cycle(false,false,false,false);cycle(false,false,false,false);
    check(d.scan_hold_required,"partial scan lost hold");
    d.freeze=d.held=0;cycle(false,false,false,false);
    check(d.scan_fault&&d.scan_hold_required&&d.done&&d.error,"lost boundary not quarantined");
    check(d.helper_frozen==0x3fffff,"memory helper escaped quarantine");
    auto quarantined=d.candidate_ff;auto quarantined_raw=d.candidate_raw;
    d.request=d.write=1;d.state_index={pipeline};d.state_data=transport_mask;
    for(unsigned i=0;i<30;++i){{pins();cycle(true,false,false,false);
        check(d.scan_fault&&d.scan_hold_required&&d.frozen,"quarantine released");
        check(d.helper_frozen==0x3fffff,"memory helper escaped quarantine");
        for(unsigned word=0;word<(1607+31)/32;++word)
            check(d.candidate_ff[word]==quarantined[word],"quarantined FF state changed");
        for(unsigned word=0;word<3;++word)check(d.candidate_raw[word]==quarantined_raw[word],"quarantined read pipeline changed");}}
    d.system_reset=1;d.freeze=d.held=1;cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
    d.system_reset=0;cycle(false,false,false,false);check(d.frozen&&!d.done,"fault reset reentry accepted request");
    d.request=0;cycle(false,false,false,false);
'''
    marker = '    std::printf("FULL MemD RESTORE PASS'
    text = once(text,marker,quarantine+marker)
    return original,text


def run(root,out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones = gen.generate(root)
    top,header,metadata = fixture(root,manifest)
    old_cpp,cpp = driver(manifest)
    inputs = out/'executed-inputs';inputs.mkdir()
    (inputs/'original-memd-test.cpp').write_text(old_cpp)
    (inputs/'test_memd_ff_scan.cpp').write_text(cpp)
    names = ['test_memd_ff_scan.py','generate_memd_ff_scan.py','test_memd_full.py','test_memd_full.cpp',
             'generate_memd_full.py','audit_memd_im.py','audit_ifu_im.py','audit_contb_im.py','generate_conta_ff_scan.py',
             'generate_conta_full.py','generate_contb_full.py','generate_contb.py','generate_snapshot_ff.py',
             'generate_processor_ff.py','generate_f10145.py','generate_f10145_shared_write.py',
             'generate_f10145_zero_write.py','f10470_word_bank.sv','ff_scan_controller.sv']
    paths = [HERE/n for n in names]+[gen.qualified.im.SIL,gen.qualified.im.SIL.with_suffix('.lc'),root/gen.SOURCE]+[root/n for n in manifest['original_sources']]
    hashes = {str(p):sha(p) for p in paths}
    for p in paths:
        (inputs/p.name).write_bytes(p.read_bytes())
    (out/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__,source_hashes=hashes,
        derived_cpp_sha256=sha(inputs/'test_memd_ff_scan.cpp')),indent=2)+'\n')
    cases = [('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_chain_bit','board','scan_bits[967:1]',"{scan_bits[967:2],1'b0}",'board auxiliary state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board auxiliary state differs'),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required',
             '(state_freeze && state_guest_held)','memory helper escaped quarantine'),
            ('reset_access','board',' && !state_system_reset;',';','system reset admitted access'),
            ('lost_bank_hold','board','.freeze(state_hold),.guest_held(state_hold)',
             '.freeze(state_freeze),.guest_held(state_guest_held)','memory helper escaped quarantine'),
            ('lost_storage_word','board','words[state_row[0]]<=state_data;',"words[state_row[0]]<=18'b0;",'aux bit write/readback differs'),
            ('lost_storage_history','board','clock_d<=state_data[0];',"clock_d<=1'b0;",
             ('aux bit write/readback differs','board auxiliary state differs')),
            ('unsupported_storage_delay',None,None,None,'MemD snapshot requires STORAGE_INPUT_DELAY=2'),
            ('lost_pipeline','f10470_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'RAM bit write/readback differs'),
        ]
    results = []
    for name,target,before,after,expected in cases:
        case = out/name;case.mkdir()
        text,copies = board,dict(clones)
        if target=='board':
            assert before in text
            text = text.replace(before,after,1)
        elif target:
            copies[target] = once(copies[target],before,after)
        case_top=top
        if name=='unsupported_storage_delay':
            case_top=once(top,'#(.SYSPER(4),.STORAGE_INPUT_DELAY(2)) candidate','#(.SYSPER(4),.STORAGE_INPUT_DELAY(1)) candidate')
        for n,s in {'board.sv':text,'fixture.sv':case_top,'ports.h':header,**copies}.items():
            (case/n).write_text(s)
        command = ['verilator','--cc','--exe','--build','-j','4','--top-module','memd_full_fixture','-Wno-fatal',
                   '-DDORADO_F10145_LOGIC','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                   str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n) for n in copies],str(root/gen.SOURCE),
                   *[str(root/n) for n in manifest['original_sources']],str(inputs/'test_memd_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
        result = subprocess.run([str(case/'obj/Vmemd_full_fixture')],cwd=root,capture_output=True,text=True,timeout=1200)
        log = result.stdout+result.stderr;(case/'test.txt').write_text(log)
        print(name+': '+log.strip().splitlines()[-1],flush=True)
        if expected:
            diagnostics=(expected,) if isinstance(expected,str) else expected
            assert result.returncode and any(message in log for message in diagnostics),(name,log)
        else:
            assert result.returncode==0 and 'FULL MemD RESTORE PASS' in log,log
        results.append(dict(case=name,returncode=result.returncode,expected_failure=expected))
    assert all(sha(Path(p))==h for p,h in hashes.items()), 'input changed during test'
    for name,data in [('manifest',manifest),('fixture-metadata',metadata),
                      ('results',dict(status='PASS',scope=__doc__,cases=results,source_hashes=hashes))]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true')
    a = p.parse_args();run(a.source_root.resolve(),a.output.resolve(),a.mutations)
