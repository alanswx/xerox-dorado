#!/usr/bin/env python3
"""Complete actual MCS6532 wrapper restore, including phase and routed replies."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_mcs6532 as gen
from test_riot import CONTROL
HERE=Path(__file__).resolve().parent
PORTS='''.p39(pins[37]),.p34(pins[1]),.p38(pins[19]),.p37(pins[20]),.p36(pins[18]),.p35(pins[9]),
.p7(pins[2]),.p6(pins[3]),.p5(pins[4]),.p4(pins[5]),.p3(pins[6]),.p2(pins[7]),.p40(pins[8]),
.d_in(pins[17:10]),.pa_in(pins[28:21]),.pb_in(pins[36:29]),
.p33(outputs[0]),.p32(outputs[1]),.p31(outputs[2]),.p30(outputs[3]),.p29(outputs[4]),.p28(outputs[5]),.p27(outputs[6]),.p26(outputs[7]),
.p8(outputs[8]),.p9(outputs[9]),.p10(outputs[10]),.p11(outputs[11]),.p12(outputs[12]),.p13(outputs[13]),.p14(outputs[14]),.p15(outputs[15]),
.p24(outputs[16]),.p23(outputs[17]),.p22(outputs[18]),.p21(outputs[19]),.p19(outputs[20]),.p18(outputs[21]),.p17(outputs[22]),.p16(outputs[23]),
.p25(outputs[24]),.p1(1'b0),.p20(1'b1)'''
FIXTURE='''module mcs6532_fixture(
 input host_clk,ref_clk,advance,freeze,held,request,write,
 input [7:0] index,input [17:0] data,input [37:0] pins,
 output frozen,done,error,output [17:0] result,
 output [1097:0] physical,original_state,output [25:0] outputs,original_outputs);
 cell_MCS6532_snapshot candidate(.state_host_clk(host_clk),.state_advance(advance),
 .state_freeze(freeze),.state_guest_held(held),.state_request(request),.state_write(write),
 .state_index(index),.state_data(data),.state_frozen(frozen),.state_done(done),
 .state_error(error),.state_result(result),.sys_clk(ref_clk),PORTS);
 cell_MCS6532 original(.sys_clk(ref_clk),REFPORTS);
 genvar i;generate for(i=0;i<128;i=i+1)begin:ram
  assign physical[8*i+:8]=candidate.u_riot.riot_ram[i];
  assign original_state[8*i+:8]=original.u_riot.riot_ram[i];
 end endgenerate
 assign physical[1096:1024]=CANDIDATE_CONTROL;
 assign original_state[1096:1024]=ORIGINAL_CONTROL;
 assign physical[1097]=candidate.phase_d;assign original_state[1097]=original.phase_d;
 assign outputs[25]=candidate.oe;assign original_outputs[25]=original.oe;
endmodule
'''
FIXTURE=FIXTURE.replace('REFPORTS',PORTS.replace('outputs','original_outputs')).replace('PORTS',PORTS)
FIXTURE=FIXTURE.replace('CANDIDATE_CONTROL',CONTROL.replace('CELL','candidate.u_riot'))
FIXTURE=FIXTURE.replace('ORIGINAL_CONTROL',CONTROL.replace('CELL','original.u_riot').replace('original.u_riot.old_pa7','original.u_riot.riot_stuff.old_pa7'))


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);wrapper,manifest,children=gen.generate()
 paths=[HERE/n for n in('generate_mcs6532.py','generate_riot.py','test_mcs6532.py','test_mcs6532.cpp','riot_oracle.hpp','test_riot.py')]+[gen.SOURCE,gen.riot.SOURCE]
 inputs=out/'executed-inputs';inputs.mkdir()
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 cases=[('candidate',None,None,None)]
 if mutations:cases +=[
  ('ungated_phase','!state_hold && state_advance)phase_d','!state_hold)phase_d','wrapper state oracle differs'),
  ('ignore_phase_freeze','!state_hold && state_advance)phase_d','state_advance)phase_d','wrapper state oracle differs'),
  ('lost_phase','phase_d<=state_data[0]',"phase_d<=1'b0",'wrapper state oracle differs'),
  ('phase_alias',"state_index==8'd133","state_index==8'd132",'wrapper state oracle differs'),
  ('wrong_response',"response_vendor<=state_index<8'd133","response_vendor<=state_index<8'd134",'wrapper read response differs'),
  ('invalid_index',"state_index>=8'd134","state_index>=8'd135",'wrapper request handshake differs'),
  ('wrong_phase_edge','wire phase_rise=p39 && !phase_d','wire phase_rise=!p39 && phase_d','wrapper state oracle differs'),
  ('running_access','state_hold && state_frozen && state_request','state_request','wrapper request handshake differs')]
 results=[]
 for name,before,after,why in cases:
  d=out/name;d.mkdir();text=wrapper
  if before:assert text.count(before)==1,(name,before);text=text.replace(before,after,1)
  (d/'fixture.sv').write_text(FIXTURE);(d/'cell_MCS6532_snapshot.sv').write_text(text)
  for n,s in children.items():(d/n).write_text(s)
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','mcs6532_fixture','-Wno-fatal','--Mdir',str(d/'obj'),
       str(d/'fixture.sv'),str(d/'cell_MCS6532_snapshot.sv'),str(d/'M6532_snapshot.sv'),str(gen.SOURCE),str(gen.riot.SOURCE),str(HERE/'test_mcs6532.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
  p=subprocess.run([str(d/'obj/Vmcs6532_fixture')],capture_output=True,text=True,timeout=60)
  text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert not p.returncode and 'MCS6532 PASS'in text,text
  results.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 guards=[]
 for name,before,after in [('extra_state',b'reg phase_d=0;',b'reg hidden; reg phase_d=0;'),('changed_phase',b'p39 && !phase_d',b'!p39 && phase_d')]:
  raw=gen.SOURCE.read_bytes();assert before in raw
  try:gen.generate(raw.replace(before,after,1))
  except ValueError:guards.append(dict(case=name,rejected=True))
  else:raise AssertionError('changed MCS6532 source accepted:'+name)
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 (out/'results.json').write_text(json.dumps(dict(status='PASS',manifest=manifest,cases=results,source_controls=guards,
  source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in paths}),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true')
 a=p.parse_args();run(a.output.resolve(),a.mutations)
