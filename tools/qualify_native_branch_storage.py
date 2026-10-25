#!/usr/bin/env python3
"""Qualify all104 native flag storage/history bits against four frozen cells.

Keep actual PRECAP=1 edge history, LOW-transparent f17, and e13 write-close
history. Seed arbitrary state only through the existing administration ports;
all138 raw mux/data/clock/state/advance inputs are free. Package clock/data
arrival and their producer wiring remain separate required qualifications.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from qualify_native_flag_file_window import CELL,reference
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
FILES=['verilog/rtl/dorado_flag_file_window_step.sv','verilog/rtl/dorado_native_branch_capture_step.sv']
TOP='''module native_branch_storage_contract(input wire clk,input wire [137:0] stimulus,
 output wire [103:0] actual,expected,output wire equivalent,g11_equal);
 reg [4:0] phase=0;reg [137:0] held;
 always @(posedge clk)begin if(phase<19)phase<=phase+1'b1;if(phase==0)held<=stimulus;end
 wire [103:0] before=held[137:34];wire advance=held[33];wire [3:0] clocks=held[32:29];
 wire [5:0] gd=held[22:17],hd=held[28:23];wire [3:0] fa=held[16:13],fb=held[12:9];wire fs=held[8];
 wire [3:0] address=held[7:4],fd=held[3:0];wire loading=phase<=17;
 wire [4:0] index=phase==17 ? 5'd16:phase-1'b1;
 wire [9:0] load_data=phase==17 ? before[89:80]:{6'b0,before[4*index+:4]};
 cell_F10145A_snapshot file_original(.state_host_clk(clk),.state_advance(phase==18 && advance),
  .state_freeze(loading),.state_guest_held(loading),.state_request(phase>0 && phase<=17),.state_write(1'b1),
  .state_index(index),.state_data(load_data),.physical_memory(actual[63:0]),.physical_window(actual[89:80]),.sys_clk(clk),
  .p10(address[3]),.p9(address[2]),.p7(address[1]),.p6(address[0]),
  .p12(fd[3]),.p11(fd[2]),.p4(fd[1]),.p5(fd[0]),.p3(1'b0),.p13(clocks[3]),.p8(1'b0));
 wire [17:0] fq,gq,hq;
 cell_MC10173_snapshot latch_original(.state_host_clk(clk),.state_advance(phase==18 && advance),
  .state_hold(loading),.state_write(phase==1),.state_data({14'b0,before[67:64]}),.state_result(fq),.sys_clk(clk),
  .p5(fa[0]),.p3(fa[1]),.p12(fa[2]),.p10(fa[3]),.p6(fb[0]),.p4(fb[1]),.p13(fb[2]),.p11(fb[3]),.p9(fs),.p7(clocks[2]));
 cell_MC10176_snapshot g_original(.state_host_clk(clk),.state_advance(phase==18 && advance),
  .state_hold(loading),.state_write(phase==1),.state_data({5'b0,before[96:90],before[73:68]}),.state_result(gq),.sys_clk(clk),
  .p12(gd[5]),.p11(gd[4]),.p10(gd[3]),.p7(gd[2]),.p6(gd[1]),.p5(gd[0]),.p9(clocks[0]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
 cell_MC10176_snapshot h_original(.state_host_clk(clk),.state_advance(phase==18 && advance),
  .state_hold(loading),.state_write(phase==1),.state_data({5'b0,before[103:97],before[79:74]}),.state_result(hq),.sys_clk(clk),
  .p12(hd[5]),.p11(hd[4]),.p10(hd[3]),.p7(hd[2]),.p6(hd[1]),.p5(hd[0]),.p9(clocks[1]),.p1(1'b0),.p8(1'b0),.p16(1'b0));
 assign actual[67:64]=fq[3:0];assign actual[73:68]=gq[5:0];assign actual[96:90]=gq[12:6];
 assign actual[79:74]=hq[5:0];assign actual[103:97]=hq[12:6];
 dorado_native_branch_capture_step candidate(before,advance,clocks,{fd,address,fs ? fb:fa,hd,gd},expected);
 assign equivalent=phase!=19 || actual==expected;
 assign g11_equal=phase!=19 || actual[73:68]==expected[73:68];
endmodule
'''


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    cell_names=[CELL]+[next(n for n in m['source_hashes']if Path(n).name=='cell_'+kind+'_snapshot.v')for kind in ['MC10173','MC10176']]
    proch=(stage/'src/fpga/mister/snapshot_native_processor/ProcH_full_snapshot.sv').read_text()
    assert 'cell_MC10176_snapshot u_g11 ('in proch and'cell_MC10176_snapshot u_h11 ('in proch
    cell_sources={}
    for n in cell_names:
        text=(stage/n).read_text();after=reference(text)if n==CELL else text
        (out/Path(n).name).write_text(after);cell_sources[n]=sha(stage/n)
    for n in FILES:shutil.copy2(ROOT/n,out/Path(n).name)
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    s=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),source_hashes=cell_sources,
        physical_flag_bits=80,existing_implementation_history_bits=24,compared_storage_bits=104,arbitrary_input_bits=138,
        assumptions=0,seeded_only_through_original_state_interfaces=True,PRECAP=1,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    save()
    names=[Path(n).name for n in cell_names]+[Path(n).name for n in FILES]
    for label,bad in [('complete-storage',False),('wrong-g11-data-epoch',True)]:
        d=out/label;d.mkdir()
        for n in names:shutil.copy2(out/n,d/n)
        if bad:
            p=d/Path(FILES[1]).name;text=p.read_text();token='state_after[73:68]=state_before[95:90];'
            assert text.count(token)==1;p.write_text(text.replace(token,'state_after[73:68]=data[5:0];',1))
        (d/'top.sv').write_text(TOP)
        script=f'''read_verilog -sv -defer -D DORADO_F10145_LOGIC {' '.join('/work/'+n for n in names)} /work/top.sv
hierarchy -check -top native_branch_storage_contract
proc
flatten
memory_map
opt
check -assert
sat -seq 21 -timeout 120 -prove {'g11_equal'if bad else'equivalent'} 1 -{'falsify'if bad else'verify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log,env=env);row=dict(label=label,negative=bad,pid=child.pid,command=command,
                started=time.time(),files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            s['actions'].append(row);save();code=child.wait()
        marker='model found: FAIL!'if bad else'no model found: SUCCESS!'
        row.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,row['passed'],flush=True)
        if not row['passed']:break
    passed=len(s['actions'])==2 and all(a['passed']for a in s['actions'])
    s.update(status='PASS_ACTUAL_NATIVE_BRANCH_STORAGE_TRANSFER'if passed else'TERMINAL_NATIVE_BRANCH_STORAGE_NOT_ESTABLISHED',finished=time.time(),
        native_owner_selected=False,clock_data_producer_arrivals_qualified=False,FPGA_timing_or_60ns_qualified=False)
    save();print(s['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
