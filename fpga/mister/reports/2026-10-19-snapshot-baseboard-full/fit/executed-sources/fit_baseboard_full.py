#!/usr/bin/env python3
"""Matched actual complete BaseBoard fits with enabled local state access."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(root,out):
 import generate_baseboard_full as gen
 from test_baseboard_full import fixture
 from fit_f10415 import AUDIT
 from fit_f10415_enable import SDC
 out.mkdir(parents=True,exist_ok=False);board,m,clones=gen.generate(root);_,_,meta=fixture(root,m)
 inputs=[p for p in meta['ports']if p['direction']=='input'];outputs=[p for p in meta['ports']if p['direction']=='output'];assert sum(p['width']for p in inputs)==64 and sum(p['width']for p in outputs)==100
 top='''module snapshot_fit(input wire host_clk,advance,freeze,held,request,write,
 input wire [63:0] pins,input wire [16:0] index,input wire [17:0] data,
 output reg [99:0] outputs,output reg [17:0] result,output reg frozen,done,error);
 reg [103:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,freeze,held,request,write,pins,index,data};
 always @(posedge host_clk)launch<=input_stage;
 wire advance_r,freeze_r,held_r,request_r,write_r;wire [63:0] pins_r;wire [16:0] index_r;wire [17:0] data_r;
 assign {advance_r,freeze_r,held_r,request_r,write_r,pins_r,index_r,data_r}=launch;
 wire [99:0] outputs_w;wire [17:0] result_w;wire frozen_w,done_w,error_w;
 always @(posedge host_clk){outputs,result,frozen,done,error}<={outputs_w,result_w,frozen_w,done_w,error_w};
'''
 offset=0;out_names={p['name']for p in outputs}
 for p in outputs:top+='wire '+(''if p['width']==1 else'['+str(p['width']-1)+':0] ')+p['name']+';\n'
 common=['.sys_clk(host_clk)']
 for p in inputs:
  value=f'pins_r[{offset}]'if p['width']==1 else f"pins_r[{offset}+:{p['width']}]"
  if p['name']+'__drv'in out_names:value+=' | '+p['name']+'__drv'
  common.append('.'+p['name']+'('+value+')');offset+=p['width']
 common +=['.'+p['name']+'('+p['name']+')'for p in outputs]
 top+='assign outputs_w={'+','.join(p['name']for p in reversed(outputs))+'};\n'
 top+='''`ifdef SNAPSHOT_BANK_EXPERIMENT
'''+gen.MODULE+''' #(.SYSPER(4)) board(
 .state_host_clk(host_clk),.state_guest_advance(advance_r),.state_freeze(freeze_r),.state_guest_held(held_r),
 .state_request(request_r),.state_write(write_r),.state_index(index_r),.state_data(data_r),
 .state_frozen(frozen_w),.state_done(done_w),.state_error(error_w),.state_result(result_w),
'''+','.join(common)+''');
`else
 assign {frozen_w,done_w,error_w,result_w}=21'b0;
'''+gen.ORIGINAL_MODULE+' #(.SYSPER(4)) board('+','.join(common)+');\n`endif\nendmodule\n'
 qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE BaseBd_snapshot.sv
set_global_assignment -name VERILOG_FILE BaseBd_original.v
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
 for name in m['original_sources']:qsf+='set_global_assignment -name '+('SYSTEMVERILOG_FILE'if name.endswith('.sv')else'VERILOG_FILE')+' '+Path(name).name+'\n'
 for name in clones:qsf+='set_global_assignment -name '+('SYSTEMVERILOG_FILE'if name.endswith('.sv')else'VERILOG_FILE')+' '+name+'\n'
 for port in('advance','freeze','held','request','write','pins[*]','index[*]','data[*]','outputs[*]','result[*]','frozen','done','error'):qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
 for variant in('baseline','snapshot'):
  d=out/variant;d.mkdir();(d/'BaseBd_original.v').write_bytes((root/gen.SOURCE).read_bytes());(d/'BaseBd_snapshot.sv').write_text(board)
  for name in m['original_sources']:shutil.copyfile(root/name,d/Path(name).name)
  for name,s in clones.items():(d/name).write_bytes(s if isinstance(s,bytes)else s.encode())
  for p in gen.eprom.images(root).values():
   target=d/p.relative_to(root);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,target)
  for name,text in {'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
   'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():(d/name).write_text(text)
 here=Path(__file__).resolve().parent
 sources=[Path(__file__).resolve(),here/'generate_baseboard_full.py',here/'test_baseboard_full.py',here/'fit_f10415.py',here/'fit_f10415_enable.py']
 executed=out/'executed-sources';executed.mkdir()
 for p in sources:shutil.copyfile(p,executed/p.name)
 record=dict(manifest=m,builder_sha256=digest(Path(__file__)),source_hashes={str(p.relative_to(gen.ROOT)):digest(p)for p in sources},
  compiled_files={str(p.relative_to(out)):digest(p)for p in out.rglob('*')if p.is_file()},expected_qsf={v:(out/v/'snapshot_fit.qsf').read_text()for v in('baseline','snapshot')},
  scope='Complete actual nominal BaseBoard40366modeled mutable bits,all local state paths enabled,actualSYSPER4/N4,original6502/fiveRIOTs/fourbyteSRAMs/eightEPROMs. Matched50MHz staged-input positive launch/capture,20ns embedded paths,2nsmax/0nsmin virtualIO,no cuts. Isolated board,not enabled whole-core/global barrier/physical restore or timing calibration.')
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(out,quartus):
 r=json.loads((out/'provenance.json').read_text());assert digest(Path(__file__))==r['builder_sha256']
 for n,h in r['compiled_files'].items():assert digest(out/n)==h,'prepared input differs:'+n
 for variant in('baseline','snapshot'):
  d=out/variant;assert not(d/'map.log').exists(),'inspect existing job;do not restart'
  for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
   print(variant,label,flush=True)
   with(d/(label+'.log')).open('w')as f:subprocess.run([str(quartus/tool),*args],cwd=d,stdout=f,stderr=subprocess.STDOUT,check=True)
 print('Complete BaseBoard fits terminal; independent full source/memory/timing audit required',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--frozen-root',type=Path);p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare and a.frozen_root:prepare(a.frozen_root.resolve(),a.prepare.resolve())
 elif a.run:run(a.run.resolve(),a.quartus)
 else:p.error('Specify --prepare/--frozen-root or --run')
