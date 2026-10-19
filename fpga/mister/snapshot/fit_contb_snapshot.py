#!/usr/bin/env python3
"""Prepare isolated matched ContB fits; never edit/program a production core.

Run --prepare locally, copy the new directory to misterubuntu, then run its
run-fits.py. Both boards use the original falling-edge-latched global clock
gate. The candidate RAM additionally uses an identically latched host enable.
No timing exception or whole-machine qualification is implied.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

import audit_contb_im as im
import generate_contb as gen
from fit_f10415 import AUDIT
from test_contb_snapshot import fixture

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
SDC = '''create_clock -name host_clock -period 20.000 [get_ports host_clk]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports host_clk]]
set_output_delay -clock host_clock -max 2.000 [all_outputs]
set_output_delay -clock host_clock -min 0.000 [all_outputs]
'''


def top(ports):
    inputs, outputs = ports['inputs'], ports['outputs']
    code = [f'''module snapshot_fit(
 input wire host_clk,run,input wire [{len(inputs)-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [13:0] state_index,
 input wire [17:0] state_data,
 output reg [{len(outputs)-1}:0] board_outputs,
 output reg frozen,done,error,output reg [17:0] result);
 reg run_r;
 always @(posedge host_clk) run_r <= run;
 reg enabled;
 always @(negedge host_clk) enabled <= run_r;
 wire guest_clk;
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),
              .intended_device_family("Cyclone V")) machine_clock
   (.inclk({{3'b000,host_clk}}),.clkselect(2'b00),.ena(run_r),.outclk(guest_clk));
 reg [{len(inputs)-1}:0] pins;
 reg [35:0] admin;
 always @(negedge host_clk) begin
   pins <= stimulus;
   admin <= {{freeze,held,request,write,state_index,state_data}};
 end
 wire freeze_r,held_r,request_r,write_r;
 wire [13:0] index_r;wire [17:0] data_r;
 assign {{freeze_r,held_r,request_r,write_r,index_r,data_r}}=admin;
 wire bank_frozen,bank_done,bank_error;wire [17:0] bank_result;
 always @(negedge host_clk)
   {{frozen,done,error,result}} <= {{bank_frozen,bank_done,bank_error,bank_result}};''']
    code += [f' wire {n};' for n in outputs]
    code += [' always @(negedge host_clk) board_outputs <= {'+
             ', '.join(reversed(outputs))+'};',
             '`ifdef SNAPSHOT_BANK_EXPERIMENT',
             f' {gen.MODULE} #(.SYSPER(4)) board (',
             ' .state_host_clk(host_clk),.state_guest_advance(enabled),',
             ' .state_freeze(freeze_r),.state_guest_held(held_r),',
             ' .state_frozen(bank_frozen),.state_request(request_r),.state_write(write_r),',
             ' .state_index(index_r),.state_data(data_r),.state_done(bank_done),',
             ' .state_error(bank_error),.state_result(bank_result),',
             '`else',
             ' assign bank_frozen=0;assign bank_done=0;assign bank_error=0;assign bank_result=0;',
             ' ContB_m_Rev_m_Cd #(.SYSPER(4)) board (',
             '`endif']
    connections = ['.sys_clk(guest_clk)']
    for i, n in enumerate(inputs):
        feedback = ' | '+n+'__drv' if n+'__drv' in outputs else ''
        connections.append(f'.{n}(pins[{i}]{feedback})')
    connections += [f'.{n}({n})' for n in outputs]
    code += [',\n'.join(' '+c for c in connections), ' );', 'endmodule', '']
    return '\n'.join(code)


def prepare(out):
    out.mkdir(parents=True, exist_ok=False)
    board, manifest = gen.generate()
    _, _, ports = fixture(manifest)
    kinds = {k for k, _, _ in im.instances(im.GENERATED.read_text())}
    sources = [im.GENERATED, HERE/'f10415_enable_bank.sv',
               *(ROOT/'verilog/cells'/f'{k}.v' for k in sorted(kinds))]
    tools = [Path(__file__), HERE/'generate_contb.py', HERE/'audit_contb_im.py',
             HERE/'test_contb_snapshot.py', HERE/'fit_f10415.py']
    hashes = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
              for p in [*sources, *tools]}
    (out/'provenance.json').write_text(json.dumps(dict(
        scope='Actual isolated ContB, original clock gate plus host enabled RAM, matched registered I/O shell,50MHz/2ns/0ns I/O; no whole-core qualification or FPGA programming',
        source_hashes=hashes, manifest=manifest), indent=2)+'\n')
    for variant in ('baseline', 'snapshot'):
        case = out/variant
        case.mkdir()
        for p in sources:
            shutil.copyfile(p, case/p.name)
        (case/'ContB_snapshot.sv').write_text(board)
        (case/'top.sv').write_text(top(ports))
        (case/'bank.sdc').write_text(SDC)
        (case/'audit.tcl').write_text(AUDIT)
        (case/'snapshot_fit.qpf').write_text('PROJECT_REVISION = "snapshot_fit"\n')
        qsf = '''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for filename in ('top.sv', 'ContB_snapshot.sv', *(p.name for p in sources)):
            qsf += f'set_global_assignment -name SYSTEMVERILOG_FILE {filename}\n'
        for port in ('run', 'stimulus[*]', 'freeze', 'held', 'request', 'write',
                     'state_index[*]', 'state_data[*]', 'board_outputs[*]',
                     'frozen', 'done', 'error', 'result[*]'):
            qsf += f'set_instance_assignment -name VIRTUAL_PIN ON -to {port}\n'
        if variant == 'snapshot':
            qsf += 'set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    (out/'run-fits.py').write_text('''import subprocess
from pathlib import Path
root=Path(__file__).resolve().parent
quartus=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin')
for variant in ('baseline','snapshot'):
 for tool,args,label in (('quartus_map',['snapshot_fit'],'map'),
                         ('quartus_fit',['snapshot_fit'],'fit'),
                         ('quartus_sta',['snapshot_fit'],'sta'),
                         ('quartus_sta',['-t','audit.tcl'],'corners')):
  print(variant,label,flush=True)
  with (root/variant/(label+'.log')).open('w')as log:
   subprocess.run([str(quartus/tool),*args],cwd=root/variant,
                  stdout=log,stderr=subprocess.STDOUT,check=True)
print('ContB fits terminal; inspect all evidence before qualification.',flush=True)
''')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare', type=Path, required=True)
    args = parser.parse_args()
    prepare(args.prepare.resolve())
