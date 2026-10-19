#!/usr/bin/env python3
"""Matched eight-socket BaseBoard EPROM fit, no production or hardware change."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(out):
 import generate_i2716 as gen
 from fit_f10415 import AUDIT
 from fit_f10415_enable import SDC
 here=Path(__file__).resolve().parent;out.mkdir(parents=True,exist_ok=False);adapter,manifest=gen.generate();assets=gen.images()
 top='''module snapshot_fit(
 input wire host_clk,input wire [7:0] advance,ce_n,pd_n,freeze,held,request,write,
 input wire [87:0] address,input wire [143:0] state_data,input wire [15:0] word_index,
 output reg [63:0] q,output reg [143:0] result,output reg [7:0] frozen,done,error);
'''
 for i,pkg in enumerate(gen.SOCKETS):
  image='verilog/proms/eprom/BaseBd-'+pkg+'.mem'if pkg in gen.IMAGES else''
  top+=f'''
 reg [37:0] launch_{i};
 always @(negedge host_clk)launch_{i}<=
  {{advance[{i}],address[{i*11}+:11],ce_n[{i}],pd_n[{i}],freeze[{i}],held[{i}],request[{i}],write[{i}],word_index[{i*2}+:2],state_data[{i*18}+:18]}};
 wire step_{i},ce_{i},pd_{i},freeze_{i},held_{i},request_{i},write_{i};
 wire [10:0] address_{i};wire [1:0] word_{i};wire [17:0] state_{i};
 assign {{step_{i},address_{i},ce_{i},pd_{i},freeze_{i},held_{i},request_{i},write_{i},word_{i},state_{i}}}=launch_{i};
 wire [7:0] q_{i};wire [17:0] result_{i};wire frozen_{i},done_{i},error_{i};
 always @(negedge host_clk)
  {{q[{i*8}+:8],frozen[{i}],done[{i}],error[{i}],result[{i*18}+:18]}}<=
  {{q_{i},frozen_{i},done_{i},error_{i},result_{i}}};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 cell_i2716_snapshot #(.INIT_FILE("{image}")) rom_{pkg}(
 .state_host_clk(host_clk),.state_advance(step_{i}),.state_freeze(freeze_{i}),.state_guest_held(held_{i}),
 .state_request(request_{i}),.state_write(write_{i}),.state_index(word_{i}),.state_data(state_{i}),
 .state_frozen(frozen_{i}),.state_done(done_{i}),.state_error(error_{i}),.state_result(result_{i}),
`else
 assign {{frozen_{i},done_{i},error_{i},result_{i}}}=21'b0;
 cell_i2716 #(.INIT_FILE("{image}")) rom_{pkg}(
`endif
 .sys_clk(host_clk),.p19(address_{i}[10]),.p22(address_{i}[9]),.p23(address_{i}[8]),.p1(address_{i}[7]),.p2(address_{i}[6]),.p3(address_{i}[5]),
 .p4(address_{i}[4]),.p5(address_{i}[3]),.p6(address_{i}[2]),.p7(address_{i}[1]),.p8(address_{i}[0]),.p20(ce_{i}),.p18(pd_{i}),
 .p9(q_{i}[0]),.p10(q_{i}[1]),.p11(q_{i}[2]),.p13(q_{i}[3]),.p14(q_{i}[4]),.p15(q_{i}[5]),.p16(q_{i}[6]),.p17(q_{i}[7]),
 .p12(1'b0),.p21(1'b1),.p24(1'b1));
'''
 top+='endmodule\n'
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
set_global_assignment -name SYSTEMVERILOG_FILE cell_i2716_snapshot.sv
set_global_assignment -name VERILOG_FILE cell_i2716.v
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
 for port in ('advance','ce_n','pd_n','freeze','held','request','write','address','state_data','word_index','q','result','frozen','done','error'):qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'[*]\n'
 for variant in ('baseline','snapshot'):
  case=out/variant;case.mkdir();shutil.copyfile(gen.ROOT/'verilog/cells/cell_i2716.v',case/'cell_i2716.v')
  for p in assets.values():
   target=case/'verilog/proms/eprom'/p.name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,target)
  for name,text in {'top.sv':top,'cell_i2716_snapshot.sv':adapter,'bank.sdc':SDC,'audit.tcl':AUDIT,
    'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
    'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():(case/name).write_text(text)
 sources=[Path(__file__).resolve(),here/'generate_i2716.py',here/'generate_snapshot_ff.py',here/'fit_f10415.py',here/'fit_f10415_enable.py',gen.ROOT/'verilog/cells/cell_i2716.v']
 executed=out/'executed-sources';executed.mkdir()
 for p in sources:shutil.copyfile(p,executed/p.name)
 record=dict(scope='Eight actual2716socket configurations,4originalimages/4emptyINIT_FILE;72read pipeline bits,same50MHz negative-edge launch/capture shell/10ns embedded paths,2nsmax/0nsmin virtualIO,no cuts. Not whole BaseBoard/global barrier/physicalsnapshot.',
  manifest=manifest,sockets=list(gen.SOCKETS),image_hashes=gen.IMAGES,builder_sha256=digest(Path(__file__)),source_hashes={str(p.relative_to(gen.ROOT)):digest(p)for p in sources},
  compiled_files={str(p.relative_to(out)):digest(p)for p in out.rglob('*')if p.is_file()},expected_qsf={v:(out/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
 (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(out,quartus):
 record=json.loads((out/'provenance.json').read_text());assert digest(Path(__file__))==record['builder_sha256']
 for name,sha in record['compiled_files'].items():assert digest(out/name)==sha,'prepared input differs:'+name
 for variant in ('baseline','snapshot'):
  case=out/variant;assert not(case/'map.log').exists(),'inspect existing job;do not restart'
  for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
   print(variant,label,flush=True)
   with(case/(label+'.log')).open('w')as f:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=f,stderr=subprocess.STDOUT,check=True)
 print('Eight-socket fits terminal;independent inference/timing audit still required',flush=True)


if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare:prepare(a.prepare.resolve())
 elif a.run:run(a.run.resolve(),a.quartus)
 else:p.error('Specify --prepare or --run')
