#!/usr/bin/env python3
"""UIO and bounded board fabric qualification, no FPGA or guest state imports."""
import argparse,hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]


def run(out,ethernet,mutations):
 out.mkdir(parents=True,exist_ok=False);inputs=out/'executed-inputs';inputs.mkdir()
 paths=[Path(__file__).resolve(),HERE/'test_board_state.sv',HERE/'board_state_fabric.sv',HERE/'board_state_uio.sv',ethernet]
 for p in paths:(inputs/p.name).write_bytes(p.read_bytes())
 cases=[('candidate',None,None,None,None)]
 if mutations:cases +=[
  ('width_alias','board_state_fabric.sv','0:index_valid=!(|index[23:12]);',"0:index_valid=1'b1;",'result differs'),
  ('running_port','board_state_fabric.sv','boundary && !pending','!pending','running command accepted'),
  ('early_control','board_state_uio.sv','2:begin mode<=din[0];','2:begin mode<=din[0];freeze<=din[0];','short control accepted'),
  ('short_request','board_state_uio.sv','count==8','count>=7','short request accepted'),
  ('all_boards','board_state_fabric.sv',"(11'b1<<board)","11'h7ff",'nononehot child request'),
  ('owner_alias','board_state_fabric.sv','owner<=board;',"owner<=0;",'result differs'),
  ('write_leak','board_state_fabric.sv','board_error[owner] || was_write','board_error[owner]','result differs'),
  ('lost_quarantine','board_state_fabric.sv','!pending && !fault','!pending','timeout did not quarantine port'),
  ('torn_reply','board_state_uio.sv','7:dout<=result_snapshot[15:0]','7:dout<=last_result[15:0]','incoherent status snapshot'),
  ('busy_release','board_state_uio.sv','valid_packet && count==4 && !busy','valid_packet && count==4','busy boundary release accepted'),
  ('incomplete_boundary','board_state_fabric.sv','freeze && guest_held && (&board_frozen)','freeze && guest_held','incomplete boundary accepted'),
  ('command_collision','board_state_uio.sv',"command>=16'h76","command>=16'h70",'EXT_BUS ownership collision'),
  ('padding','board_state_uio.sv',"if(|din[15:2])valid_packet<=0;","if(1'b0)valid_packet<=0;",'malformed request accepted')]
 records=[]
 for name,target,before,after,why in cases:
  d=out/name;d.mkdir();sources=[]
  for n in('test_board_state.sv','board_state_fabric.sv','board_state_uio.sv',ethernet.name):
   raw=(inputs/n).read_bytes()
   if n==target:assert raw.count(before.encode())==1,(name,before);raw=raw.replace(before.encode(),after.encode(),1)
   p=d/n;p.write_bytes(raw);sources.append(p)
  cmd=['iverilog','-g2012','-s','board_state_fixture','-o',str(d/'test.vvp'),*map(str,sources)]
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
  with(d/'build.log').open('w')as f:subprocess.run(cmd,check=True,stdout=f,stderr=subprocess.STDOUT)
  r=subprocess.run(['vvp',str(d/'test.vvp')],capture_output=True,text=True,timeout=60);text=r.stdout+r.stderr;(d/'test.txt').write_text(text);print(name+': '+text.strip(),flush=True)
  if why:assert r.returncode and why in text,(name,text)
  else:assert not r.returncode and'BOARD STATE PASS'in text,text
  records.append(dict(case=name,returncode=r.returncode,expected_failure=why))
 (out/'results.json').write_text(json.dumps(dict(status='PASS',scope='Board-only diagnostic transport and stub-board routing, not machine/global snapshot; unchanged native59Ethernet coexistence',executed_inputs={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs.iterdir()},cases=records),indent=2)+'\n')


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ethernet',type=Path,required=True);p.add_argument('--mutations',action='store_true');a=p.parse_args();run(a.output.resolve(),a.ethernet.resolve(),a.mutations)
