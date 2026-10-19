#!/usr/bin/env python3
"""Eight actual 2716 sockets: four-state original/oracle/restore qualification."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import generate_i2716 as gen
HERE=Path(__file__).resolve().parent


def instances():
 text=''
 for i,pkg in enumerate(gen.SOCKETS):
  image='verilog/proms/eprom/BaseBd-'+pkg+'.mem'if pkg in gen.IMAGES else''
  text+=f'''reg [7:0] oracle_{i}[0:2047];
 initial if("{image}"!="")$readmemh("{image}",oracle_{i});
 assign expected_word[{i}]=oracle_{i}[address];
 assign raw[{i}]={{candidate_{i}.en_q,candidate_{i}.logical_d}};
 assign original_raw[{i}]={{original_{i}.en_q,original_{i}.d}};
'''
  for name,kind,clock,out in [('candidate','cell_i2716_snapshot','host','q'),('original','cell_i2716','ref_clk','original_q')]:
   text+=f'''{kind} #(.INIT_FILE("{image}")) {name}_{i}(
'''
   if name=='candidate':text+=f''' .state_host_clk(host),.state_advance(advance),.state_freeze(freeze),.state_guest_held(held),
 .state_request(request && index[4:2]==3'd{i}),.state_write(write),.state_index(index[1:0]),.state_data(data),
 .state_frozen(frozen[{i}]),.state_done(done[{i}]),.state_error(error[{i}]),.state_result(result[{i}]),
'''
   text+=f''' .sys_clk({clock}),.p19(address[10]),.p22(address[9]),.p23(address[8]),.p1(address[7]),.p2(address[6]),.p3(address[5]),
 .p4(address[4]),.p5(address[3]),.p6(address[2]),.p7(address[1]),.p8(address[0]),.p20(cs_n[{i}]),.p18(pd_n[{i}]),
 .p9({out}[{i}][0]),.p10({out}[{i}][1]),.p11({out}[{i}][2]),.p13({out}[{i}][3]),.p14({out}[{i}][4]),
 .p15({out}[{i}][5]),.p16({out}[{i}][6]),.p17({out}[{i}][7]),.p12(1'b0),.p21(1'b1),.p24(1'b1));
'''
 return text


def run(out,mutations):
 out.mkdir(parents=True,exist_ok=False);source,manifest=gen.generate();assets=gen.images();inputs=out/'executed-inputs';inputs.mkdir()
 paths=[Path(__file__).resolve(),HERE/'test_i2716.sv',HERE/'generate_i2716.py',HERE/'generate_snapshot_ff.py',gen.ROOT/'verilog/cells/cell_i2716.v']
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 fixture=(inputs/'test_i2716.sv').read_text().replace(' // INSERT_INSTANCES',instances())
 for pkg,p in assets.items():
  target=out/'verilog/proms/eprom'/p.name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 cases=[('candidate',None,None,None)]
 if mutations:cases +=[
  ('ungated_read','if(!state_hold && state_advance)begin','if(!state_hold)begin','2716 raw pipeline differs'),
  ('ignore_freeze','if(!state_hold && state_advance)begin','if(state_advance)begin','2716 raw pipeline differs'),
  ('ignore_held','state_freeze && state_guest_held','state_freeze','2716 handshake differs'),
  ('running_access','state_hold && state_frozen && state_request','state_request','2716 handshake differs'),
  ('invalid_alias',"state_index==2'b00","1'b1",'2716 handshake differs'),
  ('lost_enable','en_q<=state_data[8];',"en_q<=1'b0;",'2716 raw pipeline differs'),
  ('wrong_rom_read','d    <= mem[a];','d    <= ~mem[a];','2716 raw pipeline differs'),
  ('empty_zero','initial if (INIT_FILE != "") $readmemh(INIT_FILE, mem);','initial begin integer a;for(a=0;a<2048;a=a+1)mem[a]=0;if(INIT_FILE!="")$readmemh(INIT_FILE,mem);end','2716 raw pipeline differs'),
  ('lost_restored_data','restored_d<=state_data[7:0];',"restored_d<=8'b0;",'2716 raw pipeline differs'),
  ('early_resume',"restore_pending<=1'b0;","restore_pending<=1'b0;end else if(!state_hold)begin restore_pending<=1'b0;",'2716 raw pipeline differs'),
  ('padding',"state_result<={9'b0,en_q,logical_d};","state_result<={9'b1,en_q,logical_d};",'2716 response/padding differs')]
 results=[]
 for name,before,after,why in cases:
  d=out/name;d.mkdir();text=source
  if before:assert text.count(before)==1,(name,before);text=text.replace(before,after,1)
  (d/'cell_i2716_snapshot.sv').write_text(text);(d/'fixture.sv').write_text(fixture)
  cmd=['iverilog','-g2012','-s','i2716_fixture','-o',str(d/'test.vvp'),str(d/'fixture.sv'),str(d/'cell_i2716_snapshot.sv'),str(inputs/'cell_i2716.v')]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT,cwd=out)
  p=subprocess.run(['vvp',str(d/'test.vvp')],capture_output=True,text=True,cwd=out,timeout=60)
  (d/'test.txt').write_text(p.stdout+p.stderr);print(name+': '+(p.stdout+p.stderr).strip(),flush=True)
  if why:assert p.returncode and why in p.stdout+p.stderr,(name,p.stdout,p.stderr)
  else:assert not p.returncode and'I2716 PASS'in p.stdout,(p.stdout,p.stderr)
  results.append(dict(case=name,returncode=p.returncode,expected_failure=why))
 guards=[]
 for name,before,after in [('cell','reg [7:0] d;','reg [6:0] d;'),('init','INIT_FILE != ""','INIT_FILE == ""')]:
  original=(inputs/'cell_i2716.v').read_text();assert before in original
  try:gen.generate(original.replace(before,after,1))
  except ValueError as e:guards.append(dict(case=name,rejected=True,diagnostic=str(e)))
  else:raise AssertionError('unreviewed original source accepted')
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 (out/'results.json').write_text(json.dumps(dict(status='PASS',cases=results,source_guards=guards,logical_state_bits=72,
  scope='Four-state original and independent immutable-ROM/pipeline oracle, all8sockets/4images/4empty; no board/global barrier/physical snapshot.',
  source_hashes={str(p.relative_to(gen.ROOT)):hashlib.sha256((inputs/p.name).read_bytes()).hexdigest()for p in paths},image_hashes=gen.IMAGES),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.mutations)
