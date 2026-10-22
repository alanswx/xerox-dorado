#!/usr/bin/env python3
"""Prove a combinational MC10181 carry rewrite against the qualified cell.

Keep the exact S/X/Y/logic table and pin order. Replace the serial group-sum,
increment and carry-in adders with Boolean carry lookahead (HM3.7). Exhaust
all16384 binary inputs/all seven outputs in two simulators. No RTL register,
capture epoch, canonical cell or FPGA is changed by this component experiment.
"""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
CELL='src/verilog/cells/cell_MC10181.v'
def sha(b):return hashlib.sha256(b).hexdigest()

def lookahead(raw):
 s=raw.decode();start=s.index('  wire [4:0] gsum = ');end=s.index('  wire [3:0] F = ',start)
 s=s[:start]+'''  // HM3.7: preserve every ALUF/Cn result and the original look-ahead
  // pin polarity. Inclusive bit propagate is valid with generate=X&Y.
  // This factors X+Y+Cn directly, avoiding serial group-sum incrementers.
  wire [3:0] bit_g=X & Y,bit_p=X | Y;
  wire c1=bit_g[0] | (bit_p[0]&Cn);
  wire c2=bit_g[1] | (bit_p[1]&bit_g[0]) |
          (bit_p[1]&bit_p[0]&Cn);
  wire c3=bit_g[2] | (bit_p[2]&bit_g[1]) |
          (bit_p[2]&bit_p[1]&bit_g[0]) |
          (bit_p[2]&bit_p[1]&bit_p[0]&Cn);
  wire group_g=bit_g[3] | (bit_p[3]&bit_g[2]) |
               (bit_p[3]&bit_p[2]&bit_g[1]) |
               (bit_p[3]&bit_p[2]&bit_p[1]&bit_g[0]);
  wire group_p=&bit_p;
  wire [3:0] carry_bits={c3,c2,c1,Cn};
  wire [3:0] sum_bits=(X^Y)^carry_bits;
'''+s[end:]
 old='wire [3:0] F = M ? lg : sum[3:0];';assert s.count(old)==1;s=s.replace(old,'wire [3:0] F = M ? lg : sum_bits;')
 start=s.index('  wire [4:0] gsum1 = ');end=s.index('\nendmodule',start)
 s=s[:start]+'''  assign p5=M ? 1'b0 : group_g | (group_p&Cn);
  assign p4=group_g | group_p;  // GG: carry out with Cn=1
  assign p8=group_g;            // PG: carry out with Cn=0
'''+s[end:]
 return s.encode()

def bench():
 ports='''.p10(a[3]),.p16(a[2]),.p18(a[1]),.p21(a[0]),
 .p9(b[3]),.p11(b[2]),.p19(b[1]),.p20(b[0]),
 .p13(s[3]),.p15(s[2]),.p17(s[1]),.p14(s[0]),.p23(m),.p22(cin),
 .p6(OUT[3]),.p7(OUT[2]),.p3(OUT[1]),.p2(OUT[0]),.p5(OUT[4]),.p4(OUT[5]),.p8(OUT[6])'''
 return ('''`timescale 1ns/1ps
module tb_lookahead;
 reg [3:0] a,b,s;reg m,cin;
 wire [6:0] original,result;
 original_MC10181 reference(PORTS_ORIGINAL);
 cell_MC10181 candidate(PORTS_RESULT);
 integer checks=0;reg[4:0] arithmetic;reg[3:0] not_b;
 initial begin
  for(integer v=0;v<16384;v=v+1)begin
   {s,a,b,m,cin}=14'(v);#1;
   if(result!==original)$fatal(1,"MC10181_LOOKAHEAD differs a=%h b=%h S=%h M=%b Cn=%b got=%h original=%h",a,b,s,m,cin,result,original);
   if(!m)begin
    if(result[4]!==((result[6]|cin)&result[5]))$fatal(1,"MC10181_LOOKAHEAD group carry polarity");
    if(s==6 || s==9 || s==0)begin
     not_b=~b;
     if(s==6)arithmetic={1'b0,a}+{1'b0,b}+cin;
     else if(s==9)arithmetic={1'b0,a}+{1'b0,not_b}+cin;
     else arithmetic={1'b0,a}+cin;
     if(result[4:0]!==arithmetic)$fatal(1,"MC10181_LOOKAHEAD independent arithmetic mismatch");
    end
   end
   checks=checks+1;
  end
  $display("MC10181_LOOKAHEAD PASS all=%0d binary cases, all7 pins, HM3.7 arithmetic/polarity",checks);$finish;
 end
endmodule
'''.replace('PORTS_ORIGINAL',ports.replace('OUT','original')).replace('PORTS_RESULT',ports.replace('OUT','result'))).encode()

def run(parent,out):
 assert not out.exists();out.mkdir(parents=True);inp=out/'inputs';inp.mkdir()
 r=json.loads((parent/'sdram-burst-native.json').read_text());b=(parent/CELL).read_bytes();assert sha(b)==r['source_hashes'][CELL]
 original=b.replace(b'module cell_MC10181 (',b'module original_MC10181 (');assert original!=b
 (inp/'reference.v').write_bytes(original);(inp/'candidate.v').write_bytes(lookahead(b));(inp/'tb.sv').write_bytes(bench())
 (inp/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 proof=dict(scope=__doc__,parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),original_cell_sha256=sha(b),binary_cases=16384,outputs=7,registers_changed=False,inputs={f.name:sha(f.read_bytes())for f in inp.iterdir()},native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(proof,indent=2)+'\n');state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(cmd,label,negative=False):
  with(out/(label+'.log')).open('x')as log:
   p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);a=dict(stage=label,pid=p.pid,command=cmd,started=time.time());state['actions'].append(a);save();rc=p.wait();a.update(returncode=rc,finished=time.time());save()
  t=(out/(label+'.log')).read_text();assert(rc!=0 and 'MC10181_LOOKAHEAD differs'in t)if negative else rc==0,(label,rc,t[-2500:])
 def case(engine,label,rtl,negative=False):
  label=engine+'-'+label;files=[str(rtl),str(inp/'reference.v'),str(inp/'tb.sv')]
  if engine=='icarus':
   binary=out/(label+'.vvp');execute(['iverilog','-g2012','-s','tb_lookahead','-o',str(binary),*files],label+'-compile');execute(['vvp',str(binary)],label+'-test',negative)
  else:
   obj=out/('obj-'+label);execute(['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_lookahead','--Mdir',str(obj),'-o','alu-test',*files],label+'-compile');execute([str(obj/'alu-test')],label+'-test',negative)
  if not negative:assert 'MC10181_LOOKAHEAD PASS all=16384'in(out/(label+'-test.log')).read_text()
  print(label,'runtime rejected'if negative else'PASS',flush=True)
 save()
 try:
  candidate=inp/'candidate.v';source=candidate.read_bytes()
  for engine in ['icarus','verilator']:
   case(engine,'positive',candidate)
   for label,old,new in [('wrong-GG',b'assign p4=group_g | group_p;',b'assign p4=group_g;'),('lost-Cn',b'carry_bits={c3,c2,c1,Cn}',b"carry_bits={c3,c2,c1,1'b0}")]:
    assert source.count(old)==1;f=out/(engine+'-'+label+'.v');f.write_bytes(source.replace(old,new));case(engine,label,f,True)
  state.update(status='PASS_MC10181_LOOKAHEAD_ALL_BINARY_PINS_TWO_SIMULATORS',finished=time.time());save()
 except BaseException:state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(state['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--parent',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();run(a.parent.resolve(),a.output.resolve())
