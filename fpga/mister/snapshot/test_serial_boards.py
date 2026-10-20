#!/usr/bin/env python3
"""Actual eleven-board equivalence/forced-hold/abort with qualified physical probes.

Large memories use direct sampled probes plus every helper's frozen flag;
their unchanged exhaustive primitive/board qualifications remain prerequisites.
No native wrapper/ARM barrier, core capacity or hardware snapshot is provided.
"""
import argparse
import json
import os
from pathlib import Path
import re
import resource
import subprocess
import sys
import generate_serial_boards as gen


def unescape(path):
    return re.sub(r'__0([0-9a-fA-F]{2})',lambda m:chr(int(m[1],16)),path)


def fixture(record,original):
    ports=[p for p in original['ports'] if p['name']!='sys_clk']
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width'] for p in inputs);no=sum(p['width'] for p in outputs)
    physical={r['path'] for r in original['classification']['decisions'] if r['scope']=='PHYSICAL'}
    small=[];large=[];frozen=[]
    prefix=original['manifest']['module']+'.'
    for row in original['inventory']['candidates']:
        path=unescape(row['path'].removeprefix(prefix))
        dims=row['unpacked_dimensions']
        if row['path'] in physical:
            if len(dims)>1:raise ValueError('unreviewed multidimensional physical field')
            depth=dims[0][1]-dims[0][0]+1 if dims else 1
            (small if depth<=128 else large).append(dict(path=path,width=row['width'],bits=row['bits'],dims=dims))
        if row['category']=='sequential' and row['width']==1 and not dims and path.endswith('frozen') and not path.startswith('u_ff_scan.'):
            frozen.append(path)
    bits=sum(r['bits'] for r in small);probe_bits=sum(r['width'] for r in large)
    assert bits>0 and frozen
    code=f'''module global_fixture(
 input wire clk,advance,freeze,held,system_reset,global_hold,global_abort,request,write,
 input wire [{ni-1}:0] stimulus,input wire [23:0] index,input wire [17:0] data,
 input wire [15:0] probe_index,
 output wire [{no-1}:0] reference_outputs,dut_outputs,
 output wire [{bits-1}:0] reference_state,dut_state,
 output wire [{max(1,probe_bits)-1}:0] reference_probe,dut_probe,
 output wire all_helpers_frozen,
 output wire reference_frozen,reference_done,reference_error,reference_hold,reference_fault,
 output wire dut_frozen,dut_done,dut_error,dut_hold,dut_fault,
 output wire [17:0] reference_result,dut_result);
'''
    driven={p['name'] for p in outputs}
    for copy,module in [('reference',original['manifest']['module']),('dut',record['module'])]:
        for p in outputs:code+='wire '+('' if p['width']==1 else f"[{p['width']-1}:0] ")+copy+'_'+p['name']+';\n'
        conn=['.sys_clk(clk)'];offset=0
        for p in inputs:
            value=f'stimulus[{offset}]' if p['width']==1 else f"stimulus[{offset}+:{p['width']}]"
            if p['name']+'__drv' in driven:value+=' | '+copy+'_'+p['name']+'__drv'
            conn.append('.'+p['name']+'('+value+')');offset+=p['width']
        conn +=['.'+p['name']+'('+copy+'_'+p['name']+')' for p in outputs]
        conn +=['.state_host_clk(clk)','.state_guest_advance(advance)',
            '.state_freeze('+('freeze || global_hold' if copy=='reference' else 'freeze')+')',
            '.state_guest_held('+('held || global_hold' if copy=='reference' else 'held')+')',
            '.state_system_reset(system_reset)',
            '.state_request('+('request && freeze && held && !global_abort' if copy=='reference' else 'request')+')',
            '.state_write(write)',f".state_index(index[{original['manifest'].get('index_bits',next(s[1] for s in gen.fmt.SPECS if s[0]==record['board']))-1}:0])",
            '.state_data(data)',*['.state_'+p+'('+copy+'_'+p+')' for p in ('frozen','done','error','result')]]
        alias='' if record['board']=='ContA' and copy=='reference' else 'state_'
        conn +=['.'+alias+'scan_hold_required('+copy+'_hold)','.'+alias+'scan_fault('+copy+'_fault)']
        if copy=='dut':conn +=['.state_global_hold(global_hold)','.state_global_abort(global_abort)']
        code+=module+' '+original['parameters']+' '+copy+'(\n'+',\n'.join(conn)+'\n);\n'
        code+='assign '+copy+'_outputs={'+','.join(copy+'_'+p['name'] for p in reversed(outputs))+'};\n'
        offset=0
        for row in small:
            dims=row['dims']
            value=copy+'.'+row['path'] if not dims else '{'+','.join(copy+'.'+row['path']+f'[{i}]' for i in range(dims[0][1],dims[0][0]-1,-1))+'}'
            code+=f"assign {copy}_state[{offset}+:{row['bits']}]={value};\n";offset+=row['bits']
        offset=0
        for row in large:
            lo,hi=row['dims'][0];depth=hi-lo+1
            code+=f"assign {copy}_probe[{offset}+:{row['width']}]={copy}.{row['path']}[{lo}+(probe_index%{depth})];\n";offset+=row['width']
        if not large:code+='assign '+copy+"_probe=1'b0;\n"
    code+='assign all_helpers_frozen=&{'+','.join('dut.'+p for p in frozen)+'};\nendmodule\n'
    scan=original['manifest']['scan'];base=scan.get('base_word',0)
    catalog=gen.fmt.build_catalog(gen.ROOT/'fpga/mister/reports')
    descriptor=next(b for b in catalog['boards'] if b['name']==record['board'])
    auxiliary=next(r['word'] for r in descriptor['runs'] if r['offset']>=scan['state_bits'])
    header=f'constexpr unsigned STIM_BITS={ni},SCAN_WORD={base},AUX_WORD={auxiliary};\n'
    return code,header,dict(inputs=ni,outputs=no,sampled_physical_bits=bits,large_ram_probe_bits=probe_bits,
        large_ram_fields=len(large),frozen_flags=len(frozen),small=small,large=large,helper_frozen=frozen)


