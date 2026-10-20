#!/usr/bin/env python3
"""Pause the actual Pilot serial/HPS medium, preserving all original state.

Independent serial Fire-code and whole-image payload oracles are the existing
Pilot pack fixture. Drain accepted HPS transfers; retain partial serial blocks,
pending stores and dirty buffer state. This is not Main integration or restore.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from generate_serial_boards import ROOT,raw_receipt,once
from classify_serial_boards import read_inventory
import inventory_disk_cache_phase as geometry

BLOCK='''  // Complete already-issued HPS work before leasing the ARM image. Partial
  // serial blocks, pending guest-port stores and dirty buffer generations
  // remain machine state; never finish a guest write or flush partial data.
  reg snapshot_paused = 0;
  reg snapshot_attached = 0;
  wire snapshot_media_known = (attached===1'b1) ||
      ((attached===1'b0) && (valid===1'b0) && (dirty===1'b0));
  wire snapshot_quiet = (snapshot_guest_held===1'b1) &&
      (reset===1'b0) && snapshot_media_known && (remounted===1'b0) &&
      (fault===1'b0) && (host_state===3'd0) &&
      (sd_rd===1'b0) && (sd_wr===1'b0) && (sd_ack===1'b0) && (sd_buff_wr===1'b0);
  wire snapshot_enter = (SNAPSHOT_PAUSE!=0) && (snapshot_hold===1'b1) && snapshot_quiet;
  assign snapshot_block = (SNAPSHOT_PAUSE!=0) && (snapshot_paused || snapshot_enter);
  assign snapshot_held = (SNAPSHOT_PAUSE!=0) && snapshot_paused;
  assign snapshot_idle = snapshot_held && (snapshot_hold===1'b1) && snapshot_quiet &&
      (attached===snapshot_attached);
  always @(posedge clk_sys) begin
    if (reset) begin snapshot_paused <= 0; snapshot_attached <= 0; end
    else if (snapshot_paused) begin
      if ((snapshot_hold===1'b0) && (sd_ack===1'b0) && (sd_buff_wr===1'b0)) snapshot_paused <= 0;
    end else if (snapshot_enter) begin snapshot_paused <= 1; snapshot_attached <= attached; end
  end
'''


def sha(raw):return hashlib.sha256(raw).hexdigest()


def inverse(text):
    text=once(text,'    parameter integer SNAPSHOT_PAUSE = 0,\n','')
    text=once(text,'    input wire snapshot_hold, snapshot_guest_held,\n    output wire snapshot_idle, snapshot_held,\n','')
    text=once(text,BLOCK,'')
    text=once(text,'  wire snapshot_block;\n','')
    text=once(text,'''    if (!snapshot_block) begin
    sd_buff_din <= buffer[sd_buff_addr];
    if (sd_buff_wr) buffer[sd_buff_addr] <= sd_buff_dout;
    end''','''    sd_buff_din <= buffer[sd_buff_addr];
    if (sd_buff_wr) buffer[sd_buff_addr] <= sd_buff_dout;''')
    return once(text,'end else if (!snapshot_block) begin\n      if (host_state == 0)',
        'end else begin\n      if (host_state == 0)')


def run(out,image):
    out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
    source=(ROOT/'fpga/mister/dorado_pack_hps.sv').read_bytes()
    original=raw_receipt(ROOT/'fpga/mister/reports/2026-10-20-snapshot-native-memory-pause-rejected','terminal-fit/src/fpga/mister/dorado_pack_hps.sv')
    assert inverse(source.decode()).encode()==original,'unapproved normal medium edit'
    (inputs/'medium.sv').write_bytes(source);(inputs/'original.sv').write_bytes(original)
    for name in('tb_pilot_pack.sv','pilot_pack_test.cpp','hps_disk_test.h'):
        (inputs/name).write_bytes((ROOT/'verilog/verilator'/name).read_bytes())
    wanted=geometry.expected(ROOT/'fpga/mister/reports');observed=[];descriptions={}
    (inputs/'expected-native-fields.json').write_text(json.dumps(wanted,indent=2)+'\n')
    for label,body,extra in(('original',original,[]),('candidate',source,['-GSNAPSHOT_PAUSE=1'])):
        path=inputs/('original.sv'if label=='original'else'medium.sv')
        command=['verilator','--xml-only','--top-module','dorado_pack_hps','-Wno-fatal','-GLABEL_WORDS=10','-GHOST_CLOCK_HZ=50000000',*extra,'--xml-output',str(out/(label+'.xml')),str(path)]
        (out/(label+'-command.json')).write_text(json.dumps(command,indent=2)+'\n')
        with(out/(label+'-xml.log')).open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
        data,_=read_inventory((out/(label+'.xml')).read_bytes());have=geometry.geometry(data);expected=dict(wanted)
        if label=='candidate':expected.update({'dorado_pack_hps.snapshot_paused':(1,1,[]),'dorado_pack_hps.snapshot_attached':(1,1,[])})
        assert have==expected,'original disk geometry differs'
        descriptions[label]=dict(fields=len(have),bits=sum(v[1]for v in have.values()))
        if label=='candidate':
            for name,(width,bits,dims)in have.items():
                field=name.removeprefix('dorado_pack_hps.')
                if dims:
                    assert field=='buffer'and(width,bits,dims)==(16,131072,[[0,8191]])
                    observed.extend('u_medium.buffer['+str(i)+']'for i in range(8192))
                else:observed.append('u_medium.'+field)
    bits=descriptions['candidate']['bits'];assert bits==131818
    top=(inputs/'tb_pilot_pack.sv').read_text()
    top=once(top,' input wire clk_host,sys_clk,reset,attached,data_adv,writing,write_bit,',
        f''' input wire snapshot_hold,output wire snapshot_idle,snapshot_held,
 output wire [{bits-1}:0] observed_state,
 output wire [1:0] write_state,write_block,
 output wire [5:0] write_bits,output wire [8:0] write_count,
 output wire pending_store,
 input wire clk_host,sys_clk,reset,attached,data_adv,writing,write_bit,''')
    top=once(top,'#(.INITIAL_CYL(3),.LABEL_WORDS(10))',
        '#(.SNAPSHOT_PAUSE(PAUSE_MODE),.INITIAL_CYL(3),.LABEL_WORDS(10))')
    top=once(top,'.clk_sys(clk_host),.machine_clk(sys_clk),',
        '.clk_sys(clk_host),.machine_clk(sys_clk),\n  .snapshot_hold(snapshot_hold),.snapshot_guest_held(snapshot_hold && !sys_clk),\n  .snapshot_idle(snapshot_idle),.snapshot_held(snapshot_held),')
    top=once(top,'endmodule','''assign write_state=u_medium.wr_state;assign write_block=u_medium.wr_block;
assign write_bits=u_medium.wr_bits;assign write_count=u_medium.wr_count;
assign pending_store=u_medium.store;
// Test-only nonzero contents cover every unused track-buffer word too.
initial for(integer i=0;i<8192;i++)u_medium.buffer[i]=16'((i*16'h9e37)^16'ha36c);
assign observed_state={'''+',\n'.join(observed)+'''};
endmodule''')
    legacy=(inputs/'pilot_pack_test.cpp').read_text()
    cpp=once(legacy,'int main(int argc,char**argv){','int payload_main(int argc,char**argv){')
    cpp=once(cpp,'#include <cstdlib>','#include <cstdlib>\n#include <array>\n#include <functional>\n#include "disk_pause_test.inc"')
    cpp=once(cpp,'top.clk_host=top.sys_clk=0;top.reset=1;',
        'top.snapshot_hold=0;top.clk_host=top.sys_clk=0;top.reset=1;')
    cpp=once(cpp,' auto tick=[&](){',' unsigned pause_mask=0;std::function<void()> take_snapshot;\n auto tick=[&](){')
    cpp=once(cpp,'bool run=top.pack_ready&&!top.reset;',
        'bool run=top.pack_ready&&!top.reset&&!top.snapshot_hold&&!top.snapshot_held;')
    cpp=once(cpp,'  return advance;', '''#ifdef DISK_PAUSE_TEST
  unsigned mark=0;
  if(!(pause_mask&256)&&host==5&&!top.attached)mark=256;
  else if(!(pause_mask&1)&&disk.active&&!disk.writing&&disk.index==37)mark=1;
  else if(!(pause_mask&2)&&!top.writing&&steps>=500&&!expect_fault)mark=2;
  else if(!expect_fault&&top.writing&&top.write_state==1&&top.write_bits==7){
   if(top.write_block==0)mark=4;
   if(top.write_block==1&&top.write_count==5)mark=8;
   if(top.write_block==2&&top.write_count==120)mark=16;
  }
  else if(!(pause_mask&32)&&!expect_fault&&top.writing&&top.write_block==2&&top.pending_store)mark=32;
  else if(!(pause_mask&64)&&disk.active&&disk.writing&&disk.index==37)mark=64;
  else if(!(pause_mask&128)&&expect_fault&&top.writing&&top.write_state==2&&top.write_bits==8)mark=128;
  if(mark&&!(pause_mask&mark)){pause_mask|=mark;printf("DISK PAUSE POINT mask=%u\\n",mark);take_snapshot();}
#endif
  return advance;''')
    cpp=once(cpp,' for(;host<200000 && record<3;){',' DISK_DEFINE_PAUSE\n for(;host<200000 && record<3;){')
    cpp=once(cpp,' top.final();','''#ifdef DISK_PAUSE_TEST
 if(pause_mask!=511)throw std::runtime_error("disk snapshot coverage incomplete mask="+std::to_string(pause_mask));
 printf("DISK SNAPSHOT PASS holds=9 observed_bits=%u all_track_RAM=1 pending_store=1 partial_CRC=1 inactive_disk=1 restored=0\\n",DISK_STATE_BITS);
#endif
 top.final();return 0;''')
    cpp+='''\nint main(int argc,char **argv){try{return payload_main(argc,argv);}catch(const std::exception &e){fprintf(stderr,"DISK SNAPSHOT FAIL: %s\\n",e.what());return 23;}}\n'''
    (inputs/'test.cpp').write_text(cpp)
    for path in(Path(__file__),Path(__file__).with_suffix('.inc'),ROOT/'verilog/verilator/hps_disk_test.h'):
        (inputs/('disk_pause_test.inc'if path.suffix=='.inc'else path.name)).write_bytes(path.read_bytes())
    cases=[('candidate',1,source.decode(),None),('default_off',0,source.decode(),None),
        ('early_hps_freeze',1,once(source.decode(),"wire snapshot_enter = (SNAPSHOT_PAUSE!=0) && (snapshot_hold===1'b1) && snapshot_quiet;","wire snapshot_enter = (SNAPSHOT_PAUSE!=0) && (snapshot_hold===1'b1) && (snapshot_guest_held===1'b1);"),'disk pause failed to drain HPS'),
        ('unheld_ram_port',1,once(source.decode(),'    if (!snapshot_block) begin\n    sd_buff_din','    if (1\'b1) begin\n    sd_buff_din'),'held disk state changed'),
        ('unheld_timer',1,once(source.decode(),'    else if (snapshot_paused) begin','    else if (snapshot_paused) begin\n      host_wait <= host_wait+1\'b1;'),'held disk state changed')]
    results=[]
    for label,mode,body,diagnostic in cases:
        case=out/label;case.mkdir();(case/'fixture.sv').write_text(top.replace('PAUSE_MODE',str(mode)));(case/'medium.sv').write_text(body)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','tb_pilot_pack','-Wno-fatal','--Mdir',str(case/'obj'),str(case/'fixture.sv'),str(case/'medium.sv'),str(inputs/'test.cpp'),
            '-CFLAGS',f'-O2 -std=c++17 -I{inputs} -DDISK_STATE_BITS={bits} -DDISK_STATE_WORDS={(bits+31)//32}'+(' -DDISK_PAUSE_TEST=1'if mode else'')]
        (case/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(case/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
        result=subprocess.run([str(case/'obj/Vtb_pilot_pack'),str(image)],capture_output=True,text=True,timeout=60)
        text=result.stdout+result.stderr;(case/'test.log').write_text(text)
        if diagnostic:assert result.returncode==23 and diagnostic in text,text
        else:assert not result.returncode and'PILOT-PACK PASS'in text and'PACK-FAULT PASS'in text,text
        if label=='candidate':assert'DISK SNAPSHOT PASS'in text,text
        print(label+': '+text.strip(),flush=True)
        results.append(dict(case=label,returncode=result.returncode,expected_failure=diagnostic,log_sha256=sha((case/'test.log').read_bytes())))
    # Retain the exact independently checked input medium, never alter it.
    (inputs/'pilot.pack').write_bytes(image.read_bytes())
    result=dict(status='PASS',scope=__doc__,cases=results,geometry=descriptions,new_required_wrapper_bits=2,sequential_exclusions=[],
        observed_expressions=observed,inputs={p.name:sha(p.read_bytes())for p in inputs.iterdir()},
        limits='Actual serial/HPS medium only; no Main disk lease/coordinator, native integration, timing, state export/import or complete machine restore.')
    (out/'qualification.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--image',type=Path,default=ROOT/'build/rtl-executive/pilot-pack-fixture.pack');a=p.parse_args();run(a.output.resolve(),a.image.resolve())
