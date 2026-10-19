#!/usr/bin/env python3
"""Full original IFU/RAM-oracle restore test with compact serial FF access.

Keep the original ordinary operation, independent RAM model and raw probes.
Derive only state-word mapping/waits and reset/quarantine observations.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_ifu_ff_scan as gen
import generate_ifu_full as reference
import test_ifu_full as qualified
from generate_conta_ff_scan import once

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(root,manifest):
    old = reference.generate(root)[1]
    top,header,metadata = qualified.fixture(root,old)
    top = once(top,'input wire host_clk,run,run_ref,input wire',
               'input wire host_clk,run,run_ref,system_reset,input wire')
    top = once(top,'output wire advance,ref_advance);',
               'output wire advance,ref_advance,scan_hold_required,scan_fault,output wire [10:0] helper_frozen);')
    top = once(top,'.state_host_clk(host_clk)',
               '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    rows = [dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    rows += [dict(word=old['phase']['word'],width=4,offset=974)]
    header = once(header,'FF_COUNT=95','FF_COUNT=56')
    for name,values in [('FF_WIDTHS',[r['width'] for r in rows]),('FF_WORDS',[r['word'] for r in rows]),
                        ('FF_OFFSETS',[r['offset'] for r in rows])]:
        before = re.search(r'constexpr unsigned '+name+r'\[\]=\{[^}]*\};',header)
        assert before
        header = once(header,before[0],'constexpr unsigned '+name+'[]={'+','.join(map(str,values))+'};')
    header += 'constexpr unsigned SCAN_BITS=974,SCAN_WORDS=55;\n'
    probes = [f"assign helper_frozen[{r['bank']}]=candidate.snapshot_bank_{r['bank']}.frozen;"
              for r in manifest['banks']]
    top = once(top,'endmodule','\n'.join(probes)+'\nendmodule')
    return top,header,dict(metadata,ff_rows=rows,ff_offsets=[r['offset'] for r in rows])


def driver(manifest):
    original = (HERE/'test_ifu_full.cpp').read_text()
    start,end = original.index('static unsigned access('),original.index('\nstatic unsigned ram(')
    pipeline = manifest['banks'][0]['word']|1024
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
    marker = '    d.request=0;cycle(false,false);d.freeze=1;d.held=0;cycle(false,false);'
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
    check(d.helper_frozen==0x7ff,"memory helper escaped quarantine");
    auto quarantined=d.candidate_ff;unsigned quarantined_raw=d.candidate_raw;
    d.request=d.write=1;d.state_index={pipeline};d.state_data=transport_mask;
    for(unsigned i=0;i<30;++i){{pins();cycle(true,false,false,false);
        check(d.scan_fault&&d.scan_hold_required&&d.frozen,"quarantine released");
        check(d.helper_frozen==0x7ff,"memory helper escaped quarantine");
        for(unsigned word=0;word<(978+31)/32;++word)
            check(d.candidate_ff[word]==quarantined[word],"quarantined FF state changed");
        check(d.candidate_raw==quarantined_raw,"quarantined read pipeline changed");}}
    d.system_reset=1;d.freeze=d.held=1;cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
    d.system_reset=0;cycle(false,false,false,false);check(d.frozen&&!d.done,"fault reset reentry accepted request");
    d.request=0;cycle(false,false,false,false);
'''
    marker = '    std::printf("FULL IFU RESTORE PASS'
    text = once(text,marker,quarantine+marker)
    return original,text


def run(root,out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones = gen.generate(root)
    top,header,metadata = fixture(root,manifest)
    old_cpp,cpp = driver(manifest)
    inputs = out/'executed-inputs';inputs.mkdir()
    (inputs/'original-ifu-test.cpp').write_text(old_cpp)
    (inputs/'test_ifu_ff_scan.cpp').write_text(cpp)
    names = ['test_ifu_ff_scan.py','generate_ifu_ff_scan.py','test_ifu_full.py','test_ifu_full.cpp',
             'generate_ifu_full.py','audit_ifu_im.py','audit_contb_im.py','generate_conta_ff_scan.py',
             'generate_conta_full.py','generate_contb_full.py','generate_contb.py','generate_snapshot_ff.py',
             'generate_jk.py','f10415_word_bank.sv','ff_scan_controller.sv']
    paths = [HERE/n for n in names]+[gen.qualified.im.SIL,root/gen.SOURCE]+[root/n for n in manifest['original_sources']]+[root/p['file'] for p in manifest['immutable_proms']]
    hashes = {str(p):sha(p) for p in paths}
    for p in paths:
        (inputs/p.name).write_bytes(p.read_bytes())
    (out/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__,source_hashes=hashes,
        derived_cpp_sha256=sha(inputs/'test_ifu_ff_scan.cpp')),indent=2)+'\n')
    cases = [('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_chain_bit','board','scan_bits[973:1]',"{scan_bits[973:2],1'b0}",'board FF state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board FF state differs'),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required',
             '(state_freeze && state_guest_held)','memory helper escaped quarantine'),
            ('reset_access','board',' && !state_system_reset;',';','system reset admitted access'),
            ('lost_bank_hold','board','.freeze(state_hold),.guest_held(state_hold)',
             '.freeze(state_freeze),.guest_held(state_guest_held)','memory helper escaped quarantine'),
            ('lost_pipeline','f10415_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'RAM bit write/readback differs'),
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
        for n,s in {'board.sv':text,'fixture.sv':top,'ports.h':header,**copies}.items():
            (case/n).write_text(s)
        command = ['verilator','--cc','--exe','--build','-j','4','--top-module','ifu_full_fixture','-Wno-fatal',
                   '-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                   str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n) for n in copies],str(root/gen.SOURCE),
                   *[str(root/n) for n in manifest['original_sources']],str(inputs/'test_ifu_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
        result = subprocess.run([str(case/'obj/Vifu_full_fixture')],cwd=root,capture_output=True,text=True,timeout=1200)
        log = result.stdout+result.stderr;(case/'test.txt').write_text(log)
        print(name+': '+log.strip(),flush=True)
        if expected:
            assert result.returncode and expected in log,(name,log)
        else:
            assert result.returncode==0 and 'FULL IFU RESTORE PASS' in log,log
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
