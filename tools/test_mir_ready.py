#!/usr/bin/env python3
"""Qualify original MIR/CP-to-Ready digital ordering in one extracted graph.

Stable binary boundary inputs precede phase edges; CP transactions are separate
from phase edges. Three illustrative gate/FF delay profiles are ordering tests,
not analog characterization, IM/phase/arbiter closure or native60ns proof.
"""
import argparse,hashlib,json,re,subprocess,sys,os,time
from pathlib import Path
from generate_mir_ready_reference import generate,COMB,ROOT
from sil_netlist import load_board
from test_task_ready import ORACLE,PROFILES

CANDIDATE='verilog/rtl/dorado_mir_ready_step.sv'
SUPPORT=['verilog/rtl/dorado_mir_capture_step.sv','verilog/rtl/dorado_cp_control_step.sv','verilog/rtl/dorado_task_ready_step.sv']
TB=r'''`timescale 1ps/1ps
module tb_mir_ready;
 reg [98:0] pins=0,before_pins=0;
 reg [49:0] state=0,fixture=0;reg load=0;
 wire [49:0] actual,next_q;wire [9:0] clocks,events,after_clocks;
 wire [38:0] original_flags,flags;reg [38:0] last_flags=0;
 reg [9:0] observed=0;integer checks=0,notify_tasks=0,shared_old_epochs=0,cp_edges=0,held=0;
 reg [49:0] seen_zero=0,seen_one=0;integer counts[0:9];
 reg [31:0] rng=32'hd04a1137;
 function automatic [31:0] random_word;
  begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);random_word=rng;end
 endfunction
 function automatic [3:0] raw_task(input integer n);raw_task={n[0],n[1],n[2],n[3]};endfunction
 mir_ready_reference original(load,fixture,1'b0,1'b0,1'b0,pins,actual,clocks,original_flags);
 dorado_mir_ready_step dut(state,before_pins[90:0],pins[90:0],next_q,events,after_clocks,flags);
 generate for(genvar k=0;k<10;k++)begin: count_edges
  always @(posedge clocks[k])observed[k]=1;
 end endgenerate
 task automatic event_pins(input [98:0] value);
  begin
   observed=0;before_pins=pins;pins=value;#20000;
   if(actual!==next_q || clocks!==after_clocks || observed!==events || original_flags!==flags)begin
    $display("MIR_READY_FAIL check=%0d q=%h/%h flags=%h/%h clocks=%b/%b events=%b/%b before=%h after=%h old=%h",checks,actual,next_q,original_flags,flags,clocks,after_clocks,observed,events,before_pins,pins,state);
    $fatal(1,"MIR Ready coupled mismatch");
   end
   for(integer k=0;k<10;k++)if(events[k])counts[k]++;
   cp_edges+=events[2]+events[3];seen_zero|=~next_q;seen_one|=next_q;last_flags=flags;state=next_q;checks++;#100;
  end
 endtask
 task automatic seed(input [49:0] value);
  reg [98:0] quiet;
  begin
   // First load quiet CP state; then load all original physical Qs after the
   // former rMIR/ClrReady levels have fallen. Fixture is comparison-only.
   quiet=0;quiet[79]=1;quiet[78:76]=3'b111;quiet[90:87]=4'hf;
   pins=quiet;#20000;fixture=value;fixture[20]=0;fixture[25]=0;
   load=1;#20000;load=0;#20000;load=1;#20000;load=0;#20000;
   state=actual;before_pins=pins;#100;
  end
 endtask
 task automatic cp_write(input integer bank,data);
  reg [98:0] p;
  begin
   p=pins;p[79]=1;p[78:76]=~3'(bank);p[75:69]=7'(data);event_pins(p);
   p[79]=0;event_pins(p);p[79]=1;event_pins(p);
  end
 endtask
 reg [98:0] p;reg [49:0] initial_q;reg [21:0] held_mir;
 initial begin
  for(integer k=0;k<10;k++)counts[k]=0;
  #20000;seed(0);event_pins(pins);
  // Actual MIR Notify on every task. Coincident MIR/Ready clocks consume OLD
  // MIR; the NEXT Ready edge consumes the newly loaded instruction.
  for(integer task_no=1;task_no<16;task_no++)begin
   seed(0);p=pins;p[12]=1;p[81]=1;p[31:24]={raw_task(task_no),4'hf};p[32]=1;
   event_pins(p);p[18]=1;p[86]=1;event_pins(p);
   if(last_flags[29:15]!==0)$fatal(1,"MIR Ready used new MIR before setup");shared_old_epochs++;
   p[18]=0;p[86]=0;event_pins(p);p[18]=1;event_pins(p);
   if(!last_flags[14+task_no])$fatal(1,"MIR Notify task not ready");notify_tasks++;
   // StopMIRClk freezes the fanout HIGH once asserted during its HIGH window.
   p[86]=1;event_pins(p);p[85]=1;event_pins(p);held_mir=state[49:28];
   p[86]=0;p[31:24]=0;event_pins(p);p[86]=1;event_pins(p);
   if(state[49:28]!==held_mir)$fatal(1,"MIR held clock captured late data");held++;
  end
  // Distinct stored d23/d24 pairs must feed the actual wired-OR decoder.
  for(integer pattern=0;pattern<256;pattern++)begin
   initial_q=0;initial_q[31:28]=4'hf;initial_q[43:40]=4'(pattern);initial_q[37:36]=2'(pattern>>4);
   seed(initial_q);p=pins;p[80]=pattern[6];p[81]=pattern[7];event_pins(p);
  end
  // Positive and negative BNT packages have independent startup/maintenance Qs.
  for(integer nt=0;nt<16;nt++)for(integer nt_n=0;nt_n<16;nt_n++)begin
   seed(50'h3ffffffffffff);p=pins;p[3:0]=4'(nt);p[90:87]=4'(nt_n);p[12]=0;event_pins(p);
   p[18]=1;event_pins(p);p[18]=0;event_pins(p);
  end
  // ClrReady from a real i02 transaction clears populated Ready without a phase edge.
  seed(50'hffff);event_pins(pins);cp_write(1,64);
  if(state[15:0]!==0)$fatal(1,"MIR actual CP clear absent");
  for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)cp_write(bank,data);
  // All FF bytes and low JCN combinations, from the real MIR data inputs.
  seed(0);
  for(integer ff=0;ff<256;ff++)for(integer jcn=0;jcn<16;jcn++)begin
   // Quiet maintenance first; no CP/phase edge coincidence is assumed.
   if(state[20]||state[25])begin cp_write(0,0);cp_write(1,0);end
   p=pins;p[85:84]=0;p[86]=0;p[18]=0;event_pins(p);
   p[31:24]=8'(ff);p[39:32]=8'(jcn);p[41]=jcn[2];p[81]=1;p[80]=jcn[3];
   p[82]=jcn[0];p[83]=jcn[1];p[7:4]=raw_task(ff%16);p[98:91]=8'(random_word());event_pins(p);
   p[86]=1;p[18]=1;event_pins(p);shared_old_epochs++;
   p[18]=0;event_pins(p);p[18]=1;event_pins(p);
  end
  if(notify_tasks!=15 || shared_old_epochs<4096 || cp_edges<256 || held!=15 || seen_zero!==50'h3ffffffffffff || seen_one!==50'h3ffffffffffff)
   $fatal(1,"MIR Ready coverage incomplete zero=%h one=%h",seen_zero,seen_one);
  for(integer k=0;k<10;k++)if(counts[k]<128)$fatal(1,"MIR Ready clock not covered k=%0d",k);
  $display("MIR_READY PASS checks=%0d notify_tasks=%0d shared_old_epochs=%0d cp_edges=%0d held=%0d",checks,notify_tasks,shared_old_epochs,cp_edges,held);$finish;
 end
endmodule
'''
MUTATIONS={
 'new_MIR_epoch':('capture_decode=decode(state_q[49:28],pins_after)','capture_decode=decode(mir_next,pins_after)'),
 'merge_JCN_storage':('q[9] | q[8] | q[12] | q[13]','q[9] | q[8] | q[14] | q[15]'),
 'ignore_CP_MIR_reset':('{3{cp_next[4]}}',"{3{1'b0}}"),
 'old_CP_clear_epoch':('ready_after[15]=cp_next[9]','ready_after[15]=state_q[25]'),
 'ignore_preemption_hold':('!(p[82] | q[17])','!q[17]'),
 'merge_BNT_storage':('.INDEPENDENT_BNT(1)','.INDEPENDENT_BNT(0)'),
}


