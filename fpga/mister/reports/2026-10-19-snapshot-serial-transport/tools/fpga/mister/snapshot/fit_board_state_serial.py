#!/usr/bin/env python3
"""Matched isolated old/serial board transport timing and area, no core build."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE=Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def shell(serial):
    text='''module snapshot_fit(input wire host_clk,input wire [279:0] pins,output reg [167:0] outputs);
 reg [279:0] input_stage,launch;
 always @(negedge host_clk)input_stage<=pins;
 always @(posedge host_clk)launch<=input_stage;
 wire system_reset,transport_reset,local_held;
 wire [4:0] services_held;
 wire [10:0] board_frozen,board_done,board_error,board_hold_required,board_fault;
 wire [197:0] board_result;wire [18:0] master;
 assign {system_reset,transport_reset,local_held,services_held,board_frozen,board_done,board_error,board_hold_required,board_fault,board_result,master}=launch;
 wire [35:0] bus;assign bus[35:33]=master[18:16];assign bus[31:16]=master[15:0];
 wire hold,command_hold,scan_abort,hold_update,hold_value,control_error;
 wire request,write,ready,done,fault,busy,complete;wire [3:0] board;wire [23:0] index;
 wire [17:0] data,result;wire [2:0] error;wire [10:0] board_request;
 wire [31:0] txn_sequence,control_errors;
 always @(posedge host_clk)outputs<={bus[32],bus[15:0],hold,command_hold,scan_abort,ready,board_request,busy,complete,txn_sequence,control_errors,fault,done,error,result,write,board,index,data};
'''
    if serial:
        text+=''' dorado_board_state_uio_serial #(.SCHEMA_ID(64'h79c5a16e42d38b07)) uio(
 .clk(host_clk),.system_reset(system_reset),.transport_reset(transport_reset),.bus(bus),
 .hold_update(hold_update),.hold_value(hold_value),.hold(hold),.command_hold(command_hold),
 .local_held(local_held),.services_held(services_held),.control_error(control_error),.board_frozen(board_frozen),
 .request(request),.write(write),.board(board),.index(index),.data(data),
 .ready(ready),.done(done),.error(error),.result(result),.fault(fault),
 .busy(busy),.complete(complete),.txn_sequence(txn_sequence),.control_errors(control_errors));
 dorado_board_state_fabric_serial fabric(
 .clk(host_clk),.system_reset(system_reset),.transport_reset(transport_reset),
 .hold_update(hold_update),.hold_value(hold_value),.local_held(local_held),.services_held(services_held),
 .hold(hold),.scan_abort(scan_abort),.command_hold(command_hold),.control_error(control_error),
 .request(request),.write(write),.board(board),.index(index),.data(data),
 .ready(ready),.done(done),.error(error),.result(result),.fault(fault),
 .board_frozen(board_frozen),.board_done(board_done),.board_error(board_error),
 .board_hold_required(board_hold_required),.board_fault(board_fault),.board_result(board_result),.board_request(board_request));
'''
    else:
        text+=''' assign command_hold=hold;assign scan_abort=fault;assign {hold_update,hold_value,control_error}=3'b0;
 dorado_board_state_uio uio(host_clk,system_reset||transport_reset,bus,hold,local_held,board_frozen,
 request,write,board,index,data,ready,done,error,result,fault,busy,complete,txn_sequence,control_errors);
 dorado_board_state_fabric fabric(host_clk,system_reset||transport_reset,hold,local_held,
 request,write,board,index,data,ready,done,error,result,fault,board_frozen,board_done,board_error,board_result,board_request);
'''
    return text+'endmodule\n'


def prepare(out):
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    out.mkdir(parents=True,exist_ok=False)
    files=['board_state_fabric.sv','board_state_uio.sv','board_state_fabric_serial.sv','board_state_uio_serial.sv']
    for variant in ('baseline','snapshot'):
        case=out/variant;case.mkdir()
        for n in files:shutil.copyfile(HERE/n,case/n)
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
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
set_instance_assignment -name VIRTUAL_PIN ON -to pins[*]
set_instance_assignment -name VIRTUAL_PIN ON -to outputs[*]
'''
        for n in files:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+n+'\n'
        for n,s in {'top.sv':shell(variant=='snapshot'),'bank.sdc':SDC,'audit.tcl':AUDIT,
                    'snapshot_fit.qsf':qsf,'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n'}.items():
            (case/n).write_text(s)
    sources=[Path(__file__),HERE/'fit_f10415.py',HERE/'fit_f10415_enable.py']+[HERE/n for n in files]
    for p in sources:
        q=out/'executed-sources'/p.name;q.parent.mkdir(exist_ok=True);shutil.copyfile(p,q)
    record=dict(scope=__doc__,driver_sha256=sha(Path(__file__)),
        source_hashes={str(p):sha(p) for p in sources},
        compiled_files={str(p.relative_to(out)):sha(p) for p in out.rglob('*') if p.is_file()},
        shell='280 staged input bits,168 captured output bits; original50MHz/20ns/2nsmax0nsminSDC; no cuts.',
        limits='Isolated transport only; services are input contracts, no native pause implementation or snapshot/core capacity claim.')
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text());assert sha(Path(__file__))==record['driver_sha256']
    for n,h in record['compiled_files'].items():assert sha(out/n)==h,'prepared source differs: '+n
    for variant in ('baseline','snapshot'):
        case=out/variant;assert not(case/'map.log').exists(),'inspect existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print('Serial transport',variant,label,flush=True)
            with(case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Serial transport fits terminal; independent audit required.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
