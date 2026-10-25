#!/usr/bin/env python3
"""Compare a common native flag owner with all four original storage cells.

All190 state/raw-mux/pin/guest/admin inputs are arbitrary, including independent
RAM response and frozen state. Seed only through the original interfaces, then
compare all175 physical-state/read/admin outputs after an arbitrary edge.
No source arrival, reduced cadence, Cedar boot or FPGA timing is implied.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_branch_storage import FILES as TRANSFER_FILES
from qualify_native_flag_file_window import CELL,reference
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
FILES=[*TRANSFER_FILES,'verilog/rtl/dorado_native_branch_owner.sv']
TOP='''module native_branch_owner_contract(input wire clk,input wire [189:0] stimulus,
 output wire [174:0] actual,expected,output wire equivalent,guest_q_equal);
 reg [4:0] phase=0;reg [189:0] held;
 always @(posedge clk)begin if(phase<21)phase<=phase+1'b1;if(phase==0)held<=stimulus;end
 wire [103:0] state_before;wire advance;wire [3:0] clocks;wire [5:0] gd,hd;
 wire [3:0] fa,fb,address,fd;wire fs,ff_hold;wire [2:0] ff_write;wire [17:0] ff_data;
 wire file_freeze,file_guest_held,file_request,file_write,keep_frozen;
 wire [4:0] file_index;wire [9:0] file_data,initial_file_result;
 assign {state_before,advance,clocks,gd,hd,fa,fb,fs,address,fd,ff_hold,ff_write,ff_data,
  file_freeze,file_guest_held,file_request,file_write,file_index,file_data,initial_file_result,keep_frozen}=held;
 wire guest_edge=phase==20 && advance;
 wire hold_ff=phase<20 ? 1'b1:ff_hold;
 wire [2:0] write_ff=phase==1 ? 3'b001:phase==2 ? 3'b010:phase==3 ? 3'b100:phase==20 ? ff_write:3'b0;
 wire [17:0] data_ff=phase==1 ? {14'b0,state_before[67:64]}:
  phase==2 ? {5'b0,state_before[96:90],state_before[73:68]}:
  phase==3 ? {5'b0,state_before[103:97],state_before[79:74]}:ff_data;
 wire freeze_file=phase<=18 ? 1'b1:phase==19 ? keep_frozen:file_freeze;
 wire held_file=phase<20 ? 1'b1:file_guest_held;
 wire request_file=(phase>0 && phase<=18) || (phase==20 && file_request);
 wire write_file=phase>0 && phase<=18 ? 1'b1:phase==20 ? file_write:1'b0;
 wire [4:0] index_file=phase==17 || phase==18 ? 5'd16:phase<17 ? phase-1'b1:file_index;
 // Read-before-write at18 returns the independently planted response while
 // restoring the arbitrary window. A held result is not assumed equal to it.
 wire [9:0] data_file=phase==17 ? initial_file_result:phase==18 ? state_before[89:80]:phase<17 ? {6'b0,state_before[4*index_file+:4]}:file_data;
 wire [103:0] original_state,owner_state;
 wire [3:0] original_read,owner_read;
 wire [17:0] original_f,original_g,original_h,owner_f,owner_g,owner_h;
 wire original_frozen,original_done,original_error,owner_frozen,owner_done,owner_error;
 wire [9:0] original_result,owner_result;
 cell_F10145A_snapshot file_original(.state_host_clk(clk),.state_advance(guest_edge),
  .state_freeze(freeze_file),.state_guest_held(held_file),.state_request(request_file),.state_write(write_file),
  .state_index(index_file),.state_data(data_file),.state_frozen(original_frozen),.state_done(original_done),
  .state_error(original_error),.state_result(original_result),
  .physical_memory(original_state[63:0]),.physical_window(original_state[89:80]),.sys_clk(clk),
  .p10(address[3]),.p9(address[2]),.p7(address[1]),.p6(address[0]),
  .p12(fd[3]),.p11(fd[2]),.p4(fd[1]),.p5(fd[0]),.p3(1'b0),.p13(clocks[3]),.p8(1'b0),
  .p14(original_read[3]),.p15(original_read[2]),.p1(original_read[1]),.p2(original_read[0]));
 cell_MC10173_snapshot latch_original(.state_host_clk(clk),.state_advance(guest_edge),
  .state_hold(hold_ff),.state_write(write_ff[0]),.state_data(data_ff),.state_result(original_f),.sys_clk(clk),
  .p5(fa[0]),.p3(fa[1]),.p12(fa[2]),.p10(fa[3]),.p6(fb[0]),.p4(fb[1]),.p13(fb[2]),.p11(fb[3]),.p9(fs),.p7(clocks[2]));
 cell_MC10176_snapshot g_original(.state_host_clk(clk),.state_advance(guest_edge),
  .state_hold(hold_ff),.state_write(write_ff[1]),.state_data(data_ff),.state_result(original_g),.sys_clk(clk),
  .p12(gd[5]),.p11(gd[4]),.p10(gd[3]),.p7(gd[2]),.p6(gd[1]),.p5(gd[0]),.p9(clocks[0]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
 cell_MC10176_snapshot h_original(.state_host_clk(clk),.state_advance(guest_edge),
  .state_hold(hold_ff),.state_write(write_ff[2]),.state_data(data_ff),.state_result(original_h),.sys_clk(clk),
  .p12(hd[5]),.p11(hd[4]),.p10(hd[3]),.p7(hd[2]),.p6(hd[1]),.p5(hd[0]),.p9(clocks[1]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
 assign original_state[67:64]=original_f[3:0];
 assign original_state[73:68]=original_g[5:0];assign original_state[96:90]=original_g[12:6];
 assign original_state[79:74]=original_h[5:0];assign original_state[103:97]=original_h[12:6];
 dorado_native_branch_owner owner(clk,guest_edge,clocks,{fd,address,fs ? fb:fa,hd,gd},
  hold_ff,write_ff,data_ff,freeze_file,held_file,request_file,write_file,index_file,data_file,
  owner_state,owner_read,owner_f,owner_g,owner_h,owner_frozen,owner_done,owner_error,owner_result);
 assign actual={owner_state,owner_read,owner_f,owner_g,owner_h,owner_frozen,owner_done,owner_error,owner_result};
 assign expected={original_state,original_read,original_f,original_g,original_h,original_frozen,original_done,original_error,original_result};
 assign equivalent=phase!=21 || actual==expected;
 assign guest_q_equal=phase!=21 || owner_state[79:0]==original_state[79:0];
endmodule
'''


def prepare(stage,out):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    cells=[CELL]+[next(n for n in m['source_hashes']if Path(n).name=='cell_'+k+'_snapshot.v')for k in ['MC10173','MC10176']]
    for n in cells:(out/Path(n).name).write_text(reference((stage/n).read_text())if n==CELL else(stage/n).read_text())
    for n in FILES:shutil.copy2(ROOT/n,out/Path(n).name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    return m,cells


def run(stage,out,yosys):
    m,cells=prepare(stage,out)
    r=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        original_cell_hashes={n:sha(stage/n)for n in cells},arbitrary_binary_inputs=190,compared_outputs=175,
        previous_file_response_independent_of_all104_storage_bits=True,
        physical_and_history_bits=104,assumptions=0,seeded_only_through_existing_interfaces=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(r,indent=2)+'\n')
    names=[Path(n).name for n in [*cells,*FILES]];save()
    for label in ['complete-owner','wrong-g11-epoch','wrong-admin-row']:
        d=out/label;d.mkdir()
        for n in names:shutil.copy2(out/n,d/n)
        if label=='wrong-g11-epoch':
            p=d/'dorado_native_branch_capture_step.sv';text=p.read_text();token='state_after[73:68]=state_before[95:90];'
            assert text.count(token)==1;p.write_text(text.replace(token,'state_after[73:68]=data[5:0];',1))
        if label=='wrong-admin-row':
            p=d/'dorado_native_branch_owner.sv';text=p.read_text();token='else q[4*file_index[3:0]+:4]<=file_data[3:0];'
            assert text.count(token)==1;p.write_text(text.replace(token,"else q[4*(file_index[3:0]^4'b1)+:4]<=file_data[3:0];",1))
        (d/'top.sv').write_text(TOP);positive=label=='complete-owner'
        script=f'''read_verilog -sv -defer -D DORADO_F10145_LOGIC {' '.join('/work/'+n for n in names)} /work/top.sv
hierarchy -check -top native_branch_owner_contract
proc
flatten
memory_map
opt
check -assert
sat -seq 23 -timeout 120 -prove {'equivalent'if positive else'guest_q_equal'} 1 -{'verify'if positive else'falsify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        cmd=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=log,env=env);a=dict(label=label,positive=positive,pid=p.pid,command=cmd,started=time.time(),
                files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'});r['actions'].append(a);save();code=p.wait()
        marker='no model found: SUCCESS!'if positive else'model found: FAIL!'
        a.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,a['passed'],flush=True)
        if not a['passed']:break
    passed=len(r['actions'])==3 and all(a['passed']for a in r['actions'])
    r.update(status='PASS_ACTUAL_NATIVE_BRANCH_COMMON_OWNER'if passed else'TERMINAL_NATIVE_BRANCH_OWNER_NOT_ESTABLISHED',finished=time.time(),
        native_selected=False,source_arrivals_or_reduced_cadence_or_FPGA_timing_qualified=False)
    save();print(r['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
