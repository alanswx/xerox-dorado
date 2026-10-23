#!/usr/bin/env python3
"""Compare actual Control-A clock/task/MIR/CP/Ready feedback with original gates.

Binary data/CP transactions settle separately from root transitions. Primary
checks use the HIGH window; optional probes add LOW CP and extended captures.
Independent delay profiles are digital arrival contracts,
not native IM/IFU/memory-Hold setup or a complete60ns operating proof.
"""
import argparse,hashlib,json,os,re,subprocess,sys,time
from pathlib import Path
from generate_conta_mir_reference import generate,plan,ROOT
from sil_netlist import load_board
from test_task_ready import ORACLE

CANDIDATE='verilog/rtl/dorado_conta_mir_step.sv'
PROFILES=[('balanced',1000,1000,1000),('fast_gates',500,1000,500),('later_control',1000,2000,1000)]
MUTATIONS={
 'external_NoDispatch_clock':('controller_clocks(q[31:0],base,q[69])','controller_clocks(q[31:0],base,p[0])'),
 'old_CP_MIR_epoch':('pins_after,q[68])','pins_after,state_q[68])'),
 'old_CP_clear_epoch':('if(q[73])n=0;','if(source[73])n=0;'),
 'merged_BNT_storage':("raw=i<8 ? q[39:36] : ~q[35:32];","raw=q[39:36];"),
 'missing_Notify':("(!decoded[0] && notify_task==4'(i))","(1'b0 && notify_task==4'(i))"),
 'missing_StopMIR':('mir_clock=normal0 | repeat_cur | p[125];','mir_clock=normal0 | repeat_cur;'),
 'missing_NoDispatch_task_reset':('if(q[69])begin n[8]=0;',"if(1'b0)begin n[8]=0;"),
}

