#!/usr/bin/env python3
"""Fit the18-bit dual-port prototype; no FPGA programming or production edits.

Two in-phase50MHz clock inputs model the host and its held guest clock. This
is an isolated virtual-I/O inference/timing experiment, not a clock-distribution
or CDC qualification of a complete machine barrier.
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
 input wire guest_clk,host_clk,input wire [9:0] address,
 input wire [17:0] data,input wire ce_n,we_n,
 output wire [17:0] q,
 input wire freeze,guest_held,request,write,
 input wire [10:0] word_index,input wire [17:0] state_data,
 output wire frozen,done,error,output wire [17:0] result);
`ifdef SNAPSHOT_BANK_EXPERIMENT
 dorado_snapshot_f10415_dual_bank bank(guest_clk,host_clk,address,data,
   ce_n,we_n,q,freeze,guest_held,frozen,request,write,word_index,state_data,
   done,error,result);
`else
 assign frozen=0;assign done=0;assign error=0;assign result=0;
 genvar i;
 generate for(i=0;i<18;i=i+1)begin: original
   cell_F10415A bit_cell(guest_clk,address[9],address[8],address[7],address[6],
     address[5],address[4],address[3],address[2],address[1],address[0],
     data[i],ce_n,we_n,q[i]);
 end endgenerate
`endif
endmodule
'''
SDC = '''create_clock -name guest_clock -period 20.000 [get_ports guest_clk]
create_clock -name host_clock -period 20.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports {guest_clk host_clk}]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports {guest_clk host_clk}]]
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
    sources = [HERE/'f10415_dual_bank.sv', ROOT/'verilog/cells/cell_F10415A.v']
    files = [*sources, Path(__file__).resolve(), HERE/'fit_f10415.py']
    (out/'provenance.json').write_text(json.dumps(dict(
        source_hashes={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()for p in files},
        scope='Isolated18-bit dual-port bank, two in-phase50MHz clocks and2ns virtual I/O budgets; not a full board/clock barrier/physical deployment'), indent=2)+'\n')
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
set_global_assignment -name SYSTEMVERILOG_FILE f10415_dual_bank.sv
set_global_assignment -name VERILOG_FILE cell_F10415A.v
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to guest_clk
set_location_assignment PIN_Y13 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to guest_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for port in ('address[*]', 'data[*]', 'ce_n', 'we_n', 'q[*]', 'freeze',
                     'guest_held', 'request', 'write', 'word_index[*]', 'state_data[*]',
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
