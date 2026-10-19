#!/usr/bin/env python3
"""Actual MOS RIOT original/independent oracle and all1097bit restoration."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import generate_riot as gen

HERE=Path(__file__).resolve().parent
CONTROL='{CELL.old_pa7,CELL.rollover,CELL.edge_detect,CELL.irq_en,CELL.incr,CELL.prescaler,CELL.timer,CELL.interrupt,CELL.dir_b,CELL.dir_a,CELL.out_b,CELL.out_a,CELL.d_out}'
FIXTURE='''module riot_fixture(
 input host_clk,ref_clk,advance,freeze,held,request,write,
 input [7:0] index,input [17:0] data,input [36:0] pins,
 output frozen,done,error,output [17:0] result,
 output [1096:0] physical,original_state,
 output [25:0] outputs,original_outputs);
 M6532_snapshot #(.init_7800(0)) candidate(
 .state_host_clk(host_clk),.state_advance(advance),.state_freeze(freeze),.state_guest_held(held),
 .state_request(request),.state_write(write),.state_index(index),.state_data(data),
 .state_frozen(frozen),.state_done(done),.state_error(error),.state_result(result),
 .clk(ref_clk),PORTS);
 M6532 #(.init_7800(0)) original(.clk(ref_clk),REFPORTS);
 genvar i;generate for(i=0;i<128;i=i+1)begin:ram
  assign physical[8*i+:8]=candidate.riot_ram[i];
  assign original_state[8*i+:8]=original.riot_ram[i];
 end endgenerate
 assign physical[1096:1024]=CANDIDATE_CONTROL;
 assign original_state[1096:1024]=ORIGINAL_CONTROL;
 endmodule
'''
PORTS='.ce(pins[0]),.res_n(pins[1]),.addr(pins[8:2]),.RW_n(pins[9]),.d_in(pins[17:10]),.RS_n(pins[18]),.CS1(pins[19]),.CS2_n(pins[20]),.PA_in(pins[28:21]),.PB_in(pins[36:29]),.d_out(outputs[7:0]),.PA_out(outputs[15:8]),.PB_out(outputs[23:16]),.IRQ_n(outputs[24]),.oe(outputs[25])'
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)
FIXTURE=FIXTURE.replace('CANDIDATE_CONTROL',CONTROL.replace('CELL','candidate'))
FIXTURE=FIXTURE.replace('ORIGINAL_CONTROL',CONTROL.replace('CELL','original').replace('original.old_pa7','original.riot_stuff.old_pa7'))


def run(out,mutations):
    out.mkdir(parents=True,exist_ok=False);source,m=gen.generate()
    cases=[('candidate',None,None,None)]
    if mutations:cases +=[
        ('ungated','!state_hold && state_advance','!state_hold','RIOT state oracle differs'),
        ('ignore_freeze','!state_hold && state_advance','state_advance','RIOT state oracle differs'),
        ('running_access','state_hold && state_frozen && state_request','state_request','request handshake differs'),
        ('invalid_alias',"state_index<8'd133","state_index<8'd134",'request handshake differs'),
        ('lost_pa7','old_pa7<=state_data[0:0]',"old_pa7<=1'b0",'RIOT state oracle differs'),
        ('ram_alias','riot_ram[state_index[6:0]]<=state_data[7:0]',"riot_ram[0]<=state_data[7:0]",'RIOT state oracle differs'),
        ('read_enable','if (ce && (CS1 & ~CS2_n) && RW_n)','if ((CS1 & ~CS2_n) && RW_n)','RIOT state oracle differs'),
        ('timer_enable','if (ce) begin : riot_stuff',"if (1'b1) begin : riot_stuff",'RIOT state oracle differs'),
        ('reset_priority','if (~res_n) begin','if (res_n) begin','RIOT state oracle differs')]
    results=[]
    for name,before,after,why in cases:
        d=out/name;d.mkdir();text=source
        if before:assert text.count(before)==1,(name,before);text=text.replace(before,after,1)
        (d/'fixture.sv').write_text(FIXTURE);(d/'M6532_snapshot.sv').write_text(text)
        cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','riot_fixture','-Wno-fatal',
             '--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'M6532_snapshot.sv'),str(gen.SOURCE),str(HERE/'test_riot.cpp')]
        (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
        with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
        p=subprocess.run([str(d/'obj/Vriot_fixture')],capture_output=True,text=True,timeout=60)
        text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
        if why:assert p.returncode and why in text,(name,text)
        else:assert not p.returncode and 'RIOT PASS'in text,text
        results.append(dict(case=name,returncode=p.returncode,expected_failure=why))
    guards=[]
    for name,before,after in [('extra_state',b'reg edge_detect;',b'reg edge_detect; reg hidden;'),
                              ('changed_timer',b'timer - 8\'d1',b'timer + 8\'d1')]:
        raw=gen.SOURCE.read_bytes();assert before in raw
        try:gen.generate(raw.replace(before,after,1))
        except ValueError:guards.append(dict(case=name,rejected=True))
        else:raise AssertionError('changed RIOT original accepted:'+name)
    paths=[HERE/n for n in('generate_riot.py','test_riot.py','test_riot.cpp')]+[gen.SOURCE]
    for p in paths[:-1]:(out/p.name).write_bytes(p.read_bytes())
    (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps(dict(status='PASS',manifest=m,cases=results,source_controls=guards,
        source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