TB=r'''`timescale 1ps/1ps
module tb_conta_mir;
 localparam [142:0] IDLE=143'h128 | (143'h1<<122) | (143'h7<<119) | (143'h1<<124) | (143'h1<<125) | (143'h1<<93) | (143'h1<<142);
 reg base=0,before_base=0;reg [142:0] pins=IDLE,before_pins=IDLE;
 reg [97:0] state=0,fixture=0;reg load=0;
 wire [97:0] next_q,actual;wire [21:0] events,repeated,clocks;
 wire unsettled;wire [38:0] flags;wire [3:0] next_task;wire [42:0] original_flags;
 reg [38:0] committed_flags=0;reg [21:0] observed=0;
 integer checks=0,cp_edges=0,mir_edges=0,task_changes=0;reg [15:0] seen_tasks=0;
 integer counts[0:21];reg [31:0] rng=32'h13cad047;
 function automatic [31:0] random_word;
  begin rng=rng^(rng<<13);rng=rng^(rng>>17);rng=rng^(rng<<5);random_word=rng;end
 endfunction
 function automatic [3:0] raw_task(input integer n);raw_task={n[0],n[1],n[2],n[3]};endfunction
 function automatic [3:0] current_task(input [97:0] q);current_task={q[46],q[47],q[44],q[45]};endfunction
 function automatic [97:0] normalized(input [97:0] value);
  reg [97:0] q;
  begin
   q=value;q[75:64]=0;
   if(q[21])begin q[14]=1;q[15]=1;q[16]=0;q[17]=0;end
   if(q[6]||!q[7])begin q[23]=0;q[24]=0;q[25]=0;q[26]=0;q[27]=0;q[28]=0;q[29]=1;q[31]=0;end
   q[30]=1;normalized=q;
  end
 endfunction
 conta_mir_reference original(load,fixture,base,1'b0,1'b0,pins,actual,clocks,original_flags);
 dorado_conta_mir_step dut(state,before_base,base,before_pins[127:0],pins[127:0],next_q,events,repeated,unsettled,flags,next_task);
 generate for(genvar k=0;k<22;k++)begin: observe
  always @(posedge clocks[k])observed[k]=1;
 end endgenerate
 task automatic event_pins(input next_base,input [142:0] value);
  reg [3:0] old_ct;
  begin
   observed=0;before_base=base;before_pins=pins;old_ct=current_task(state);base=next_base;pins=value;#20000;
   if(actual!==next_q || unsettled || repeated || observed!==events || original_flags!=={next_task,flags})begin
    $display("CONTA_MIR_FAIL check=%0d old=%h actual=%h next=%h diff=%h root=%b->%b pins=%h flags=%h/%h events=%h/%h repeats=%h unsettled=%b",checks,state,actual,next_q,actual^next_q,before_base,base,pins,original_flags,{next_task,flags},observed,events,repeated,unsettled);
    $fatal(1,"Control-A coupled mismatch");
   end
   cp_edges+=events[20]+events[21];mir_edges+=events[12];
   for(integer i=0;i<22;i++)if(events[i])counts[i]++;
   committed_flags=flags;state=next_q;checks++;if(current_task(state)!=old_ct)task_changes++;
   seen_tasks[raw_task(current_task(state))]=1;#100;
  end
 endtask
 task automatic seed(input [97:0] value);
  begin
   base=0;pins=IDLE;#20000;fixture=0;load=1;#20000;load=0;#20000;
   fixture=normalized(value);load=1;#20000;load=0;#20000;load=1;#20000;load=0;#20000;
   if(actual!==normalized(value))$fatal(1,"Control-A fixture normalization mismatch wanted=%h actual=%h",normalized(value),actual);
   state=actual;before_base=base;before_pins=pins;#100;
  end
 endtask
 task automatic cycle;
  begin event_pins(1,pins);event_pins(0,pins);end
 endtask
 task automatic start;
  reg [142:0] p;
  begin
   event_pins(1,pins);p=pins;p[6]=1;event_pins(1,p);
   repeat(24)cycle();event_pins(1,pins);p=pins;p[125]=0;event_pins(1,p);
  end
 endtask
 task automatic cp_write(input integer bank,data);
  reg [142:0] p;
  begin
   if(!base)$fatal(1,"Control-A CP transaction outside HIGH setup window");
   p=pins;p[122]=1;p[121:119]=~3'(bank);p[118:112]=7'(data);event_pins(1,p);
   p[122]=0;event_pins(1,p);p[122]=1;event_pins(1,p);
  end
 endtask
 reg [142:0] p;reg [97:0] q;
 initial begin
  for(integer k=0;k<22;k++)counts[k]=0;
  #20000;seed(0);event_pins(0,pins);start();repeat(64)cycle();
  // No external Ready/Notify/CTask stimulus: a real MIR Notify wakes each task.
  for(integer task_no=1;task_no<16;task_no++)begin
   seed(0);start();p=pins;p[83:76]={raw_task(task_no),4'hf};p[84]=1;p[123]=0;event_pins(1,p);
   repeat(128)cycle();
   if(!seen_tasks[task_no])$fatal(1,"Control-A MIR Notify did not reach task %0d",task_no);
  end
  // A lower current task must yield to task15 during memory Hold.
  seed(0);start();p=pins;p[83:76]={raw_task(1),4'hf};p[84]=1;event_pins(1,p);repeat(128)cycle();
  if(raw_task(current_task(state))!=1)$fatal(1,"Control-A low task setup failed");
  event_pins(1,pins);p=pins;p[69]=1;p[34]=1;event_pins(1,p);repeat(128)cycle();
  if(raw_task(current_task(state))!=15)$fatal(1,"Control-A higher task did not preempt during Hold");
  event_pins(1,pins);p=pins;p[69]=0;event_pins(1,p);
  // Actual CP control outputs drive the running clock/task/MIR/Ready logic.
  cp_write(1,64);repeat(8)cycle();event_pins(1,pins);cp_write(1,0);
  cp_write(0,24);repeat(8)cycle();event_pins(1,pins);cp_write(0,0);
  for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)cp_write(bank,data);
  cp_write(0,0);cp_write(1,0);
  // Independent BNT packages and maintenance reset while stopped.
  for(integer positive=0;positive<16;positive++)for(integer negative=0;negative<16;negative++)begin
   q=0;q[39:36]=4'(positive);q[35:32]=4'(negative);q[40]=1;q[41]=1;q[43]=1;
   seed(q);event_pins(0,pins);event_pins(1,pins);cp_write(0,8);
  end
  // Restart and exercise stable producer arrivals followed by both root edges.
  seed(0);start();
  for(integer trial=0;trial<2048;trial++)begin
   event_pins(1,pins);p=pins;p[34:20]=15'(random_word());p[69]=trial[0];p[65]=trial[1];
   p[83:76]=8'(random_word());p[91:84]=8'(random_word());p[93]=trial[2];
   p[123]=trial[3];p[124]=trial[4];p[125]=trial[5];event_pins(1,p);cycle();
  end
  if(cp_edges<256 || mir_edges<100 || task_changes<16 || seen_tasks!==16'hffff)$fatal(1,"Control-A coverage incomplete cp=%0d mir=%0d tasks=%h changes=%0d",cp_edges,mir_edges,seen_tasks,task_changes);
  for(integer k=0;k<22;k++)if(counts[k]<16)$fatal(1,"Control-A uncovered clock %0d",k);
  $display("CONTA_MIR PASS checks=%0d cp=%0d mir=%0d task_changes=%0d tasks=%h",checks,cp_edges,mir_edges,task_changes,seen_tasks);$finish;
 end
endmodule
'''