DRIVER=r'''#include "Vglobal_fixture.h"
#include "verilated.h"
#include "parameters.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
static Vglobal_fixture d;
static unsigned edges=0,checks=0;static uint32_t seed=0x19b04d77;
static uint32_t rnd(){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void check(bool condition,const char*why){if(!condition)throw std::runtime_error(std::string(why)+" at edge "+std::to_string(edges));++checks;}
static void pins(){
 auto*p=reinterpret_cast<unsigned char*>(&d.stimulus);
 for(unsigned i=0;i<sizeof(d.stimulus);++i)p[i]=i<(STIM_BITS+7)/8?rnd():0;
 if(STIM_BITS%8)p[STIM_BITS/8]&=(1u<<(STIM_BITS%8))-1;
}
static void equal(){
 check(!std::memcmp(&d.reference_outputs,&d.dut_outputs,sizeof(d.dut_outputs)),"ordinary outputs differ");
 check(!std::memcmp(&d.reference_state,&d.dut_state,sizeof(d.dut_state)),"physical state differs");
 check(!std::memcmp(&d.reference_probe,&d.dut_probe,sizeof(d.dut_probe)),"RAM probe differs");
}
static void tick(bool compare=false){d.clk=0;d.eval();d.clk=1;d.eval();++edges;if(compare)equal();}
int main(int argc,char**argv){
 Verilated::commandArgs(argc,argv);
 try{
  d.eval();equal();d.system_reset=1;for(unsigned i=0;i<3;++i)tick(true);d.system_reset=0;
  for(unsigned i=0;i<20000;++i){pins();d.advance=(rnd()&3)!=0;d.request=rnd()&1;d.write=rnd()&1;d.index=rnd();d.data=rnd()&0x3ffff;d.probe_index=rnd();tick(true);check(!d.dut_done,"running access admitted");}
  d.request=0;d.freeze=d.held=0;d.global_hold=1;d.advance=1;
  for(unsigned i=0;i<4;++i)tick();equal();
  check(d.dut_frozen&&d.all_helpers_frozen,"forced hold did not freeze every helper");
  check(!d.dut_hold&&!d.dut_fault,"idle hold relied on local quarantine");
  auto saved=d.dut_state;
  // Reference is held through its already-qualified boundary. DUT raw
  // freeze/held remain ZERO, so only the new global input can protect it.
  for(unsigned row=0;row<32;++row){
   d.probe_index=rnd();d.eval();auto memory=d.dut_probe;
   for(unsigned i=0;i<4;++i){pins();d.request=d.write=1;d.index=rnd();d.data=rnd()&0x3ffff;tick();
    check(d.dut_frozen&&d.all_helpers_frozen,"forced hold helper escaped");
    check(!std::memcmp(&saved,&d.dut_state,sizeof(saved)),"forced hold changed physical state");
    check(!std::memcmp(&memory,&d.dut_probe,sizeof(memory)),"forced hold changed RAM");
    check(!d.dut_done,"forced hold admitted unowned request");equal();
   }
  }
  // Both boards execute the same actual partial scan; only DUT then aborts.
  d.request=0;d.freeze=d.held=1;tick();tick();d.index=SCAN_WORD;d.data=0x315ca;d.write=d.request=1;tick();
  check(d.dut_hold,"physical scan not active");d.request=0;
  for(unsigned i=0;i<5;++i)tick(true);
  d.global_abort=1;tick();
  check(d.dut_fault&&d.dut_hold&&d.dut_done&&d.dut_error,"global abort did not quarantine scan");
  auto quarantined=d.dut_state;auto memory=d.dut_probe;
  for(unsigned i=0;i<32;++i){pins();d.request=d.write=1;d.index=AUX_WORD;d.data=0x3ffff;tick();
   check(d.dut_frozen&&d.all_helpers_frozen,"abort released memory helper");
   check(!std::memcmp(&quarantined,&d.dut_state,sizeof(quarantined)),"aborted physical state changed");
   check(!std::memcmp(&memory,&d.dut_probe,sizeof(memory)),"aborted RAM changed");
  }
  // After SYSTEM reset clears local scan quarantine, a still-asserted global
  // abort must also block non-FF requests; local fault is no longer the gate.
  d.request=0;d.system_reset=1;tick();d.system_reset=0;tick();tick();
  check(!d.dut_fault&&!d.dut_hold&&d.dut_frozen,"local reset/boundary differs");
  d.request=d.write=1;d.index=AUX_WORD;d.data=0x3ffff;tick();
  check(!d.dut_done&&!d.dut_error,"global abort admitted auxiliary access");
  check(d.all_helpers_frozen,"global abort dropped forced hold");
  std::printf("GLOBAL BOARD PASS host=%u checks=%u ordinary=20000 forced_rows=32 abort=32\n",edges,checks);return 0;
 }catch(const std::exception&e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
'''


