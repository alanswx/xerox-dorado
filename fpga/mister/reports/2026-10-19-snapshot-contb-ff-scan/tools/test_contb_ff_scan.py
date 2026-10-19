#!/usr/bin/env python3
"""Original ContB/independent IM oracle with compact FF access and quarantine.

Retain original stimulus, reference, RAM and pipeline oracles. Only admin
mapping/waits, reset handling and direct fault observations are derived.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import generate_contb_ff_scan as gen
import test_contb_full as qualified
from generate_conta_ff_scan import once

HERE=Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def fixture(root,manifest):
    with gen.source_context(root):
        top,header,ports=qualified.fixture(gen.reference(root)[1])
    top=once(top,'input wire host_clk,run,run_ref,','input wire host_clk,run,run_ref,system_reset,')
    top=once(top,'output wire [208:0] original_ff,candidate_ff);',
             '''output wire [208:0] original_ff,candidate_ff,
 input wire [9:0] probe_row,output wire [143:0] original_probe,candidate_probe,
 output wire scan_hold_required,scan_fault,output wire [7:0] helper_frozen);''')
    top=once(top,'.state_host_clk(host_clk)',
             '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    header=once(header,'FF_COUNT=22','FF_COUNT=12')
    for name,values in [('FF_WIDTHS',[r['width'] for r in manifest['scan']['word_map']]),
                        ('FF_OFFSETS',[r['lsb'] for r in manifest['scan']['word_map']])]:
        found=re.search(r'constexpr unsigned '+name+r'\[\]=\{[^}]*\};',header)
        assert found
        header=once(header,found[0],'constexpr unsigned '+name+'[]={'+','.join(map(str,values))+'};')
    header+='constexpr unsigned SCAN_BITS=209,SCAN_WORDS=12;\n'
    probes=[]
    for group in manifest['groups']:
        i=group['index']
        probes.append(f'assign helper_frozen[{i}]=candidate.snapshot_bank_{i}.frozen;')
        for bit in group['bits']:
            b=bit['bit'];offset=18*i+b
            probes.append(f"assign original_probe[{offset}]=original.u_{bit['package']}.mem[probe_row];")
            probes.append(f'assign candidate_probe[{offset}]=candidate.snapshot_bank_{i}.bits[{b}].mem[probe_row];')
    top=once(top,'endmodule','\n'.join(probes)+'\nendmodule')
    return top,header,ports


def driver():
    original=(HERE/'test_contb_full.cpp').read_text()
    start,end=original.index('static uint32_t access('),original.index('\nstatic uint32_t ram_access(')
    access='''static uint32_t access(unsigned index,bool write,uint32_t data,
                       bool error=false,bool compare=true) {
    d.state_index=index;d.write=write;d.state_data=data;d.request=1;
    cycle(false,false,false,false);++requests;unsigned waited=0;
    while(!d.done){
        d.request=1;d.write=1;d.state_index=1024;d.state_data=mask;
        cycle(false,false,false,false);check(++waited<=SCAN_BITS+1,"scan completion timeout");
    }
    d.request=0;
    if((index>>11)==8&&(index&2047)<SCAN_WORDS&&!error)
        check(waited==SCAN_BITS,"scan rotation length differs");
    check(!d.scan_hold_required&&!d.scan_fault,"completed access still held/faulted");
    check(bool(d.error)==error,"invalid state address accepted or valid rejected");
    if(compare)equal();
    return d.result;
}
'''
    text=once(original,original[start:end],access)
    text=once(text,'    return access((8<<11)|cell,write,data,false,compare);',
              '''    if(write)data|=mask^((1u<<FF_WIDTHS[cell])-1);
    return access((8<<11)|cell,write,data,false,compare);''')
    text=once(text,'    Verilated::commandArgs(argc,argv);','    Verilated::commandArgs(argc,argv);d.system_reset=0;')
    marker='    d.request=0;cycle(false,false);\n    d.freeze=1;d.held=0;cycle(false,false);'
    reset='''    d.request=0;cycle(false,false);d.freeze=d.held=1;cycle(false,false);
    d.system_reset=1;d.request=d.write=1;d.state_index=1024;d.state_data=mask;
    cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset admitted access");equal();
    d.system_reset=0;cycle(false,false,false,false);
    check(d.frozen&&!d.done,"reset reentry accepted request");equal();
    d.request=d.write=d.freeze=d.held=0;cycle(false,false);
'''
    text=once(text,marker,reset+marker)
    quarantine='''    d.request=d.write=0;cycle(false,false);
    d.freeze=d.held=1;cycle(false,false);
    d.request=d.write=1;d.state_index=8<<11;d.state_data=0x12345;cycle(false,false,false,false);
    d.request=0;cycle(false,false,false,false);cycle(false,false,false,false);
    check(d.scan_hold_required,"partial scan lost hold");
    d.freeze=d.held=0;cycle(false,false,false,false);
    check(d.scan_fault&&d.scan_hold_required&&d.done&&d.error,"lost boundary not quarantined");
    check(d.helper_frozen==0xff,"memory helper escaped quarantine");
    auto quarantined=d.candidate_ff;auto raw=d.candidate_raw;
    d.request=d.write=1;d.state_index=1024;d.state_data=mask;
    for(unsigned i=0;i<30;++i){random_inputs();d.probe_row=random_word()&1023;cycle(true,false,false,false);
        check(d.scan_fault&&d.scan_hold_required&&d.frozen,"quarantine released");
        check(d.helper_frozen==0xff,"memory helper escaped quarantine");
        for(unsigned word=0;word<7;++word)check(d.candidate_ff[word]==quarantined[word],"quarantined FF state changed");
        for(unsigned word=0;word<5;++word){
            check(d.candidate_raw[word]==raw[word],"quarantined read pipeline changed");
            check(d.candidate_probe[word]==d.original_probe[word],"quarantined RAM changed");}}
    d.system_reset=1;d.freeze=d.held=1;cycle(false,false,false,false);
    check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
    d.system_reset=0;cycle(false,false,false,false);check(d.frozen&&!d.done,"fault reset reentry accepted request");
    d.request=0;cycle(false,false,false,false);
'''
    text=once(text,'    std::printf("FULL CONTB RESTORE PASS:',quarantine+'    std::printf("FULL CONTB RESTORE PASS:')
    return original,text


def run(root,out,mutations):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(root)
    top,header,ports=fixture(root,manifest)
    old_cpp,cpp=driver()
    inputs=out/'executed-inputs';inputs.mkdir()
    (inputs/'original-contb-test.cpp').write_text(old_cpp)
    (inputs/'test_contb_ff_scan.cpp').write_text(cpp)
    names=['test_contb_ff_scan.py','generate_contb_ff_scan.py','test_contb_full.py','test_contb_full.cpp',
           'test_contb_snapshot.py','generate_contb_full.py','generate_contb.py','audit_contb_im.py',
           'generate_snapshot_ff.py','generate_conta_ff_scan.py','generate_conta_full.py',
           'f10415_enable_bank.sv','ff_scan_controller.sv']
    wirelist=root/gen.WIRELIST if (root/gen.WIRELIST).exists() else gen.ROOT/gen.WIRELIST
    paths=[HERE/n for n in names]+[root/gen.SOURCE,wirelist,wirelist.with_suffix('.lc')]+[root/n for n in manifest['grouping_sources']]
    hashes={str(p):sha(p) for p in paths}
    for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
    (out/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__,source_hashes=hashes,
        derived_cpp_sha256=sha(inputs/'test_contb_ff_scan.cpp')),indent=2)+'\n')
    cases=[('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_chain_bit','board','scan_bits[208:1]',"{scan_bits[208:2],1'b0}",'board FF state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board FF state differs'),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required',
             '(state_freeze && state_guest_held)','memory helper escaped quarantine'),
            ('reset_access','board','!state_system_reset',"1'b1",'system reset admitted access'),
            ('reset_reentry','board','state_boundary_ready<=state_hold && !state_system_reset;',
             'state_boundary_ready<=state_hold;','reset reentry accepted request'),
            ('lost_bank_hold','board','.freeze(state_hold), .guest_held(state_hold)',
             '.freeze(state_freeze), .guest_held(state_guest_held)','memory helper escaped quarantine'),
            ('lost_pipeline','f10415_enable_bank.sv','saved_q <= state_data;',"saved_q <= 18'b0;",
             ('destructive RAM write failed','board raw pipeline differs')),
        ]
    results=[]
    for name,target,before,after,expected in cases:
        case=out/name;case.mkdir();text,copies=board,dict(clones)
        if target=='board':
            assert before in text
            text=text.replace(before,after) if name=='reset_access' else text.replace(before,after,1)
        elif target:
            copies[target]=once(copies[target],before,after)
        for n,s in {'board.sv':text,'fixture.sv':top,'ports.h':header,**copies}.items():(case/n).write_text(s)
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','contb_full_fixture','-Wno-fatal',
                 '-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),'-CFLAGS','-I'+str(case),
                 str(case/'fixture.sv'),str(case/'board.sv'),*[str(case/n) for n in copies],str(root/gen.SOURCE),
                 *[str(root/n) for n in manifest['grouping_sources']],str(inputs/'test_contb_ff_scan.cpp')]
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        with (case/'build.log').open('w') as log:subprocess.run(command,cwd=root,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vcontb_full_fixture')],cwd=root,capture_output=True,text=True,timeout=1200)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log)
        print(name+': '+log.strip().splitlines()[-1],flush=True)
        if expected:
            diagnostics=(expected,) if isinstance(expected,str) else expected
            assert result.returncode and any(message in log for message in diagnostics),(name,log)
        else:assert result.returncode==0 and 'FULL CONTB RESTORE PASS' in log,log
        results.append(dict(case=name,returncode=result.returncode,expected_failure=expected))
    assert all(sha(Path(p))==h for p,h in hashes.items()), 'input changed during test'
    for name,data in [('manifest',manifest),('ports',ports),('results',dict(status='PASS',scope=__doc__,cases=results,source_hashes=hashes))]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.source_root.resolve(),a.output.resolve(),a.mutations)
