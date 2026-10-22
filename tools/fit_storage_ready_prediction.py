#!/usr/bin/env python3
"""Fit one complete MSA ready/address/gate cone at15ns on Cyclone V.

Actual original board logic and the simulation-qualified predictor are used.
Host service inputs are registered virtual pins, not a physical SDRAM backend.
The global clock gate matches the native core. No timing exceptions, assembly
or programming are provided. Full128-group/native timing/firmware remain open.
"""
import argparse,hashlib,json,os,re,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(b):return hashlib.sha256(b).hexdigest()
QSF='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY ready_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name NUM_PARALLEL_PROCESSORS 1
set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_RETIMING OFF
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name GENERATE_RBF_FILE OFF
set_global_assignment -name SDC_FILE clock.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
set_instance_assignment -name VIRTUAL_PIN ON -to pins[*]
set_instance_assignment -name VIRTUAL_PIN ON -to result[*]
'''
SDC='''create_clock -name host_clock -period15.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max2.000 [get_ports {pins[*]}]
set_input_delay -clock host_clock -min0.000 [get_ports {pins[*]}]
set_output_delay -clock host_clock -max2.000 [get_ports {result[*]}]
set_output_delay -clock host_clock -min0.000 [get_ports {result[*]}]
'''.replace('-period15','-period 15').replace('-max2','-max 2').replace('-min0','-min 0')
AUDIT='''project_open ready_fit
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {lappend corners [get_operating_conditions_info $op -name]}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions: $corners"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 puts "READY_CORNER $name"
 set predicted [get_registers {*|ready_q*}]
 set step [get_registers {*machine_step*}]
 if {[get_collection_size $predicted]<16 || [get_collection_size $step]<1} {error "Missing ready/step state"}
 puts "READY_REGISTERS $name [get_collection_size $predicted]"
 foreach_in_collection r $predicted {puts "READY_REGISTER [get_node_info -name $r]"}
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths64 -detail full_path -file ready-$mode-$name.rpt
 }
 foreach mode {setup hold} {
  report_timing -$mode -to $predicted -npaths64 -detail full_path -file ready-predict-$mode-$name.rpt
  report_timing -$mode -from $predicted -to $step -npaths64 -detail full_path -file ready-gate-$mode-$name.rpt
  report_timing -$mode -from $step -to $predicted -npaths64 -detail full_path -file ready-step-$mode-$name.rpt
 }
 report_min_pulse_width -file ready-mpw-$name.rpt
 report_clocks -file ready-clocks-$name.rpt
 report_ucp -file ready-ucp-$name.rpt
 delete_timing_netlist
}
project_close
'''.replace('-npaths64','-npaths 64')
def prepare(component,out):
 assert not out.exists()
 p=json.loads((component/'provenance.json').read_text());s=json.loads((component/'run-state.json').read_text())
 assert s['status']=='PASS_POST_EDGE_READY_ORIGINAL_MSA_COMPONENT' and len(s['actions'])==8
 for n,h in p['inputs'].items():assert sha((component/'inputs'/n).read_bytes())==h,n
 out.mkdir(parents=True)
 inputs=component/'inputs'
 for name in ['msa_ddram.v','dorado_storage_group.sv','addr_preview.sv']:(out/name).write_bytes((inputs/name).read_bytes())
 shutil.copytree(inputs/'cells',out/'cells')
 header=(out/'msa_ddram.v').read_text().split('module msa_ddram',1)[1].split(');',1)[0].split(') (',1)[1]
 ports=[]
 for m in re.finditer(r'\b(input|output)\s+wire\s*(\[[^]]+\])?\s*([^\n]+)',header):
  width=1 if not m[2] else int(re.fullmatch(r'\[(\d+):0\]',m[2])[1])+1
  for name in m[3].rstrip().rstrip(',').split(','):
   ports.append((m[1],name.strip(),width))
 assert len({n for _,n,_ in ports})==len(ports)
 special={'sys_clk':'machine_clk','storage_clk':'host_clk','storage_reset':'reset','storage_step':'machine_step','storage_ready':'board_ready'}
 ni=sum(w for d,n,w in ports if d=='input'and n not in special)+2
 no=sum(w for d,n,w in ports if d=='output'and n not in special)
 wiring=[];wires=[];outputs=[];offset=0
 for direction,name,width in ports:
  if name in special:wiring.append('.'+name+'('+special[name]+')');continue
  if direction=='input':
   wiring.append('.'+name+'(launch['+str(offset)+'+:'+str(width)+'])');offset+=width
  else:
   wires.append('wire ['+str(width-1)+':0] '+name+';');wiring.append('.'+name+'('+name+')');outputs.append(name)
 assert offset==ni-2
 top='''module ready_fit(input wire host_clk,input wire [INPUT_LAST:0] pins,
 output wire [OUTPUT_LAST:0] result);
 reg [INPUT_LAST:0] launch;
 always @(posedge host_clk)launch<=pins;
 wire reset=launch[RESET_BIT],requested=launch[REQUEST_BIT];
 wire board_ready,machine_clk;
 wire enable=board_ready && requested && !reset;
 reg machine_step=0,ready_sample=0;
 always @(negedge host_clk)begin machine_step<=enable;ready_sample<=board_ready;end
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),
  .intended_device_family("Cyclone V")) machine_clock(
  .inclk({3'b000,host_clk}),.clkselect(2'b00),.ena(enable),.outclk(machine_clk));
WIRES
 msa_ddram #(.SYSPER(4)) u_msa(PORTS);
 reg [DATA_LAST:0] data_result;
 always @(posedge host_clk)data_result<={OUTPUTS};
 assign result={ready_sample,machine_step,board_ready,data_result};
endmodule
'''
 for k,v in dict(INPUT_LAST=ni-1,OUTPUT_LAST=no+2,RESET_BIT=ni-2,REQUEST_BIT=ni-1,DATA_LAST=no-1).items():top=top.replace(k,str(v))
 top=top.replace('WIRES','\n'.join(wires)).replace('PORTS',',\n'.join(wiring)).replace('OUTPUTS',','.join(outputs))
 (out/'top.sv').write_text(top);(out/'clock.sdc').write_text(SDC);(out/'audit.tcl').write_text(AUDIT)
 (out/'ready_fit.qpf').write_text('PROJECT_REVISION = "ready_fit"\n')
 files=['top.sv','msa_ddram.v','dorado_storage_group.sv','addr_preview.sv']+[str(x.relative_to(out))for x in sorted((out/'cells').iterdir())if x.suffix in ['.v','.sv']]
 (out/'ready_fit.qsf').write_text(QSF+''.join('set_global_assignment -name SYSTEMVERILOG_FILE '+x+'\n'for x in files))
 (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 q=dict(scope=__doc__,component_provenance_sha256=sha((component/'provenance.json').read_bytes()),component_state_sha256=sha((component/'run-state.json').read_bytes()),group_count=16,period_ns=15,virtual_input_bits=ni,virtual_output_bits=no+3,files={str(f.relative_to(out)):sha(f.read_bytes())for f in out.rglob('*')if f.is_file()},assembly_permitted=False,native_qualified=False)
 (out/'provenance.json').write_text(json.dumps(q,indent=2)+'\n');print('PREPARED_READY_FIT',ni,no+3,flush=True)

def run(project,quartus):
 p=json.loads((project/'provenance.json').read_text())
 for n,h in p['files'].items():assert sha((project/n).read_bytes())==h,n
 assert not(project/'run-state.json').exists()
 s=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),actions=[],assembly_permitted=False)
 def save():(project/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
 save()
 try:
  for tool,args,label in [('quartus_map',['ready_fit'],'map'),('quartus_fit',['ready_fit'],'fit'),('quartus_sta',['ready_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'audit')]:
   with(project/(label+'.log')).open('x')as log:
    child=subprocess.Popen([str(quartus/tool),*args],cwd=project,stdout=log,stderr=subprocess.STDOUT);r=dict(stage=label,pid=child.pid,started=time.time());s['actions'].append(r);save();code=child.wait();r.update(returncode=code,finished=time.time());save();assert code==0,(label,code)
  s.update(status='TERMINAL_READY_TIMING_REVIEW_REQUIRED',finished=time.time());save();print(s['status'],flush=True)
 except BaseException:s.update(status='TERMINAL_FAILED',finished=time.time());save();raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);g=p.add_mutually_exclusive_group(required=True);g.add_argument('--prepare',type=Path);g.add_argument('--run',type=Path);p.add_argument('--component',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
 if a.prepare:assert a.component;prepare(a.component.resolve(),a.prepare.resolve())
 else:run(a.run.resolve(),a.quartus.resolve())
