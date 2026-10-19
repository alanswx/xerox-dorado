#!/usr/bin/env python3
"""Original RTL and independent state oracles for four DskEth cell families."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_dsketh_ff as gen
HERE=Path(__file__).resolve().parent
KINDS=list(gen.FIELDS)
PORTS=[
 ".p4(pins[0]),.p6(pins[1]),.p5(pins[2]),.p13(pins[3]),.p12(pins[4]),.p11(pins[5]),.p1(1'b0),.p7(1'b0),.p8(1'b0),.p10(1'b0),.p16(1'b1),.p2(outputs[0][0]),.p14(outputs[0][1])",
 ".p13(pins[0]),.p5(pins[1]),.p6(pins[2]),.p11(pins[3]),.p12(pins[4]),.p9(pins[5]),.p7(pins[6]),.p10(pins[7]),.p3(outputs[1][3]),.p2(outputs[1][2]),.p15(outputs[1][1]),.p14(outputs[1][0]),.p4(outputs[1][4])",
 ".p2(pins[0]),.p1(pins[1]),.p9(pins[2]),.p7(pins[3]),.p10(pins[4]),.p6(pins[5]),.p5(pins[6]),.p4(pins[7]),.p3(pins[8]),.p8(1'b0),.p16(1'b1),.p15(outputs[2][0])",
 ".p9(pins[0]),.p1(pins[1]),.p3(pins[2]),.p4(pins[3]),.p6(pins[4]),.p11(pins[5]),.p13(pins[6]),.p14(pins[7]),.p8(1'b0),.p16(1'b1),.p2(outputs[I][0]),.p5(outputs[I][1]),.p7(outputs[I][2]),.p10(outputs[I][3]),.p12(outputs[I][4]),.p15(outputs[I][5])"]

def fixture():
 s='''module dsketh_ff_fixture(input wire host_clk,ref_clk,advance,hold,write,individual,
 input wire [11:0] pins,input wire [17:0] data,restore_data[5],
 output wire [17:0] result[5],physical[5],original_state[5],
 output wire [5:0] outputs[5],original_outputs[5]);\n'''
 for i in range(5):
  kind=KINDS[min(i,3)];width=sum(w for _,w in gen.FIELDS[kind]);fields=','.join('cell_i.'+n for n,_ in reversed(gen.FIELDS[kind]))
  original=fields.replace('cell_i.','ref_i.');param=' #(.PRECAP('+str(i-3)+'))'if i>=3 else''
  ports=PORTS[min(i,3)].replace('[I]','['+str(i)+']')
  s+='begin:case'+str(i)+'\n'
  s+=kind+'_snapshot'+param+' cell_i(.state_host_clk(host_clk),.state_advance(advance),.state_hold(hold),.state_write(write),.state_data(individual?restore_data['+str(i)+']:data),.state_result(result['+str(i)+']),.sys_clk(ref_clk),'+ports+');\n'
  s+=kind+param+' ref_i(.sys_clk(ref_clk),'+ports.replace('outputs','original_outputs')+');\n'
  s+='assign physical['+str(i)+']={'+str(18-width)+"'b0,{"+fields+'}};\n'
  s+='assign original_state['+str(i)+']={'+str(18-width)+"'b0,{"+original+'}};\nend\n'
  driven={0:2,1:5,2:1,3:6,4:6}[i]
  if driven<6:s+='assign outputs['+str(i)+'][5:'+str(driven)+']=0;assign original_outputs['+str(i)+'][5:'+str(driven)+']=0;\n'
 return s+'endmodule\n'

def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);sources=gen.generate()
 for n in ('generate_dsketh_ff.py','test_dsketh_ff.py','test_dsketh_ff.cpp'):(out/n).write_bytes((HERE/n).read_bytes())
 cases=[('candidate',None,None,None,None)]
 if mutations:cases +=[
  ('ungated',KINDS[0],'else if(state_advance)','else if(1\'b1)','state oracle differs'),
  ('ignore_hold',KINDS[1],'if(state_hold)begin','if(1\'b0)begin','state oracle differs'),
  ('lost_history',KINDS[2],'{ck_d,p10_d,p9_d,p7_d,q}<=state_data[7:0]','{ck_d,p10_d,p9_d,p7_d,q}<={1\'b0,state_data[6:0]}','state oracle differs'),
  ('wrong_polarity',KINDS[3],'if (!p1)','if (p1)','state oracle differs'),
  ('edge_value',KINDS[1],'if (!su_pre && !sd_pre)','if (!p9 && !p7)','state oracle differs'),
  ('wrong_direction',KINDS[2],"q - 4'd1","q + 4'd1",'state oracle differs'),
  ('wrong_precap',KINDS[3],'PRECAP ? d_pre : {p14,p13,p11,p6,p4,p3}','{p14,p13,p11,p6,p4,p3}','state oracle differs'),
  ('bad_equal',KINDS[0],"else if (~p6 & p5) qa <= 1'b0;","else qa <= 1'b0;",'state oracle differs')]
 rows=[]
 for name,kind,before,after,why in cases:
  d=out/name;d.mkdir();paths=[]
  for k,(text,m)in sources.items():
   if k==kind:
    assert before in text,(name,before);text=text.replace(before,after,1)
   p=d/(k+'_snapshot.sv');p.write_text(text);paths.append(p)
  (d/'fixture.sv').write_text(fixture())
  originals=[gen.ROOT/'verilog/cells'/(k+'.v')for k in KINDS]
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','dsketh_ff_fixture','-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),*map(str,paths),*map(str,originals),str(HERE/'test_dsketh_ff.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
  p=subprocess.run([str(d/'obj/Vdsketh_ff_fixture')],capture_output=True,text=True,timeout=60);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert p.returncode==0 and'DSKETH_FF PASS'in text,text
  rows.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 controls=[]
 for kind in KINDS:
  original=(gen.ROOT/'verilog/cells'/(kind+'.v')).read_text()
  try:gen.generate_one(kind,original.replace('endmodule','reg hidden;\nendmodule'))
  except ValueError:controls.append(dict(case=kind+'_extra_state',rejected=True))
  else:raise AssertionError('unreviewed original accepted')
 (out/'results.json').write_text(json.dumps(dict(status='PASS',manifests={k:m for k,(_,m)in sources.items()},cases=rows,source_controls=controls),indent=2)+'\n')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
