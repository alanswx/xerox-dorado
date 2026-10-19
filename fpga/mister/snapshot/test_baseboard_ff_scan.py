#!/usr/bin/env python3
"""Original BaseBoard/independent SRAM+RIOT oracles with compact FF/CPU access.

Derive administrative mapping/waits and reset/quarantine observations only.
All ordinary stimulus, original physical probes and independent models remain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import shutil
import subprocess
import sys
import generate_baseboard_ff_scan as gen
import test_baseboard_full as qualified
from generate_conta_ff_scan import once

HERE=Path(__file__).resolve().parent


def fixture(root,manifest):
    old=gen.reference(root)[1]
    top,header,meta=qualified.fixture(root,old)
    top=once(top,'input wire host_clk,run,run_ref,','input wire host_clk,run,run_ref,system_reset,')
    top=once(top,'output wire advance,ref_advance);',
             'output wire advance,ref_advance,scan_hold_required,scan_fault,output wire [16:0] helper_frozen);')
    top=once(top,'.state_host_clk(host_clk)',
             '.state_system_reset(system_reset),.state_scan_hold_required(scan_hold_required),.state_scan_fault(scan_fault),.state_host_clk(host_clk)')
    old_ff_words=sum(r['words_count'] for r in old['packages'] if r['type']=='FF')
    cpu_start=40366-1725
    def offset(x):
        return x if x<279 else x+1725 if x<cpu_start else x-cpu_start+279
    # Reorder both read-only physical probe buses identically: FF279, CPU1725,
    # then all other state. The actual original and candidate RTL are untouched.
    def remap(m):
        return m[1]+str(offset(int(m[2])))+m[3]
    top=re.sub(r'(assign (?:original|candidate)_state\[)(\d+)(\+:\d+\])',remap,top)
    rows=[dict(word=r['index'],width=r['width'],offset=r['lsb']) for r in manifest['scan']['word_map']]
    rows += [dict(w,offset=offset(w['offset'])) for w in meta['state_words'][old_ff_words:-96]]
    assert sum(w['width'] for w in rows)==40366
    shift=112-old_ff_words
    ram_start=[r['start_word']+shift for r in meta['rams']]
    riot_start=[r['start_word']+shift for r in meta['riots']]
    header=once(header,'WORDS=4905',f'WORDS={len(rows)}')
    header=header[:header.index('struct StateWord')]+'''struct StateWord{unsigned word,width,offset;};
constexpr StateWord state_words[]={'''+','.join('{'+f"{w['word']},{w['width']},{w['offset']}"+'}' for w in rows)+'''};
constexpr unsigned ram_start[]={'''+','.join(map(str,ram_start))+'''};
constexpr unsigned riot_start[]={'''+','.join(map(str,riot_start))+'''};
constexpr unsigned SCAN_BITS=2004,SCAN_WORDS=112;
'''
    regular=[r for r in manifest['packages'] if r['type'] not in ('FF','CPU')]
    probes='\n'.join(f"assign helper_frozen[{i}]=candidate.state_child_frozen[{r['index']}];" for i,r in enumerate(regular))
    top=once(top,'endmodule',probes+'\nendmodule')
    return top,header,dict(meta,state_words=rows,scan_probe_order='FF279,CPU1725,SRAM32800,EPROM72,RIOT5490')


def driver():
    original=(HERE/'test_baseboard_full.cpp').read_text()
    a,b=original.index('static unsigned access('),original.index('\nint main(')
    access='''static unsigned access(unsigned index,bool write,unsigned data=0,bool invalid=false,bool compare=true){
 d.state_index=index;d.request=1;d.write=write;d.state_data=data&0x3ffff;
 pins();cycle(rnd()&1,false,false,false);++requests;unsigned waited=0;
 while(!d.done){
  d.request=d.write=1;d.state_index=30u<<11;d.state_data=0x3ffff;
  cycle(rnd()&1,false,false,false);check(++waited<=SCAN_BITS+1,"scan completion timeout");
 }
 d.request=0;
 if(index<SCAN_WORDS&&!invalid)check(waited==SCAN_BITS,"scan rotation length differs");
 check(!d.scan_hold_required&&!d.scan_fault,"completed access still held/faulted");
 check(bool(d.error)==invalid,"invalid index accepted");
 if(compare)equal();return d.result;
}
'''
    text=once(original,original[a:b],access)
    text=once(text,' Verilated::commandArgs(argc,argv);',' Verilated::commandArgs(argc,argv);d.system_reset=0;')
    marker='  // Every primitive must stop together despite a still-high advance input.'
    reset='''  d.freeze=d.held=1;d.request=0;cycle(false,false);
  d.system_reset=1;d.request=d.write=1;d.state_index=30u<<11;d.state_data=0x3ffff;
  cycle(false,false,false,false);
  check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset admitted access");equal();
  d.system_reset=0;cycle(false,false);check(d.frozen&&!d.done,"reset reentry accepted request");
  d.request=d.write=d.freeze=d.held=0;cycle(false,false);
'''
    text=once(text,marker,reset+marker)
    quarantine='''  d.request=d.write=0;cycle(false,false);d.freeze=d.held=1;cycle(false,false);
  d.request=d.write=1;d.state_index=0;d.state_data=0x12345;cycle(false,false,false,false);
  d.request=0;cycle(false,false,false,false);cycle(false,false,false,false);
  check(d.scan_hold_required,"partial scan lost hold");
  d.freeze=d.held=0;cycle(false,false,false,false);
  check(d.scan_fault&&d.scan_hold_required&&d.done&&d.error,"lost boundary not quarantined");
  check(d.helper_frozen==0x1ffff,"memory helper escaped quarantine");
  auto quarantined=image();
  d.request=d.write=1;d.state_index=30u<<11;d.state_data=0x3ffff;
  for(unsigned i=0;i<30;++i){pins();cycle(true,false,false,false);
   check(d.scan_fault&&d.scan_hold_required&&d.frozen,"quarantine released");
   check(d.helper_frozen==0x1ffff,"memory helper escaped quarantine");
   physical(quarantined);
   check(d.candidate_probe==d.original_probe,"quarantined RAM changed");
  }
  d.system_reset=1;d.freeze=d.held=1;cycle(false,false,false,false);
  check(!d.frozen&&!d.done&&!d.error&&!d.scan_hold_required&&!d.scan_fault,"system reset failed to close access");
  d.system_reset=0;cycle(false,false,false,false);check(d.frozen&&!d.done,"fault reset reentry accepted request");
  d.request=0;cycle(false,false,false,false);
'''
    text=once(text,'  std::printf("FULL BASEBOARD RESTORE PASS',quarantine+'  std::printf("FULL BASEBOARD RESTORE PASS')
    return original,text


def run(root,out,mutations,reuse=None):
    out.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(root);top,header,meta=fixture(root,manifest);old_cpp,cpp=driver()
    inputs=out/'executed-inputs';inputs.mkdir()
    (inputs/'original-baseboard-test.cpp').write_text(old_cpp)
    (inputs/'test_baseboard_ff_scan.cpp').write_text(cpp)
    (inputs/'ports.h').write_text(header)
    names=['test_baseboard_ff_scan.py','generate_baseboard_ff_scan.py','generate_baseboard_full.py',
           'test_baseboard_full.py','test_baseboard_full.cpp','riot_oracle.hpp','generate_baseboard_ff.py',
           'classify_baseboard_misc.py','generate_6502.py','generate_mcs6532.py','generate_riot.py',
           'generate_riot_shared_port.py','generate_i2716.py','ff_scan_controller.sv']
    firmware=list(gen.qualified.eprom.images(root).values())
    paths=[HERE/n for n in names]+[root/gen.SOURCE]+[root/n for n in manifest['original_sources']]+firmware
    hashes={str(p):gen.sha(p) for p in paths}
    for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
    for p in firmware:
        q=out/p.relative_to(root);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
    originals=[inputs/Path(n).name for n in manifest['original_sources']]
    originals.insert(0,inputs/Path(gen.SOURCE).name)
    cases=[('candidate',None,None,None,None)]
    if mutations:
        cases += [
            ('lost_chain_bit','board','.state_scan_in(scan_next[12])',".state_scan_in(1'b0)",'board state differs'),
            ('lost_ff_shift','board','.state_write(scan_step)',".state_write(1'b0)",'board state differs'),
            ('lost_cpu_node','chip_6502_snapshot.sv','q<={state_scan_in,q[1724:1]};',"q<={1'b0,q[1724:1]};",('board state differs','physical write differs')),
            ('early_completion','ff_scan_controller.sv','position == STATE_BITS-1','position == STATE_BITS-2','scan rotation length differs'),
            ('busy_owner','board',' && !scan_busy','','scan rotation length differs'),
            ('lost_local_hold','board','(state_freeze && state_guest_held) || state_scan_hold_required','(state_freeze && state_guest_held)','memory helper escaped quarantine'),
            ('reset_access','board','!state_system_reset',"1'b1",'system reset admitted access'),
            ('reset_reentry','board','state_boundary_ready<=state_hold && !state_system_reset;','state_boundary_ready<=state_hold;',('reset reentry accepted request','board state differs')),
            ('lost_sram_hold','board','.freeze(state_hold),.guest_held(state_hold)','.freeze(state_freeze),.guest_held(state_guest_held)','memory helper escaped quarantine'),
            ('lost_eprom_hold','board','.state_freeze(state_hold),.state_guest_held(state_hold)','.state_freeze(state_freeze),.state_guest_held(state_guest_held)','memory helper escaped quarantine'),
            ('lost_riot_phase','cell_MCS6532_snapshot.sv','phase_d<=state_data[0];',"phase_d<=1'b0;",('physical write differs','board state differs')),
            ('lost_sram_pipeline','f10415_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;",'physical write differs'),
        ]
    outcomes=[]
    for name,target,before,after,why in cases:
        case=out/name;case.mkdir();body,children=board,dict(clones)
        if target=='board':
            assert before in body
            body=body.replace(before,after) if name=='reset_access' else body.replace(before,after,1)
        elif target:
            raw=gen.raw_bytes(children[target]);assert raw.count(before.encode())==1,(name,before)
            children[target]=raw.replace(before.encode(),after.encode(),1)
        for n,s in {'board.sv':body,'fixture.sv':top,**children}.items():(case/n).write_bytes(gen.raw_bytes(s))
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','baseboard_full_fixture','-Wno-fatal',
                 '-DDORADO_F10415_M10K','--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'board.sv'),
                 *[str(case/n) for n in children],*map(str,originals),str(inputs/'test_baseboard_ff_scan.cpp'),'-CFLAGS','-I'+str(inputs)]
        binary=case/'obj/Vbaseboard_full_fixture'
        old_case=reuse/name if reuse else None
        if old_case and (old_case/'obj/Vbaseboard_full_fixture').is_file():
            # Only reuse an EXACT compiled fixture. Always execute it again,
            # capture the real exit status, and apply the current assertions.
            for n in ('board.sv','fixture.sv',*children):
                assert (old_case/n).read_bytes()==(case/n).read_bytes(),'reused HDL differs: '+n
            for n in ('ports.h','test_baseboard_ff_scan.cpp','riot_oracle.hpp',*[p.name for p in originals]):
                assert (reuse/'executed-inputs'/n).read_bytes()==(inputs/n).read_bytes(),'reused C++/original differs: '+n
            binary=old_case/'obj/Vbaseboard_full_fixture'
            shutil.copyfile(old_case/'command.json',case/'command.json')
            shutil.copyfile(old_case/'build.log',case/'build.log')
            (case/'reused-build.json').write_text(json.dumps(dict(previous_case=str(old_case),binary_sha256=gen.sha(binary),
                verified='Every original, candidate, helper, physical fixture, C++ driver, header and independent oracle is byte-identical; real binary executed again.'),indent=2)+'\n')
        else:
            (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
            with (case/'build.log').open('w') as log:subprocess.run(command,cwd=out,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(binary)],cwd=out,capture_output=True,text=True,timeout=1800)
        log=result.stdout+result.stderr;(case/'test.txt').write_text(log)
        print(name+': '+log.strip().splitlines()[-1],flush=True)
        if why:assert result.returncode and any(x in log for x in ((why,) if isinstance(why,str) else why)),(name,log)
        else:assert result.returncode==0 and 'FULL BASEBOARD RESTORE PASS' in log,log
        outcomes.append(dict(case=name,returncode=result.returncode,expected_failure=why,binary_sha256=gen.sha(binary)))
    assert all(gen.sha(Path(p))==h for p,h in hashes.items()),'input changed during test'
    for name,data in [('manifest',manifest),('fixture-metadata',meta),('results',dict(status='PASS',scope=__doc__,cases=outcomes,source_hashes=hashes))]:
        (out/(name+'.json')).write_text(json.dumps(data,indent=2)+'\n')


if __name__=='__main__':
    if resource.getrlimit(resource.RLIMIT_STACK)[0]<32*1024*1024:
        os.execv('/bin/zsh',['/bin/zsh','-c','ulimit -s 32768 || exit; exec "$@"','baseboard-scan-test',sys.executable,str(Path(__file__).resolve()),*sys.argv[1:]])
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');p.add_argument('--reuse-builds',type=Path)
    a=p.parse_args();run(a.source_root.resolve(),a.output.resolve(),a.mutations,a.reuse_builds.resolve() if a.reuse_builds else None)
