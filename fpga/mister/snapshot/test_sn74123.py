#!/usr/bin/env python3
"""Original and independent signed timer oracle; full70bit save/restore."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_sn74123 as gen
HERE=Path(__file__).resolve().parent
MODES=[dict(sysper=1,tw_ns=0),dict(sysper=1,tw_ns=30)]+[dict(sysper=s,tw_ns=3000)for s in (2,4,8,16)]
FIXTURE='''module sn74123_fixture(
input wire host_clk,ref_clk,advance,freeze,held,request,write,individual,
input wire [5:0] pins,input wire [2:0] index,input wire [17:0] data,restore_data[6],
output wire [17:0] result[6],output wire [69:0] physical[6],original_state[6],
output wire [5:0] frozen,done,error,output wire [11:0] outputs,original_outputs);
genvar i;generate for(i=0;i<6;i=i+1)begin:cells
localparam integer SP=i<2?1:i==2?2:i==3?4:i==4?8:16;
localparam integer NS=i==0?0:i==1?30:3000;
cell_SN74123_snapshot #(.SYSPER(SP),.TW_NS(NS)) candidate(.state_host_clk(host_clk),.state_advance(advance),
.state_freeze(freeze),.state_guest_held(held),.state_request(request),.state_write(write),
.state_index(index),.state_data(individual?restore_data[i]:data),.state_frozen(frozen[i]),.state_done(done[i]),.state_error(error[i]),.state_result(result[i]),.sys_clk(ref_clk),PORTS);
cell_SN74123 #(.SYSPER(SP),.TW_NS(NS)) original(.sys_clk(ref_clk),REFPORTS);
assign physical[i]={candidate.c2_d,candidate.b2_d,candidate.a2_d,candidate.c1_d,candidate.b1_d,candidate.a1_d,candidate.t2,candidate.t1};
assign original_state[i]={original.c2_d,original.b2_d,original.a2_d,original.c1_d,original.b1_d,original.a1_d,original.t2,original.t1};
end endgenerate endmodule
'''
PORTS=".p1(pins[0]),.p2(pins[1]),.p3(pins[2]),.p9(pins[3]),.p10(pins[4]),.p11(pins[5]),.p6(1'b0),.p7(1'b0),.p8(1'b0),.p14(1'b0),.p15(1'b0),.p16(1'b1),.p13(outputs[2*i]),.p5(outputs[2*i+1])"
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);source,m=gen.generate()
 for n in ('generate_sn74123.py','test_sn74123.py','test_sn74123.cpp'):(out/n).write_bytes((HERE/n).read_bytes())
 cases=[('candidate',None,None,None)]
 if mutations:cases +=[
  ('ungated','!state_hold && state_advance','!state_hold','state oracle differs'),
  ('ignore_freeze','!state_hold && state_advance','state_advance','state oracle differs'),
  ('running_access','state_hold && state_frozen && state_request','state_request','request handshake differs'),
  ('invalid_alias','!state_index[2]',"1'b1",'request handshake differs'),
  ('lost_timer_sign','0:t1[17:0]<=state_data','0:t1<=state_data','state oracle differs'),
  ('lost_history','state_data[15:0]',"{1'b0,state_data[14:0]}",'state oracle differs'),
  ('unsigned_timer','else if (t1 > 0)','else if ($unsigned(t1) > 0)','state oracle differs'),
  ('wrong_clear','if (!p3)','if (p3)','state oracle differs'),
  ('no_retrigger','else if (trig1)','else if (trig1 && t1 == 0)','state oracle differs')]
 rows=[]
 for name,before,after,why in cases:
  d=out/name;d.mkdir();text=source
  if before:assert before in text,(name,before);text=text.replace(before,after,1)
  (d/'fixture.sv').write_text(FIXTURE);(d/'cell_SN74123_snapshot.sv').write_text(text)
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','sn74123_fixture','-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),str(d/'cell_SN74123_snapshot.sv'),str(gen.ROOT/'verilog/cells/cell_SN74123.v'),str(HERE/'test_sn74123.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
  p=subprocess.run([str(d/'obj/Vsn74123_fixture')],capture_output=True,text=True,timeout=60);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert p.returncode==0 and'SN74123 PASS'in text,text
  rows.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 controls=[];original=(gen.ROOT/'verilog/cells/cell_SN74123.v').read_text()
 for name,text in [('extra_state',original.replace('endmodule','reg hidden;\nendmodule')),('changed_priority',original.replace('if (!p3)','if (p3)'))]:
  try:gen.generate(text)
  except ValueError:controls.append(dict(case=name,rejected=True))
  else:raise AssertionError('unreviewed timer original accepted')
 paths=[HERE/n for n in ('generate_sn74123.py','test_sn74123.py','test_sn74123.cpp','generate_snapshot_ff.py')]+[gen.ROOT/'verilog/cells/cell_SN74123.v']
 (out/'results.json').write_text(json.dumps(dict(status='PASS',manifest=m,parameter_modes=MODES,cases=rows,source_controls=controls,source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
