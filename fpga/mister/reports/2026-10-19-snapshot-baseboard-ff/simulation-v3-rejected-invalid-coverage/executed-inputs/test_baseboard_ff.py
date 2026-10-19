#!/usr/bin/env python3
"""Nine actual BaseBoard primitive families against original RTL and state oracles."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
import generate_baseboard_ff as gen
HERE=Path(__file__).resolve().parent
KINDS=list(gen.FIELDS)


def ports(kind,i,pins='pins',outputs='outputs'):
 text=gen.ff.masked_comments(gen.original_source(kind).decode())
 names=re.findall(r'\boutput\s+wire\s+(\w+)',text)
 inputs=re.findall(r'\binput\s+wire\s+(p\d+)\b',text)
 pairs=[f'.{p}({pins}[{i}][{int(p[1:])-1}])'for p in inputs]+[f'.{p}({outputs}[{i}][{b}])'for b,p in enumerate(names)]
 return ','.join(pairs),len(names)


def fixture():
 text='''module baseboard_ff_fixture(input wire host_clk,ref_clk,advance,hold,write,
 input wire [3:0] selected,input wire [1:0] index,input wire [17:0] data,
 input wire [15:0] pins[9],output wire [31:0] physical[9],original_state[9],
 output wire [17:0] result[9],output wire [15:0] outputs[9],original_outputs[9]);
'''
 for i,k in enumerate(KINDS):
  param=' #(.N(4))'if k=='cell_CLOCKGEN'else'';w=sum(w for _,w in gen.FIELDS[k]);p,count=ports(k,i)
  text+=f'begin:case{i}\n{k}_snapshot{param} cell_i(.state_host_clk(host_clk),.state_advance(advance),.state_hold(hold),.state_write(write && selected==4\'d{i}),.state_index(index),.state_data(data),.state_result(result[{i}]),.sys_clk(ref_clk),{p});\n'
  text+=f'{k}{param} ref_i(.sys_clk(ref_clk),{p.replace("outputs[","original_outputs[")});\n'
  for out,prefix in [('physical','cell_i'),('original_state','ref_i')]:
   fields=','.join(prefix+'.'+n for n,_ in reversed(gen.FIELDS[k]));text+=f"assign {out}[{i}]={{{32-w}'b0,{{{fields}}}}};\n"
  text+=f"assign outputs[{i}][15:{count}]=0;assign original_outputs[{i}][15:{count}]=0;\nend\n"
 return text+'endmodule\n'


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);generated=gen.generate();inputs=out/'executed-inputs';inputs.mkdir()
 paths=[Path(__file__).resolve(),HERE/'test_baseboard_ff.cpp',HERE/'generate_baseboard_ff.py',HERE/'generate_snapshot_ff.py']
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 for k in KINDS:(inputs/(k+'.v')).write_bytes(gen.original_source(k))
 cases=[('candidate',None,None,None,None)]
 if mutations:cases +=[
  ('ignore_hold',KINDS[0],'if(state_hold)begin',"if(1'b0)begin",'state oracle differs'),
  ('synchronous_clear',KINDS[1],'if (ck_en) begin',"if (ck_en || !p1) begin",'state oracle differs'),
  ('ignore_advance',KINDS[2],'else if(state_advance)begin',"else if(1'b1)begin",'state oracle differs'),
  ('preset_priority',KINDS[3],'if (!p4)', 'if (!p1)','state oracle differs'),
  ('falling_edge',KINDS[4],'p7 & ~ck_d','~p7 & ck_d','state oracle differs'),
  ('oscillator_step',KINDS[5],"osc_div + 3'd1","osc_div + 3'd2",'state oracle differs'),
  ('lost_high_history',KINDS[6],'{ck_d,cnt[23:18]}<=state_data[6:0];',"{ck_d,cnt[23:18]}<={1'b0,state_data[5:0]};",'state oracle differs'),
  ('por_saturation',KINDS[7],'if (por != POR_CYCLES[15:0])','if (1\'b1)','state oracle differs'),
  ('clock_phase',KINDS[8],'localparam integer ECP_LO = (2 * N) / 16;','localparam integer ECP_LO = 1 + (2 * N) / 16;','output oracle differs'),
  ('invalid_alias',KINDS[0],'default:;',"2'd2:{q}<=state_data[7:0];default:;",'state oracle differs'),
  ('padding',KINDS[1],"state_result={13'b0,{ck_d,q}};","state_result={13'b1,{ck_d,q}};",'read/padding differs')]
 rows=[]
 for name,k,before,after,why in cases:
  d=out/name;d.mkdir();sources=[]
  for kind,(s,m)in generated.items():
   if kind==k:assert s.count(before)==1,(name,before);s=s.replace(before,after,1)
   p=d/(kind+'_snapshot.sv');p.write_text(s);sources.append(p)
  (d/'fixture.sv').write_text(fixture())
  cmd=['verilator','--cc','--exe','--build','-j','4','--top-module','baseboard_ff_fixture','-Wno-fatal','--Mdir',str(d/'obj'),str(d/'fixture.sv'),*map(str,sources),*[str(inputs/(k+'.v'))for k in KINDS],str(inputs/'test_baseboard_ff.cpp')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
  p=subprocess.run([str(d/'obj/Vbaseboard_ff_fixture')],capture_output=True,text=True,timeout=60);text=p.stdout+p.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert p.returncode and why in text,(name,text)
  else:assert not p.returncode and'BASEBOARD_FF PASS'in text,text
  rows.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 controls=[]
 for k in KINDS:
  try:gen.generate_one(k,(inputs/(k+'.v')).read_text().replace('endmodule','reg hidden;\nendmodule'))
  except ValueError as e:controls.append(dict(case=k+'_source',rejected=True,diagnostic=str(e)))
  else:raise AssertionError('unreviewed source accepted')
 (out/'results.json').write_text(json.dumps(dict(status='PASS',manifests={k:m for k,(_,m)in generated.items()},cases=rows,source_controls=controls,representative_state_bits=70,actual_board_state_bits=87,
  executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},scope='Nine local families, actual CLOCKGEN N4, two-state original/oracle/save/restore; no whole-board/global snapshot/timer calibration.'),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