def run(reports,out,boards):
    out.mkdir(parents=True,exist_ok=False);results=[]
    for name in boards:
        case=out/name;case.mkdir();generated,record,original=gen.generate(reports,name)
        source=case/'inputs';source.mkdir()
        for n,raw in original['sources'].items():
            p=source/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
        for n,raw in generated.items():(source/n).write_bytes(raw)
        top,header,metadata=fixture(record,original)
        (source/'fixture.sv').write_text(top);(source/'parameters.h').write_text(header);(source/'driver.cpp').write_text(DRIVER)
        paths=[source/n for n in original['sources'] if n.endswith(('.v','.sv')) and n!='top.sv']+[source/n for n in generated]
        command=['verilator','--cc','--exe','--build','-j','4','--top-module','global_fixture','-Wno-fatal',
                 *original['simulation_defines'],'--Mdir',str(case/'obj'),str(source/'fixture.sv'),*map(str,paths),
                 str(source/'driver.cpp'),'-CFLAGS','-I'+str(source)]
        hashes={str(p.relative_to(source)):gen.sha(p.read_bytes()) for p in source.rglob('*') if p.is_file()}
        (case/'command.json').write_text(json.dumps(command,indent=2)+'\n')
        (case/'metadata.json').write_text(json.dumps(metadata,indent=2)+'\n');(case/'assets.json').write_text(json.dumps(record,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(command,cwd=source,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vglobal_fixture')],cwd=source,capture_output=True,text=True,timeout=180)
        text=result.stdout+result.stderr;(case/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        assert result.returncode==0 and 'GLOBAL BOARD PASS' in text,(name,text)
        assert all(gen.sha((source/n).read_bytes())==h for n,h in hashes.items()),'compiled source changed'
        results.append(dict(board=name,returncode=result.returncode,metadata=metadata,source_hashes=hashes))
    (out/'results.json').write_text(json.dumps(dict(status='PASS',scope=__doc__,boards=results),indent=2)+'\n')


if __name__=='__main__':
    if resource.getrlimit(resource.RLIMIT_STACK)[0]<32*1024*1024:
        os.execv('/bin/zsh',['/bin/zsh','-c','ulimit -s 32768 || exit; exec "$@"','serial-board-global-test',sys.executable,str(Path(__file__).resolve()),*sys.argv[1:]])
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--reports',type=Path,default=gen.ROOT/'fpga/mister/reports')
    p.add_argument('--output',type=Path,required=True);p.add_argument('--boards',nargs='+',default=[s[0] for s in gen.fmt.SPECS])
    a=p.parse_args();run(a.reports.resolve(),a.output.resolve(),a.boards)
