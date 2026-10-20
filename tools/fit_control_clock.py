#!/usr/bin/env python3
"""Measure the coupled control evaluator in a registered Cyclone V shell.

Single 15 ns clock, no timing exceptions, virtual data I/O. This checks the
combinational implementation budget only; it is not a native timing/boot test
and cannot establish Dorado speed. Run Quartus only on misterubuntu.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
TOP='''module clock_fit(input wire host_clk,input wire [51:0] pins,
 output reg [50:0] result);
 // Fixture input staging avoids treating a virtual package pin as the
 // evaluator's launch register. The evaluator still has a full 15ns path
 // from launch to result; the new staging path has a stricter half period.
 reg [51:0] input_stage;
 reg [51:0] launch;
 always @(negedge host_clk) input_stage<=pins;
 always @(posedge host_clk) launch<=input_stage;
 wire [31:0] q,next_q;
 wire before_clock,after_clock;
 wire [8:0] before_controls,after_controls,events,repeated;
 wire unsettled;
 assign {q,before_clock,after_clock,before_controls,after_controls}=launch;
 dorado_control_clock_step evaluator(q,before_clock,after_clock,
   before_controls,after_controls,next_q,events,repeated,unsettled);
 always @(posedge host_clk) result<={next_q,events,repeated,unsettled};
endmodule
'''
SDC='''create_clock -name host_clock -period 15.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [get_ports {pins[*]}]
set_input_delay -clock host_clock -min 0.000 [get_ports {pins[*]}]
set_output_delay -clock host_clock -max 2.000 [get_ports {result[*]}]
set_output_delay -clock host_clock -min 0.000 [get_ports {result[*]}]
'''
QSF='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY clock_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name NUM_PARALLEL_PROCESSORS 1
set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_RETIMING OFF
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name GENERATE_RBF_FILE OFF
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE dorado_control_clock_step.sv
set_global_assignment -name SDC_FILE clock.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
set_instance_assignment -name VIRTUAL_PIN ON -to pins[*]
set_instance_assignment -name VIRTUAL_PIN ON -to result[*]
'''
AUDIT='''project_open clock_fit
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four conditions: $corners"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 puts "CONTROL_CLOCK_CORNER $name"
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths 10 -detail full_path -file clock-$mode-$name.rpt
 }
 set launches [get_registers {*launch*}]
 set results [get_registers {*result*}]
 if {[get_collection_size $launches]==0 || [get_collection_size $results]==0} {error "Missing launch/result register coverage"}
 puts "CONTROL_CLOCK_REGISTER_COVERAGE launch=[get_collection_size $launches] result=[get_collection_size $results]"
 report_timing -setup -from $launches -to $results -npaths 10 -detail full_path -file clock-evaluator-$name.rpt
 report_min_pulse_width -file clock-mpw-$name.rpt
 report_clocks -file clock-clocks-$name.rpt
 report_ucp -file clock-ucp-$name.rpt
 delete_timing_netlist
}
project_close
'''


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(out):
    out.mkdir(parents=True,exist_ok=False)
    files={'top.sv':TOP,'clock.sdc':SDC,'clock_fit.qsf':QSF,
           'clock_fit.qpf':'PROJECT_REVISION = "clock_fit"\n','audit.tcl':AUDIT}
    for name,raw in files.items():(out/name).write_text(raw)
    (out/'dorado_control_clock_step.sv').write_bytes((ROOT/'verilog/rtl/dorado_control_clock_step.sv').read_bytes())
    (out/'fit_control_clock.py').write_bytes(Path(__file__).read_bytes())
    record=dict(scope=__doc__,clock_period_ns=15,input_staging_bits=52,launch_bits=52,result_bits=51,
                files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                assembly_permitted=False,native_qualified=False)
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')
    print('Prepared 15ns registered evaluator shell; no assembly/deploy mode',flush=True)


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text())
    assert not record['assembly_permitted']and not record['native_qualified']
    for name,digest in record['files'].items():assert sha(out/name)==digest,name
    assert sha(Path(__file__))==record['files']['fit_control_clock.py']
    status=out/'run-state.json';assert not status.exists(),'Observe the existing job; do not restart'
    state=dict(builder_pid=os.getpid(),status='RUNNING',stages=[])
    def write():status.write_text(json.dumps(state,indent=2)+'\n')
    write()
    try:
        for tool,args,label in [('quartus_map',['--read_settings_files=on','--write_settings_files=off','clock_fit'],'map'),
                                ('quartus_fit',['--read_settings_files=on','--write_settings_files=off','clock_fit'],'fit'),
                                ('quartus_sta',['clock_fit'],'sta'),
                                ('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('CONTROL_CLOCK_TIMING',label,flush=True)
            with(out/(label+'.log')).open('x')as log:
                process=subprocess.Popen([str(quartus/tool),*args],cwd=out,stdout=log,stderr=subprocess.STDOUT)
                row=dict(stage=label,pid=process.pid,started=time.time());state['stages'].append(row);write()
                code=process.wait();row.update(returncode=code,finished=time.time());write()
                if code:raise RuntimeError(label+' failed')
        state['status']='TERMINAL_REVIEW_REQUIRED';write()
        print('CONTROL_CLOCK_TIMING terminal; independent area/timing review required',flush=True)
    except BaseException:
        state['status']='TERMINAL_FAILED';write();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);a=p.add_mutually_exclusive_group(required=True)
    a.add_argument('--prepare',type=Path);a.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=p.parse_args()
    if args.prepare:prepare(args.prepare.resolve())
    else:run(args.run.resolve(),args.quartus.resolve())
