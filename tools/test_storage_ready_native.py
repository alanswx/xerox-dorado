#!/usr/bin/env python3
"""Compare complete original/candidate native backplanes through power-up.

All128 groups share an independent delayed raw-word backing fixture. Compare
every backplane output and storage address/cache/write/read state after each
host edge, with the native falling-edge global gate and extra forced pauses.
No guest instructions, IOCBs, snapshots or network replies are supplied.
This diagnoses startup/coupling; it is not Cedar or physical SDRAM qualification.
"""
import argparse,hashlib,json,os,re,subprocess,time
from pathlib import Path
from stage_storage_ready_native import BACKPLANE,MSA,GROUP,BASE
ROOT=Path(__file__).resolve().parents[1]
def sha(b):return hashlib.sha256(b).hexdigest()

def ports(raw):
 header=raw.decode().split(');',1)[0];result=[]
 for direction,width,names in re.findall(r'^\s*(input|output)\s+wire\s*(\[[^\]]+\])?\s*([^\n/]+)',header,re.M):
  for name in names.rstrip(', ').split(','):
   name=name.strip();assert re.fullmatch(r'\w+',name),(direction,width,name)
   result.append((direction,width,name))
 assert len(result)==len({r[2]for r in result})and len(result)>300
 return result

def bench(parent,clock_hz=50000000):
 raw=(parent/BACKPLANE).read_bytes();pp=ports(raw)
 wrapper=(parent/(BASE+'dorado_native_machine.sv')).read_text()
 body=wrapper.split('dorado_native_backplane #',1)[1].split(');',1)[0]
 wiring=dict(re.findall(r'^\s*\.(\w+)\(([^()\n]*)\)',body,re.M))
 declarations=[];instances=[];compares=[];constants={}
 known={'sys_clk':'guest_clk','storage_clk':'host_clk','storage_reset':'reset','contb_advance':'advance',
        'baseboard_boot_n':'boot_n','CLK_InBase':'LOCAL_BASE_CLOCK','storage_req_ready':'req_ready','storage_rsp_valid':'rsp_valid','storage_rsp_data':'rsp_data'}
 for direction,width,name in pp:
  if direction=='input':
   if name in known:constants[name]=known[name]
   else:
    value=wiring.get(name,'')
    constants[name]=value if re.fullmatch(r"\d+'[bhd][0-9a-fA-F]+",value)else "'0"
 for label,module in [('candidate','dorado_native_backplane'),('reference','dorado_reference_backplane')]:
  bindings=[]
  for direction,width,name in pp:
   if direction=='input':bindings.append('.'+name+'('+constants[name].replace('LOCAL_BASE_CLOCK',label+'_CLK_OutBase_p_')+')')
   else:
    local=label+'_'+name;declarations.append('wire '+width+' '+local+';');bindings.append('.'+name+'('+local+')')
    if label=='candidate':compares.append(f'if(candidate_{name}!==reference_{name})$fatal(1,"NATIVE_STORAGE_READY output {name} differs cycle=%0d",cycles);')
  instances.append(module+' #(.SYSPER(4)) '+label+'(\n'+',\n'.join(bindings)+'\n);')
 groupchecks=[]
 fields=['addr','cas_d','we_d','write_pending','write_data','read_pipe','data_out','cache_valid','write_done','waiting','saved_write','cache_addr','saved_addr','cache_data','saved_data','old_data']
 for board in ['msa','msaOdd','msa1','msa1Odd','msa2','msa2Odd','msa3','msa3Odd']:
  for g in range(16):
   a='{'+','.join(f'candidate.b_{board}.u_storage_{g}.{f}'for f in fields)+'}'
   b='{'+','.join(f'reference.b_{board}.u_storage_{g}.{f}'for f in fields)+'}'
   groupchecks.append(f'if({a}!=={b})$fatal(1,"NATIVE_STORAGE_READY group {board}/{g} differs cycle=%0d",cycles);')
 text='''`timescale 1ns/1ps
`default_nettype none
module tb_storage_ready_native;
 reg host_clk=0,advance=0,reset=1,boot_n=1;
 always #10 host_clk=~host_clk;
 wire guest_clk=host_clk & advance;
 integer cycles=0,limit=8000000;
 reg [127:0] rsp_valid=0,busy=0,seen=0;
 wire [127:0] req_ready;
 reg [2047:0] rsp_data=0;
 reg [15:0] backing[0:127][0:65535];
 integer delay_left[0:127];
 longint reads=0,writes=0,responses_paused=0,steps=0,pauses=0,checks=0;
 integer cp=0,issues=0,roots=0;
 reg prev_cp=0,prev_issue=0,prev_root=0;
 DECLARATIONS
 assign req_ready=~busy & ~rsp_valid & {128{cycles%19>2}};
 INSTANCES
 // The two boards must agree before one common backing accepts their request.
 // Capture OLD data before the write, then return after variable host latency.
 always @(posedge host_clk)begin
  for(integer g=0;g<128;g++)begin
   if(reset)begin rsp_valid[g]<=0;busy[g]<=0;delay_left[g]<=0;end
   else begin
    if(rsp_valid[g]&&candidate_storage_rsp_ready[g])begin
     rsp_valid[g]<=0;
     if(!advance)responses_paused=responses_paused+1;
    end
    if(busy[g])begin
     if(delay_left[g]==0)begin busy[g]<=0;rsp_valid[g]<=1;end
     else delay_left[g]<=delay_left[g]-1;
    end
    if(candidate_storage_req_valid[g]&&req_ready[g])begin
     if(reference_storage_req_valid[g]!==1'b1 ||
        candidate_storage_req_write[g]!==reference_storage_req_write[g] ||
        candidate_storage_req_addr[16*g+:16]!==reference_storage_req_addr[16*g+:16] ||
        candidate_storage_req_data[16*g+:16]!==reference_storage_req_data[16*g+:16])
       $fatal(1,"NATIVE_STORAGE_READY accepted raw request differs group=%0d",g);
     seen[g]<=1;busy[g]<=1;delay_left[g]<=1+(cycles+g)%11;
     rsp_data[16*g+:16]<=backing[g][candidate_storage_req_addr[16*g+:16]];
     if(candidate_storage_req_write[g])begin
      backing[g][candidate_storage_req_addr[16*g+:16]]<=candidate_storage_req_data[16*g+:16];writes=writes+1;
     end else reads=reads+1;
    end
   end
  end
 end
 // Same falling-edge decision and posedge clock gating as native altclkctrl.
 always @(negedge host_clk)begin
  #1;
  COMPARES
  GROUPCHECKS
  checks=checks+1;
  if(candidate.CPStrb_p_&&!prev_cp)cp=cp+1;
  if(candidate_instruction_signals[4]&&!prev_issue)issues=issues+1;
  if(candidate.CLK_ca_p_&&!prev_root)roots=roots+1;
  prev_cp=candidate.CPStrb_p_;prev_issue=candidate_instruction_signals[4];prev_root=candidate.CLK_ca_p_;
  cycles=cycles+1;
  if(cycles%100000==0)$display("NATIVE_STORAGE_READY_PROGRESS cycles=%0d bb=%h cpu=%h reads=%0d writes=%0d issues=%0d roots=%0d",cycles,candidate_baseboard_debug,candidate_processor_debug,reads,writes,issues,roots);
  reset=cycles<32;
  // Normal external boot button, shared between original and candidate.
  boot_n=!(cycles>=1000000 && cycles<2000000);
  advance<=!reset && candidate_storage_ready && cycles%100003>=17;
  if(advance)steps=steps+1;else pauses=pauses+1;
  if(cycles==limit)begin
   if(seen!=={128{1'b1}} || roots<1000 || pauses<100 || steps<100000 || responses_paused<128)
    $fatal(1,"NATIVE_STORAGE_READY coverage incomplete seen=%h roots=%0d pauses=%0d steps=%0d paused_responses=%0d",seen,roots,pauses,steps,responses_paused);
   $display("NATIVE_STORAGE_READY_STARTUP PASS checks=%0d groups=128 reads=%0d writes=%0d responses_paused=%0d steps=%0d pauses=%0d CP=%0d issues=%0d roots=%0d; original power-up, no Cedar/physical-SDRAM qualification",checks,reads,writes,responses_paused,steps,pauses,cp,issues,roots);
   $finish;
  end
 end
 initial begin
  void'($value$plusargs("cycles=%d",limit));
  for(integer g=0;g<128;g++)for(integer a=0;a<65536;a++)backing[g][a]=0;
 end
endmodule
`default_nettype wire
'''
 if clock_hz!=50000000:
  assert clock_hz in [57142857,66666667]
  text=text.replace('always #10 host_clk=~host_clk;','always #(500000000.0/'+str(clock_hz)+') host_clk=~host_clk;')
 for name,value in [('DECLARATIONS','\n'.join(declarations)),('INSTANCES','\n'.join(instances)),('COMPARES','\n'.join(compares)),('GROUPCHECKS','\n'.join(groupchecks))]:text=text.replace(name,value)
 return text.encode(),constants,len(compares)

