#!/usr/bin/env python3
"""Replay the complete original SRAM/admin cell against the tagged-read cell.

Initialize through its real administrative port, then vary guest writes,
selects, addresses, freeze/restore/reads and invalid admin addresses. Compare
every RAM bit and administrative output continuously; guest data must match
whenever the new read boundary says ready. No native boot state is planted.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from qualify_memc_read_buffer import RTL,transform
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
TB='''`timescale 1ns/1ps
module tb;
 reg clk=0;reg [35:0] stimulus=0;
 wire advance,freeze,held,request,write;wire [8:0] index;wire [3:0] data;
 wire [17:0] pins;
 assign {advance,freeze,held,request,write,index,data,pins}=stimulus;
 wire [3:0] expected,actual,original_result,buffer_result;
 wire ofrozen,odone,oerror,bfrozen,bdone,berror,ready;
 integer checks=0,matched_reads=0,misses=0,admin=0,guest_writes=0;
 reg [63:0] random_state=64'h12ab34cd5678ef09;
 cell_MB7071H_snapshot original(CONNECTIONS_ORIGINAL);
 dorado_memc_read_buffer buffered(CONNECTIONS_BUFFERED);
 task automatic check;
  begin
   for(integer i=0;i<256;i++)if(original.mem[i]!==buffered.mem[i])$fatal(1,"MEMC_READ RAM mismatch row=%0d",i);
   if({ofrozen,odone,oerror,original_result}!=={bfrozen,bdone,berror,buffer_result})$fatal(1,"MEMC_READ admin mismatch");
   if(ready===1'b1)begin
    if(actual!==expected)$fatal(1,"MEMC_READ guest mismatch actual=%h expected=%h",actual,expected);
    matched_reads++;
   end else misses++;
   checks++;
  end
 endtask
 task automatic tick;
  begin #2;check;clk=1;#2;check;clk=0;#2;end
 endtask
 initial begin
  // Borrow the original port for initialization; no hierarchical RAM writes.
  stimulus={5'b01100,9'd0,4'd0,18'd0};tick;
  for(integer i=0;i<256;i++)begin
   stimulus={5'b01111,9'(i),4'(i^(i>>4)),18'd0};tick;
  end
  stimulus=0;tick;
  for(integer i=0;i<4096;i++)begin
   random_state=random_state^(random_state<<13);random_state=random_state^(random_state>>7);random_state=random_state^(random_state<<17);
   stimulus=random_state[35:0];
   // Both transparent guest stores and every admin state get exercised.
   if(i%4==0)begin
    stimulus[35]=1;stimulus[34:33]=0;
    stimulus[17:13]=0;guest_writes++;
   end
   if(i%4==1)begin stimulus[34:33]=3;admin++;end
   tick;
  end
  if(matched_reads<4000 || misses<1000 || admin!=1024 || guest_writes!=1024)$fatal(1,"MEMC_READ coverage missing");
  $display("MEMC_READ_CELL_PASS checks=%0d matches=%0d misses=%0d admin=%0d guest_writes=%0d RAM=1024",checks,matched_reads,misses,admin,guest_writes);
  $finish;
 end
endmodule
'''


def bench():
    common=['.state_host_clk(clk)','.state_advance(advance)','.state_freeze(freeze)','.state_guest_held(held)',
      '.state_request(request)','.state_write(write)','.state_index(index)','.state_data(data)','.sys_clk(clk)']
    common += [f'.p{i}(pins[{19-i}])'for i in range(2,20)]
    text=TB
    for kind,out,prefix,result in [('ORIGINAL','expected','o','original_result'),('BUFFERED','actual','b','buffer_result')]:
        ports=common+[f'.state_frozen({prefix}frozen)',f'.state_done({prefix}done)',f'.state_error({prefix}error)',f'.state_result({result})']
        ports += [f'.p{i}({out}[{23-i}])'for i in range(20,24)]
        if kind=='BUFFERED':ports.append('.read_ready(ready)')
        text=text.replace('CONNECTIONS_'+kind,',\n  '.join(ports))
    return text


def run(proof,out,engine):
    assert not out.exists();out.mkdir(parents=True)
    p=json.loads((proof/'run-state.json').read_text());verify(proof,p['files'])
    assert p['status']=='PASS_ACTUAL_MEMC_TAGGED_SRAM_READ'and p['assumptions']==0
    assert (ROOT/RTL).read_text()==transform((proof/'original.sv').read_text())
    shutil.copy2(proof/'original.sv',out/'original.sv');shutil.copy2(ROOT/RTL,out/'buffered.sv');(out/'tb.sv').write_text(bench())
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    paths=[out/n for n in ['original.sv','buffered.sv','tb.sv']]
    state=dict(status='RUNNING',started=time.time(),scope=__doc__,engine=engine,proof_state_sha256=sha(proof/'run-state.json'),
        files={q.name:sha(q)for q in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    cmds=([['iverilog','-g2012','-s','tb','-o',str(out/'test.vvp'),*map(str,paths)],['vvp',str(out/'test.vvp')]]if engine=='icarus'else
          [['verilator','--binary','--timing','--top-module','tb','-Wno-fatal','-j','2','--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],[str(out/'obj/test')]])
    save();code=1
    for label,cmd in zip(['compile','test'],cmds):
        with(out/(label+'.log')).open('x')as log:
            child=subprocess.Popen(cmd,stdout=log,stderr=log);a=dict(label=label,pid=child.pid,command=cmd,started=time.time())
            state['actions'].append(a);save();code=child.wait()
        a.update(returncode=code,finished=time.time(),log_sha256=sha(out/(label+'.log')));save()
        if code:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    passed=code==0 and 'MEMC_READ_CELL_PASS'in log
    state.update(status='PASS_ACTUAL_MEMC_READ_CELL_REPLAY'if passed else'TERMINAL_MEMC_READ_CELL_REPLAY_NOT_ESTABLISHED',returncode=code,finished=time.time(),native_or_firmware_or_speed_qualified=False)
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['proof','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    a=p.parse_args();raise SystemExit(run(a.proof.resolve(),a.output.resolve(),a.engine))
