#!/usr/bin/env python3
"""Fit matched isolated original/snapshot IM sub-banks on the MiSTer device.

Run on misterubuntu with Quartus 17.0 installed. This creates new projects;
it never programs an FPGA or changes a production build. Virtual data pins
model a bank embedded inside the core; no board pin timing is qualified here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
TOP = '''module snapshot_fit(
 input wire clk, input wire [9:0] address,
 input wire [33:0] data,input wire ce_n,we_n,
 output wire [33:0] q,
 input wire freeze,request,write,
 input wire [10:0] word_index,input wire [33:0] state_data,
 output wire frozen,done,error,output wire [33:0] result);
`ifdef SNAPSHOT_BANK_EXPERIMENT
 dorado_snapshot_f10415_bank bank(clk,address,data,ce_n,we_n,q,
   freeze,frozen,request,write,word_index,state_data,done,error,result);
`else
 assign frozen=0;assign done=0;assign error=0;assign result=0;
 genvar i;
 generate for(i=0;i<34;i=i+1)begin: original
   cell_F10415A bit_cell(clk,address[9],address[8],address[7],address[6],
     address[5],address[4],address[3],address[2],address[1],address[0],
     data[i],ce_n,we_n,q[i]);
 end endgenerate
`endif
endmodule
'''
SDC = '''create_clock -name bank_clock -period 20.000 [get_ports clk]
derive_clock_uncertainty
set_input_delay -clock bank_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports clk]]
set_input_delay -clock bank_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports clk]]
set_output_delay -clock bank_clock -max 2.000 [all_outputs]
set_output_delay -clock bank_clock -min 0.000 [all_outputs]
'''
AUDIT = '''project_open snapshot_fit
create_timing_netlist -model slow
set corners {}
foreach_in_collection op [get_available_operating_conditions] {
 lappend corners [get_operating_conditions_info $op -name]
}
delete_timing_netlist
if {[llength $corners]!=4} {error "Expected four operating conditions: $corners"}
foreach name $corners {
 create_timing_netlist
 set_operating_conditions $name
 read_sdc
 update_timing_netlist
 puts "AUDIT_DEVICE_CORNER $name"
 foreach mode {setup hold recovery removal} {
  report_timing -$mode -npaths 3 -detail full_path -file bank-$mode-$name.rpt
 }
 report_min_pulse_width -file bank-mpw-$name.rpt
 report_clocks -file bank-clocks-$name.rpt
 report_ucp -file bank-ucp-$name.rpt
 delete_timing_netlist
}
project_close
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--quartus', type=Path,
                        default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(exist_ok=False, parents=True)
    sources = [HERE/'f10415_bank.sv', ROOT/'verilog/cells/cell_F10415A.v']
    hashes = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in [*sources, Path(__file__).resolve()]}
    (out/'provenance.json').write_text(json.dumps(dict(
        source_hashes=hashes,
        scope='Isolated 34-slice bank; virtual data I/O, 50MHz and 2ns I/O budgets; not a full Dorado board/core or physical deployment'), indent=2)+'\n')
    for variant in ('baseline', 'snapshot'):
        case = out/variant
        case.mkdir()
        for p in sources:
            shutil.copyfile(p, case/p.name)
        (case/'top.sv').write_text(TOP)
        (case/'bank.sdc').write_text(SDC)
        (case/'audit.tcl').write_text(AUDIT)
        (case/'snapshot_fit.qpf').write_text('PROJECT_REVISION = "snapshot_fit"\n')
        assignments = '''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE f10415_bank.sv
set_global_assignment -name VERILOG_FILE cell_F10415A.v
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to clk
'''
        for port in ('address[*]', 'data[*]', 'ce_n', 'we_n', 'q[*]', 'freeze',
                     'request', 'write', 'word_index[*]', 'state_data[*]',
                     'frozen', 'done', 'error', 'result[*]'):
            assignments += 'set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        if variant == 'snapshot':
            assignments += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(assignments)
        for tool, arguments, label in (
            ('quartus_map', ['snapshot_fit'], 'map'),
            ('quartus_fit', ['snapshot_fit'], 'fit'),
            ('quartus_sta', ['snapshot_fit'], 'sta'),
            ('quartus_sta', ['-t', 'audit.tcl'], 'corners'),
        ):
            print(variant, label, flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(args.quartus/tool), *arguments], cwd=case,
                               stdout=log, stderr=subprocess.STDOUT, check=True)
    print('Fits complete; inspect inference, resource delta and all-corner reports before qualification.', flush=True)


if __name__ == '__main__':
    main()
