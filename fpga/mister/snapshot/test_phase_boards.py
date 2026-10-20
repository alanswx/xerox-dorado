#!/usr/bin/env python3
"""Complete original-board oracles for optional phase-qualified FF payloads.

All original board pins and classified physical state are compared. The
producer is an explicit same-falling-edge gate with synchronous hold inputs;
native integration/source-domain proof and RAM-family optimization remain open.
"""
import argparse
import json
import os
from pathlib import Path
import re
import resource
import subprocess
import sys
import generate_phase_payload as payload
import test_serial_boards as base

HERE=Path(__file__).resolve().parent


def adapt(generated,record):
    generated=generated.copy();families=[]
    board=record['board'];body=generated[board+'__board.sv'].decode()
    for kind in payload.KINDS:
        name=board+'__cell_'+kind+'_snapshot.v'
        if name not in generated:continue
        old=generated[name].decode();new=payload.transform(old)
        assert payload.inverse(new,old)==old
        generated[name]=new.encode()
        module=re.search(r'\bmodule\s+(\w+)',base.gen.masked_comments(new))[1]
        # Existing PRECAP bindings remain byte-identical; opt in only this
        # candidate board's physical helper instances.
        pattern=r'\b'+module+r'(\s*#\(\s*\.PRECAP\([^)]*\))\s*\)'
        body,count=re.subn(pattern,lambda m:module+m[1]+',.PHASE_QUALIFIED(1))',body)
        body,defaults=re.subn(r'\b'+module+r'(\s+u_\w+\s*\()',
            lambda m:module+' #(.PHASE_QUALIFIED(1))'+m[1],body)
        count+=defaults
        if not count:raise ValueError('unreviewed board helper binding: '+module)
        families.append(dict(kind=kind,instances=count,original_sha256=base.gen.sha(old.encode()),
            generated_sha256=base.gen.sha(new.encode()),default_off_inverse=True))
    assert families,board
    generated[board+'__board.sv']=body.encode()
    return generated,families


def fixture(record,original):
    top,header,metadata=base.fixture(record,original)
    end='output wire [17:0] reference_result,dut_result);\n'
    top=base.gen.once(top,end,end+'''wire all_hold=global_hold || (freeze && held) ||
 reference_hold || dut_hold || reference_fault || dut_fault;
 reg original_step=0;
 always @(negedge clk)original_step<=advance && !all_hold;
 wire [18:0] board_steps;
 dorado_board_phase_enable copies(.clk(clk),.permit(advance && !all_hold),.advance(board_steps));
 // All nineteen copies must remain identical, including scan and reset edges.
 always @(posedge clk)begin
  if(board_steps!=={19{original_step}})$fatal(1,"board phase copies differ");
 end
 ''')
    # Connections in both whole-board instances, not equations inside helpers.
    assert top.count('.state_guest_advance(advance)')==2
    top=top.replace('.state_guest_advance(advance)', '.state_guest_advance(original_step)',1)
    top=base.gen.once(top,'.state_guest_advance(advance)', '.state_guest_advance(board_steps[0])')
    top=base.gen.once(top,'.state_freeze(freeze || global_hold)', '.state_freeze(freeze || all_hold)')
    top=base.gen.once(top,'.state_guest_held(held || global_hold)', '.state_guest_held(held || all_hold)')
    top=base.gen.once(top,'.state_global_hold(global_hold)', '.state_global_hold(all_hold)')
    bits=original['manifest']['scan']['state_bits']
    header+=f'constexpr unsigned FF_BITS={bits},FF_WORDS={(bits+17)//18};\n'
    return top,header,metadata


