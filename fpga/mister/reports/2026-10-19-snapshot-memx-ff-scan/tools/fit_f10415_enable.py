#!/usr/bin/env python3
"""Fit the18-bit explicit-enable prototype; no FPGA programming or production edits.

One50MHz host clock and a virtual guest_advance input qualify RAM inference
and routing only. The baseline has all original cells clocked continuously;
the candidate additionally gates reads/writes with the external guest enable.
A matched opposite-edge launch/capture shell adds real clock insertion on
embedded data paths and10ns internal budgets; resource counts include it.
The separately held-reference RTL test qualifies backend-pause equivalence.
No full board/clock distribution/barrier is qualified by this isolated fit.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

from fit_f10415 import AUDIT

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
TOP = '''module snapshot_fit(
 input wire host_clk,guest_advance,input wire [9:0] address,
 input wire [17:0] data,input wire ce_n,we_n,
 output reg [17:0] q,
 input wire freeze,guest_held,request,write,
 input wire [10:0] word_index,input wire [17:0] state_data,
 output reg frozen,done,error,output reg [17:0] result);
 // A real launch/capture shell models an embedded bank. The opposite edge
 // gives BOTH variants10ns register-to-bank and bank-to-register budgets;
 // external virtual I/O retains the same2ns/0ns delay contract. Resource
 // counts include this shell; unused baseline admin registers may be pruned.
 reg [63:0] launch;
 always @(negedge host_clk)
   launch <= {guest_advance,address,data,ce_n,we_n,freeze,guest_held,
              request,write,word_index,state_data};
 wire step_r,ce_r,we_r,freeze_r,held_r,request_r,write_r;
 wire [9:0] address_r;wire [17:0] data_r,state_r;wire [10:0] word_r;
 assign {step_r,address_r,data_r,ce_r,we_r,freeze_r,held_r,
         request_r,write_r,word_r,state_r}=launch;
 wire [17:0] bank_q,bank_result;wire bank_frozen,bank_done,bank_error;
 always @(negedge host_clk)
   {q,frozen,done,error,result} <= {bank_q,bank_frozen,bank_done,bank_error,bank_result};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 dorado_snapshot_f10415_enable_bank bank(host_clk,step_r,address_r,data_r,
   ce_r,we_r,bank_q,freeze_r,held_r,bank_frozen,request_r,write_r,word_r,state_r,
   bank_done,bank_error,bank_result);
`else
 assign bank_frozen=0;assign bank_done=0;assign bank_error=0;assign bank_result=0;
 genvar i;
 generate for(i=0;i<18;i=i+1)begin: original
   cell_F10415A bit_cell(host_clk,address_r[9],address_r[8],address_r[7],address_r[6],
     address_r[5],address_r[4],address_r[3],address_r[2],address_r[1],address_r[0],
     data_r[i],ce_r,we_r,bank_q[i]);
 end endgenerate
`endif
endmodule
'''
SDC = '''create_clock -name host_clock -period 20.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_output_delay -clock host_clock -max 2.000 [all_outputs]
set_output_delay -clock host_clock -min 0.000 [all_outputs]
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--quartus', type=Path,
                        default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(exist_ok=False, parents=True)
    sources = [HERE/'f10415_enable_bank.sv', ROOT/'verilog/cells/cell_F10415A.v']
    files = [*sources, Path(__file__).resolve(), HERE/'fit_f10415.py']
    (out/'provenance.json').write_text(json.dumps(dict(
        source_hashes={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()for p in files},
        scope='Isolated18-bit clock-enable bank with matched opposite-edge launch/capture shell,50MHz,2ns/0ns virtual I/O and10ns internal budgets; counts include shell; baseline full-advance original cells; no board/barrier/physical deployment'), indent=2)+'\n')
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
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE f10415_enable_bank.sv
set_global_assignment -name VERILOG_FILE cell_F10415A.v
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for port in ('address[*]', 'data[*]', 'ce_n', 'we_n', 'q[*]', 'freeze',
                     'guest_held', 'guest_advance', 'request', 'write', 'word_index[*]', 'state_data[*]',
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
            with (case/(label+'.log')).open('w')as log:
                subprocess.run([str(args.quartus/tool), *arguments], cwd=case,
                               stdout=log, stderr=subprocess.STDOUT, check=True)
    print('Fits completed; audit geometry/port modes, resources and all corners before qualification.', flush=True)


if __name__ == '__main__':
    main()
