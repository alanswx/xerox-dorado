#!/usr/bin/env python3
"""Matched fully enabled native8/12 frontend fits: qualified serial vs factored.

Identical 50MHz registered shell, virtual IO budgets, device, fitter policy and
four operating conditions. No RAM-policy/clock/geometry change or deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(out):
    import generate_storage_factored as gen
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    original,reference=gen.base.generate();candidate,manifest=gen.generate()
    top='''module snapshot_fit(
 input wire host_clk,advance,hold,scan_step,serial_in,reset,
 input wire [1:0] ras_n,cas_n,we_n,req_ready,rsp_valid,
 input wire [31:0] addr,data,rsp_data,
 output reg [109:0] outputs);
 reg [110:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,hold,scan_step,serial_in,reset,ras_n,cas_n,we_n,req_ready,rsp_valid,addr,data,rsp_data};
 always @(posedge host_clk)launch<=input_stage;
 wire advance_r,hold_r,scan_r,serial_r,reset_r;
 wire [1:0] ras_r,cas_r,we_r,req_ready_r,rsp_valid_r;
 wire [31:0] addr_r,data_r,rsp_data_r;
 assign {advance_r,hold_r,scan_r,serial_r,reset_r,ras_r,cas_r,we_r,req_ready_r,rsp_valid_r,addr_r,data_r,rsp_data_r}=launch;
 wire [1:0] ready,req_valid,req_write,rsp_ready,frozen,drained,serial_out;
 wire [31:0] req_addr,req_data,data_out;
'''
    for i,width in enumerate((8,12)):
        top+=f''' dorado_storage_group_snapshot #(.ADDR_BITS(16),.WIDTH({width})) case{i}(
 .state_guest_advance(advance_r),.state_hold(hold_r),.state_scan_step(scan_r),.state_serial_in(serial_r),
 .state_serial_out(serial_out[{i}]),.state_frozen(frozen[{i}]),.state_drain_ready(drained[{i}]),
 .host_clk(host_clk),.machine_clk(host_clk),.reset(reset_r),.ras_n(ras_r[{i}]),.cas_n(cas_r[{i}]),.we_n(we_r[{i}]),
 .addr(addr_r[{16*i}+:16]),.data_in(data_r[{16*i}+:{width}]),.data_out(data_out[{16*i}+:{width}]),
 .ready(ready[{i}]),.req_valid(req_valid[{i}]),.req_write(req_write[{i}]),.req_addr(req_addr[{16*i}+:16]),
 .req_data(req_data[{16*i}+:16]),.req_ready(req_ready_r[{i}]),.rsp_valid(rsp_valid_r[{i}]),
 .rsp_ready(rsp_ready[{i}]),.rsp_data(rsp_data_r[{16*i}+:16]));
 assign data_out[{16*i+width}+:{16-width}]=0;
'''
    top+=''' always @(posedge host_clk)outputs<={ready,req_valid,req_write,rsp_ready,frozen,drained,serial_out,req_addr,req_data,data_out};
endmodule
'''
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name GENERATE_RBF_FILE OFF
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE storage_group_snapshot.sv
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for port in ('advance','hold','scan_step','serial_in','reset','ras_n[*]','cas_n[*]','we_n[*]','req_ready[*]','rsp_valid[*]','addr[*]','data[*]','rsp_data[*]','outputs[*]'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    out.mkdir(parents=True,exist_ok=False)
    for label,source in [('baseline',original),('factored',candidate)]:
        case=out/label;case.mkdir()
        for name,text in {'storage_group_snapshot.sv':source,'top.sv':top,'snapshot_fit.qsf':qsf,
                          'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n','bank.sdc':SDC,'audit.tcl':AUDIT}.items():(case/name).write_text(text)
    inputs=out/'executed-sources';inputs.mkdir()
    sources=[Path(__file__).resolve(),Path(gen.__file__),Path(gen.base.__file__),gen.base.ROOT/'fpga/mister/snapshot/fit_f10415.py',gen.base.ROOT/'fpga/mister/snapshot/fit_f10415_enable.py']
    for p in sources:shutil.copyfile(p,inputs/p.name)
    record=dict(scope=__doc__,reference_manifest=reference,candidate_manifest=manifest,builder_sha256=sha(Path(__file__)),
                compiled_files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
                parameters=dict(ADDR_BITS=16,WIDTH=[8,12]),physical_bits=234,private_bits=2,
                expected_qsf=qsf,expected_sdc=SDC,expected_top=top,expected_audit=AUDIT)
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('Prepared exact native8/12 matched enabled frontends',flush=True)


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text());assert sha(Path(__file__))==record['builder_sha256']
    for name,digest in record['compiled_files'].items():assert sha(out/name)==digest,'prepared input changed: '+name
    for label in ('baseline','factored'):
        case=out/label;assert not(case/'map.log').exists(),'observe existing job; do not restart'
        for tool,args,stage in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(label,stage,flush=True)
            with(case/(stage+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Matched fit terminal; independent audit required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