def run(out,low_cp=False,extra_captures=False):
 assert not out.exists();out.mkdir(parents=True);inputs=out/'inputs';inputs.mkdir()
 spec,_=plan(ROOT);b=load_board(str(ROOT/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
 kinds=sorted({b.packages[p]['type']for p in set(spec['comb'])|set(spec['seq'])})
 sources=[CANDIDATE,'verilog/verilator/run_control_original_ff.sv','verilog/verilator/processor_capture_original_ff.sv',
  'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl','chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.lc',*[f'verilog/cells/cell_{k}.v'for k in kinds]]
 for m in list(sys.modules.values()):
  name=getattr(m,'__file__',None)
  if name and Path(name).resolve().parent==ROOT/'tools':sources.append(str(Path(name).resolve().relative_to(ROOT)))
 hashes={}
 for name in sorted(set(sources)):
  p=inputs/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/name).read_bytes());hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 bench=TB
 if low_cp:
  bench=bench.replace('   if(!base)$fatal(1,"Control-A CP transaction outside HIGH setup window");','')
  begin=bench.index(' task automatic cp_write(');end=bench.index(' endtask',begin)
  bench=bench[:begin]+bench[begin:end].replace('event_pins(1,p)','event_pins(base,p)')+bench[end:]
  marker='  if(cp_edges<256 || mir_edges<100'
  assert bench.count(marker)==1
  extra='  // Additional CP release events with the root LOW, then ordinary root cycles.\n  event_pins(0,pins);\n  for(integer bank=0;bank<2;bank++)for(integer data=0;data<128;data++)begin cp_write(bank,data);cycle();end\n'
  bench=bench.replace(marker,extra+marker)
 if extra_captures:
  bench=bench.replace('q=value;q[75:64]=0;', 'q=value;q[64]=0;q[65]=0;q[67]=0;q[68]=0;q[69]=0;q[73]=0;')
  bench=bench.replace(' integer checks=0,',' integer last_start,intervals,ordinary_intervals=0,extended_intervals=0,write_im_phases=0;reg old_start;\n integer checks=0,')
  marker='  if(cp_edges<256 || mir_edges<100';assert bench.count(marker)==1
  bench=bench.replace(marker,'  // Both values of otherwise unobserved storage survive a stopped capture.\n  q=\'1;seed(q);event_pins(0,pins);\n  if(state[96]!==1)$fatal(1,"Control-A unobserved MIR half was initialized");\n  seed(0);event_pins(0,pins);\n  // HM2.3 ordinary two-clock and extended six-clock instruction intervals.\n  for(integer ext=0;ext<2;ext++)begin\n   seed(0);start();event_pins(1,pins);p=pins;p[4]=1;p[1]=ext[0];p[8]=1;event_pins(1,p);\n   last_start=-1;intervals=0;\n   for(integer n=0;n<800;n++)begin\n    old_start=state[25];event_pins(n[0],pins);\n    if(!old_start && state[25])begin\n     if(last_start>=0)begin\n      if(counts[0]-last_start!=(ext?6:2))$fatal(1,"Control-A instruction cadence mismatch ext=%0d clocks=%0d",ext,counts[0]-last_start);\n      intervals++;\n     end\n     last_start=counts[0];\n    end\n   end\n   if(intervals<(ext?50:150))$fatal(1,"Control-A cadence coverage incomplete");\n   if(ext)extended_intervals=intervals;else ordinary_intervals=intervals;\n   event_pins(1,pins);p=pins;p[8]=0;event_pins(1,p);\n   for(integer n=0;n<160;n++)begin event_pins(n[0],pins);if(!state[30])write_im_phases++;end\n  end\n  if(write_im_phases<16)$fatal(1,"Control-A WriteIM phase not covered");\n  $display("CONTA_MIR_EXTRA ordinary=%0d extended=%0d write_im=%0d",ordinary_intervals,extended_intervals,write_im_phases);\n'+marker)
 (inputs/'tb_conta_mir.sv').write_text(bench);hashes['tb_conta_mir.sv']=hashlib.sha256(bench.encode()).hexdigest()
 ref=out/'reference';record=generate(inputs,ref);assert record['physical_bits']==98 and record['pin_count']==143
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(cmd,log):
  with log.open('w')as f:rc=subprocess.run(list(map(str,cmd)),cwd=inputs,stdout=f,stderr=subprocess.STDOUT).returncode
  state['actions'].append(dict(command=list(map(str,cmd)),returncode=rc,log=str(log)));save();return rc
 save();summaries=[]
 for profile,gate,ff,phase in PROFILES:
  timed=out/profile/'reference';timed.mkdir(parents=True)
  for kind in kinds:
   if kind in ('F10000','MC10231','SE10231','MC10176','MC10141'):continue
   raw=(inputs/f'verilog/cells/cell_{kind}.v').read_text();raw=re.sub(r'`timescale[^\n]*\n','',raw)
   raw=re.sub(r'\bassign\s+(p\d+)\b',lambda m:f'assign #({gate}) '+m[1],raw)
   (timed/f'cell_{kind}.v').write_text('`timescale 1ps/1ps\n'+raw)
  for file in ['run_control_original_ff.sv','processor_capture_original_ff.sv']:
   raw=(inputs/'verilog/verilator'/file).read_text();raw=re.sub(r'`timescale[^\n]*\n','',raw)
   chunks=re.split(r'(module original_SE10231\b.*?endmodule)',raw,flags=re.S)
   for i,s in enumerate(chunks):chunks[i]=s.replace('<=',f'<=#({phase if s.startswith("module original_SE10231")else ff}) ')
   (timed/file).write_text('`timescale 1ps/1ps\n'+''.join(chunks))
  (timed/'ready_ff.sv').write_text(ORACLE.replace('DELAY_PS=1000',f'DELAY_PS={ff}'))
  cases=[('original',(inputs/CANDIDATE).read_text())]
  if profile=='balanced'and not extra_captures:
   for label,(a,b)in MUTATIONS.items():
    source=cases[0][1];assert source.count(a)==1,(label,a);cases.append((label,source.replace(a,b)))
  for engine in ['icarus','verilator']:
   for label,source in cases:
    d=out/profile/(engine+'-'+label);d.mkdir();candidate=d/'candidate.sv';candidate.write_text(source)
    support=[candidate,inputs/'tb_conta_mir.sv',ref/'conta_mir_reference.sv',*timed.glob('*.v'),*timed.glob('*.sv')]
    cmd=['iverilog','-g2012','-s','tb_conta_mir','-o',d/'test.vvp',*support]if engine=='icarus'else['verilator','--binary','--timing','--top-module','tb_conta_mir','-Wno-fatal','--Mdir',d/'obj','-o','test',*support]
    assert execute(cmd,d/'compile.log')==0,d/'compile.log'
    rc=execute(['vvp',d/'test.vvp']if engine=='icarus'else[d/'obj/test'],d/'test.log');log=(d/'test.log').read_text()
    if label=='original':
     assert rc==0 and 'CONTA_MIR PASS'in log,(profile,engine,log)
     summaries.append(re.search(r'CONTA_MIR PASS ([^\n]+)',log)[1]);print(profile,engine,summaries[-1],flush=True)
    else:assert rc!=0 and 'Control-A coupled mismatch'in log,(profile,engine,label,log)
 assert len(set(summaries))==1,summaries
 for name,digest in hashes.items():assert hashlib.sha256((inputs/name).read_bytes()).hexdigest()==digest,name
 r=dict(status='PASS_CONTA_MIR_EXTENDED_CAPTURES'if extra_captures else'PASS_CONTA_MIR_COUPLING',scope=__doc__,inputs=hashes,reference=record,summary=summaries[0],actions=state['actions'],profiles=PROFILES,rejected_mutations=[]if extra_captures else list(MUTATIONS),low_CP_arrivals_probed=low_cp,extended_captures_probed=extra_captures,native_or_timing_qualified=False)
 (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');state.update(status=r['status'],finished=time.time());save();print(r['status'],flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--low-cp',action='store_true');p.add_argument('--extra-captures',action='store_true');a=p.parse_args();out=a.output.resolve()
 try:run(out,a.low_cp,a.extra_captures)
 except BaseException as error:
  f=out/'run-state.json'
  if f.exists():
   r=json.loads(f.read_text());r.update(status='TERMINAL_FAILED',error=repr(error),finished=time.time());f.write_text(json.dumps(r,indent=2)+'\n')
  raise