def run(out):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
 b=load_board(str(ROOT/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
 kinds=sorted({b.packages[p]['type']for p in COMB}|{'MC10231','MC10176','F10000'})
 sources=[CANDIDATE,*SUPPORT,'verilog/verilator/run_control_original_ff.sv','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',*[f'verilog/cells/cell_{k}.v'for k in kinds]]
 for m in list(sys.modules.values()):
  name=getattr(m,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.append(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for name in sorted(set(sources)):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 (inputs/'tb_mir_ready.sv').write_text(TB);hashes['tb_mir_ready.sv']=hashlib.sha256(TB.encode()).hexdigest()
 ref=out/'reference';record=generate(inputs,ref);assert record['physical_bits']==50 and record['pin_count']==99
 actions=[];summaries=[]
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=actions)
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 save()
 def execute(cmd,log):
  with log.open('w')as f:rc=subprocess.run(list(map(str,cmd)),cwd=inputs,stdout=f,stderr=subprocess.STDOUT).returncode
  actions.append(dict(command=list(map(str,cmd)),returncode=rc,log=str(log)));save();return rc
 source=(inputs/CANDIDATE).read_text()
 for profile,gate,ff in PROFILES:
  timed=out/profile/'reference';timed.mkdir(parents=True)
  for kind in kinds:
   if kind in ('F10000','MC10231','MC10176'):continue
   raw=(inputs/f'verilog/cells/cell_{kind}.v').read_text()
   raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:f'assign #({gate}) '+m[1],raw)
   (timed/f'cell_{kind}.v').write_text('`timescale 1ps/1ps\n'+raw)
  raw=(inputs/'verilog/verilator/run_control_original_ff.sv').read_text().replace('`timescale 1ns/1ps','`timescale 1ps/1ps')
  raw=re.sub(r'(state_q(?:\[\d+\])?)<=',lambda m:m[1]+f'<=#({ff})',raw)
  (timed/'original_ff.sv').write_text(raw);(timed/'original_ready.sv').write_text(ORACLE.replace('DELAY_PS=1000',f'DELAY_PS={ff}'))
  cases=[('original',source)]
  if profile=='balanced':
   for label,(a,c)in MUTATIONS.items():assert source.count(a)==1,(label,a);cases.append((label,source.replace(a,c)))
  for engine in ('icarus','verilator'):
   for label,text in cases:
    d=out/profile/(engine+'-'+label);d.mkdir();candidate=d/'candidate.sv';candidate.write_text(text)
    support=[candidate,*[inputs/n for n in SUPPORT],inputs/'tb_mir_ready.sv',ref/'mir_ready_reference.sv',*timed.glob('*.v'),*timed.glob('*.sv')]
    cmd=['iverilog','-g2012','-s','tb_mir_ready','-o',d/'test.vvp',*support]if engine=='icarus'else['verilator','--binary','--timing','--top-module','tb_mir_ready','-Wno-fatal','--Mdir',d/'obj','-o','test',*support]
    assert execute(cmd,d/'compile.log')==0,d/'compile.log'
    rc=execute(['vvp',d/'test.vvp']if engine=='icarus'else[d/'obj/test'],d/'test.log');log=(d/'test.log').read_text()
    if label=='original':
     assert rc==0 and 'MIR_READY PASS'in log,(profile,engine,log)
     summaries.append(re.search(r'MIR_READY PASS ([^\n]+)',log)[1]);print(profile,engine,summaries[-1],flush=True)
    else:assert rc!=0 and ('MIR Ready coupled mismatch'in log or 'MIR Ready used new MIR before setup'in log),(profile,engine,label,log)
 assert len(summaries)==6 and len(set(summaries))==1,summaries
 for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,name
 r=dict(status='PASS_MIR_CP_READY_COUPLED',scope=__doc__,inputs=hashes,reference=record,actions=actions,summary=summaries[0],profiles=PROFILES,rejected_mutations=list(MUTATIONS),native_or_timing_or_arrivals_qualified=False)
 (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');state.update(status=r['status'],finished=time.time());save();print(r['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();out=a.output.resolve()
 try:run(out)
 except BaseException as error:
  path=out/'run-state.json'
  if path.exists():
   state=json.loads(path.read_text());state.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());path.write_text(json.dumps(state,indent=2)+'\n')
  raise