def prepare(candidate,parent,out,bad_step=False):
 assert not out.exists()
 manifest='storage-board-native.json'if(candidate/'storage-board-native.json').exists()else'storage-ready-native.json'
 c=json.loads((candidate/manifest).read_text());p=json.loads((parent/'sdram-burst-native.json').read_text())
 assert c['parent_manifest_sha256']==sha((parent/'sdram-burst-native.json').read_bytes())
 for root,r in [(candidate,c),(parent,p)]:
  for n,h in r['source_hashes'].items():assert sha((root/n).read_bytes())==h,(root,n)
 qsf=(candidate/'core/Dorado.qsf').read_text();sources=set();macros=[]
 for kind,value in re.findall(r'^set_global_assignment -name (SYSTEMVERILOG_FILE|VERILOG_FILE|VERILOG_MACRO)\s+([^\n]+)',qsf,re.M):
  value=value.strip().strip('"')
  if kind=='VERILOG_MACRO':macros.append(value)
  else:
   n=value.replace(c['project']+'/','')
   if n.startswith('../src/'):n=n[3:]
   if n.startswith('src/'):assert n in c['source_hashes'],n;sources.add(n)
 out.mkdir(parents=True);frozen=out/'inputs'
 for n in c['source_hashes']:
  if n.startswith('src/'):
   f=frozen/n;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes((candidate/n).read_bytes())
 if bad_step:
  f=frozen/BACKPLANE;b=f.read_bytes();old=b'.storage_step(contb_advance)';assert b.count(old)==8
  f.write_bytes(b.replace(old,b".storage_step(1'b0)"))
 for name,source,old,new in [('reference_backplane.v',BACKPLANE,b'dorado_native_backplane',b'dorado_reference_backplane'),('reference_msa.v',MSA,b'module msa_ddram',b'module reference_msa_ddram'),('reference_group.sv',GROUP,b'module dorado_storage_group',b'module reference_storage_group')]:
  b=(parent/source).read_bytes().replace(old,new)
  if name=='reference_backplane.v':b=b.replace(b'msa_ddram #',b'reference_msa_ddram #')
  if name=='reference_msa.v':b=b.replace(b'dorado_storage_group #',b'reference_storage_group #')
  (frozen/name).write_bytes(b)
 clock_hz=c.get('frequency_hz',50000000)
 tb,constants,outputs=bench(parent,clock_hz);(frozen/'tb.sv').write_bytes(tb)
 (frozen/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 result=dict(scope=__doc__,candidate_manifest=manifest,candidate_manifest_sha256=sha((candidate/manifest).read_bytes()),
  parent_manifest_sha256=sha((parent/'sdram-burst-native.json').read_bytes()),sources=sorted(sources),macros=macros,
  host_period_ns=1000000000/clock_hz,sysper=4,compared_backplane_outputs=outputs,input_fixture=constants,
  negative_control='omit actual clock-gate step at all8 boards'if bad_step else None,
  inputs={str(f.relative_to(frozen)):sha(f.read_bytes())for f in frozen.rglob('*')if f.is_file()},
  firmware_qualified=False,physical_SDRAM_qualified=False)
 (out/'provenance.json').write_text(json.dumps(result,indent=2)+'\n');print('PREPARED_NATIVE_128_GROUP_STARTUP',outputs,'outputs',flush=True)

def run(out,cycles):
 p=json.loads((out/'provenance.json').read_text());inputs=out/'inputs'
 for n,h in p['inputs'].items():assert sha((inputs/n).read_bytes())==h,n
 assert not(out/'run-state.json').exists(),'Observe existing job'
 state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[])
 def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
 def execute(command,label,negative=False):
  with(out/(label+'.log')).open('x')as log:
   proc=subprocess.Popen(list(map(str,command)),cwd=inputs/'src',stdout=log,stderr=subprocess.STDOUT)
   row=dict(stage=label,pid=proc.pid,command=list(map(str,command)),started=time.time());state['actions'].append(row);save();rc=proc.wait();row.update(returncode=rc,finished=time.time());save()
  t=(out/(label+'.log')).read_text()
  assert (rc!=0 and any(d in t for d in ['STORAGE_READY_PREDICTION','STORAGE_BOARD_READY_REDUCTION','NATIVE_STORAGE_READY output','NATIVE_STORAGE_READY group']))if negative else rc==0,(label,t[-3000:])
 save()
 try:
  command=['verilator','--binary','--timing','--autoflush','--top-module','tb_storage_ready_native','-j','4','-Wno-fatal','-Wno-PINMISSING','-Wno-UNOPTFLAT','-Wno-MULTIDRIVEN','-Wno-LATCH','--Mdir',out/'obj','-o','test',*['+define+'+m for m in p['macros']],'-I'+str(inputs/'src'),'-I'+str(inputs/'src/verilog/cells'),'-I'+str(inputs/'src/verilog/generated'),inputs/'tb.sv',*[inputs/n for n in p['sources']],*[inputs/n for n in ['reference_backplane.v','reference_msa.v','reference_group.sv']]]
  execute(command,'compile');execute([out/'obj/test','+cycles='+str(cycles)],'test',bool(p.get('negative_control')))
  if not p.get('negative_control'):assert 'NATIVE_STORAGE_READY_STARTUP PASS'in(out/'test.log').read_text()
  state.update(status='PASS_NATIVE_128_GROUP_WRONG_STEP_REJECTED'if p.get('negative_control')else'PASS_NATIVE_128_GROUP_POWERUP_EQUIVALENCE',finished=time.time());save()
 except BaseException:state.update(status='TERMINAL_FAILED',finished=time.time());save();raise
 print(state['status'],flush=True)

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);m=p.add_mutually_exclusive_group(required=True);m.add_argument('--prepare',type=Path);m.add_argument('--run',type=Path)
 p.add_argument('--candidate',type=Path);p.add_argument('--parent',type=Path);p.add_argument('--cycles',type=int,default=8000000);p.add_argument('--bad-step',action='store_true');a=p.parse_args()
 if a.prepare:prepare(a.candidate.resolve(),a.parent.resolve(),a.prepare.resolve(),a.bad_step)
 else:run(a.run.resolve(),a.cycles)