def driver():
    source=base.DRIVER.replace('#include <string>','#include <string>\n#include <vector>')
    source=base.gen.once(source,'int main(int argc,char**argv){', '''static uint32_t access(unsigned word,bool writing,uint32_t value){
 d.index=word;d.write=writing;d.data=value;d.request=1;tick(true);d.request=0;
 for(unsigned age=0;!d.dut_done;++age){check(age<FF_BITS+8,"whole-board scan deadline");tick(true);}
 check(d.reference_done&&!d.dut_error&&!d.reference_error,"whole-board scan completion differs");
 check(d.dut_result==d.reference_result,"whole-board scan result differs");return d.dut_result;
}
static void restore_check(){
 auto before=d.dut_state;std::vector<uint32_t> saved;
 for(unsigned word=0;word<FF_WORDS;++word)saved.push_back(access(SCAN_WORD+word,false,0));
 check(!std::memcmp(&before,&d.dut_state,sizeof(before)),"whole-board scan read changed physical state");
 for(unsigned word=0;word<FF_WORDS;++word)access(SCAN_WORD+word,true,saved[word]^0x3ffff);
 for(unsigned word=0;word<FF_WORDS;++word)access(SCAN_WORD+word,true,saved[word]);
 check(!std::memcmp(&before,&d.dut_state,sizeof(before)),"whole-board restore changed physical state");
 d.freeze=d.held=d.global_hold=0;d.request=0;
 for(unsigned n=0;n<2000;++n){pins();d.advance=(rnd()&3)!=0;d.probe_index=rnd();tick(true);}
 d.global_hold=1;d.advance=1;d.freeze=d.held=1;for(unsigned n=0;n<4;++n)tick(true);
}
int main(int argc,char**argv){''')
    source=base.gen.once(source,'d.request=0;d.freeze=d.held=1;tick();tick();d.index=SCAN_WORD;',
        'd.request=0;d.freeze=d.held=1;tick();tick();restore_check();d.index=SCAN_WORD;')
    return base.gen.once(source,'GLOBAL BOARD PASS host=', 'PHASE WHOLE BOARD PASS host=')


def run(reports,out,boards):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out.mkdir(parents=True,exist_ok=False);results=[]
    for name in boards:
        case=out/name;case.mkdir();generated,record,original=base.gen.generate(reports,name)
        generated,families=adapt(generated,record);source=case/'inputs';source.mkdir()
        for n,raw in original['sources'].items():
            p=source/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
        for n,raw in generated.items():(source/n).write_bytes(raw)
        top,header,metadata=fixture(record,original)
        for n,text in [('fixture.sv',top),('parameters.h',header),('driver.cpp',driver()),
                       ('board_phase_enable.sv',(HERE/'board_phase_enable.sv').read_text())]:
            (source/n).write_text(text)
        paths=[source/n for n in original['sources'] if n.endswith(('.v','.sv'))and n!='top.sv']+[source/n for n in generated]
        cmd=['verilator','--cc','--exe','--build','--assert','-j','4','--top-module','global_fixture','-Wno-fatal',
             *original['simulation_defines'],'--Mdir',str(case/'obj'),str(source/'fixture.sv'),str(source/'board_phase_enable.sv'),
             *map(str,paths),str(source/'driver.cpp'),'-CFLAGS','-I'+str(source)]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        (case/'metadata.json').write_text(json.dumps(dict(**metadata,qualified_families=families),indent=2)+'\n')
        hashes={str(p.relative_to(source)):base.gen.sha(p.read_bytes())for p in source.rglob('*')if p.is_file()}
        with(case/'build.log').open('w')as log:subprocess.run(cmd,cwd=source,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vglobal_fixture')],cwd=source,capture_output=True,text=True,timeout=240)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        assert result.returncode==0 and 'PHASE WHOLE BOARD PASS'in text,(name,text)
        assert all(base.gen.sha((source/n).read_bytes())==h for n,h in hashes.items())
        results.append(dict(board=name,source_hashes=hashes,families=families,metadata=metadata,returncode=result.returncode))
    (out/'results.json').write_text(json.dumps(dict(status='PASS_BOARD_COMPONENTS',scope=__doc__,boards=results,
        limits='Eleven independent board components; native hold source-domain proof, native integration, MSA/SDRAM and routed timing remain required.'),indent=2)+'\n')


if __name__=='__main__':
    if resource.getrlimit(resource.RLIMIT_STACK)[0]<32*1024*1024:
        os.execv('/bin/zsh',['/bin/zsh','-c','ulimit -s 32768 || exit; exec "$@"','phase-board-test',sys.executable,str(Path(__file__).resolve()),*sys.argv[1:]])
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--reports',type=Path,default=base.gen.ROOT/'fpga/mister/reports')
    p.add_argument('--boards',nargs='+',default=[s[0]for s in base.gen.fmt.SPECS]);a=p.parse_args()
    run(a.reports.resolve(),a.output.resolve(),a.boards)
