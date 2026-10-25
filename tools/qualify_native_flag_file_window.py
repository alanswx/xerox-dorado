#!/usr/bin/env python3
"""Compare all74 file/history bits with the actual native F10145A cell.

Load all arbitrary RAM/window bits through the cell's original administration
interface, then perform one arbitrary enabled/held guest edge. All85 input bits
are free; no SAT input assumptions or direct register seeding are used. Wrong
closing address/data must change actual RAM, rather than only diagnostics.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]
CELL='src/fpga/mister/snapshot_native_conta/cell_F10145A_snapshot.sv'
RTL='verilog/rtl/dorado_flag_file_window_step.sv'
TOP='''module native_flag_window_contract(input wire clk,input wire [84:0] stimulus,
 output wire [73:0] actual,expected,output wire equivalent,ram_equal);
 reg [4:0] phase=0;reg [84:0] held;
 always @(posedge clk)begin
  if(phase<19)phase<=phase+1'b1;
  if(phase==0)held<=stimulus;
 end
 wire [63:0] memory_before=held[84:21];wire [9:0] window_before=held[20:11];
 wire advance=held[10];wire [3:0] address=held[9:6],data=held[5:2];
 wire chip_n=held[1],write_n=held[0];
 wire loading=phase<=17;
 wire [4:0] index=phase==17 ? 5'd16:phase-1'b1;
 wire [9:0] load_data=phase==17 ? window_before:{6'b0,memory_before[4*index+:4]};
 cell_F10145A_snapshot original(.state_host_clk(clk),.state_advance(phase==18 && advance),
  .state_freeze(loading),.state_guest_held(loading),.state_request(phase>0 && phase<=17),
  .state_write(1'b1),.state_index(index),.state_data(load_data),
  .physical_memory(actual[73:10]),.physical_window(actual[9:0]),.sys_clk(clk),
  .p10(address[3]),.p9(address[2]),.p7(address[1]),.p6(address[0]),
  .p12(data[3]),.p11(data[2]),.p4(data[1]),.p5(data[0]),.p3(chip_n),.p13(write_n),.p8(1'b0));
 dorado_flag_file_window_step candidate(memory_before,window_before,advance,address,data,chip_n,write_n,
  expected[73:10],expected[9:0]);
 assign equivalent=phase!=19 || actual==expected;
 assign ram_equal=phase!=19 || actual[73:10]==expected[73:10];
endmodule
'''


def reference(raw):
    token='    output wire [63:0] physical_memory,'
    assert raw.count(token)==1
    after=raw.replace(token,token+'\n    output wire [9:0] physical_window,',1)
    marker='endmodule';assert after.count(marker)==1
    after=after.replace(marker,' assign physical_window=state_window;\n'+marker,1)
    token="output reg state_done = 1'b0, state_error = 1'b0,"
    lowered="output reg state_done = 1'b0, output reg state_error = 1'b0,"
    assert after.count(token)==1
    after=after.replace(token,lowered,1)
    assert after.replace(lowered,token,1).replace('\n    output wire [9:0] physical_window,','',1).replace(' assign physical_window=state_window;\n','',1)==raw
    return after


def run(stage,out,yosys):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    raw=(stage/CELL).read_text()
    assert 'wire [9:0] state_window = {ce_lat,d_lat,a_lat,we_d};'in raw
    assert 'if (p13 && !we_d && ce_lat) mem[a_lat] <= d_lat;'in raw
    (out/'native-cell-before.sv').write_text(raw);(out/'native-cell-reference.sv').write_text(reference(raw))
    shutil.copy2(ROOT/RTL,out/Path(RTL).name);shutil.copy2(Path(__file__),out/Path(__file__).name)
    s=dict(status='RUNNING',scope=__doc__,started=time.time(),stage=str(stage),source_cell_sha256=sha(stage/CELL),
        stage_manifest_sha256=sha(stage/'r-front-control-native.json'),RTL_sha256=sha(ROOT/RTL),
        arbitrary_input_bits=85,compared_storage_bits=74,physical_RAM_bits=64,existing_window_history_bits=10,
        assumptions=0,seeded_only_through_original_state_interface=True,read_only_reference_export=True,
        files={p.name:sha(p)for p in out.iterdir()},actions=[])
    def save():(out/'run-state.json').write_text(json.dumps(s,indent=2)+'\n')
    save()
    for label in ['complete-storage','wrong-closing-address','wrong-closing-data']:
        d=out/label;d.mkdir();(d/'original.sv').write_text(reference(raw));candidate=(ROOT/RTL).read_text()
        if label=='wrong-closing-address':candidate=candidate.replace('4*window_before[4:1]','4*address',1)
        if label=='wrong-closing-data':candidate=candidate.replace('=window_before[8:5];','=data;',1)
        assert (candidate==(ROOT/RTL).read_text())==(label=='complete-storage')
        (d/'actual.sv').write_text(candidate);(d/'top.sv').write_text(TOP)
        positive=label=='complete-storage';signal='equivalent'if positive else'ram_equal'
        script=f'''read_verilog -sv -defer -D DORADO_F10145_LOGIC /work/original.sv /work/actual.sv /work/top.sv
hierarchy -check -top native_flag_window_contract
proc
flatten
memory_map
opt
check -assert
sat -seq 21 -timeout 120 -prove {signal} 1 -{'verify'if positive else'falsify'} -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env=os.environ.copy();env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');env['YOWASP_MOUNT']='/work='+str(d)
        command=[str(yosys),'-Q','-T','-s','/work/proof.ys']
        with(d/'proof.log').open('x')as log:
            child=subprocess.Popen(command,stdout=log,stderr=log,env=env)
            row=dict(label=label,positive=positive,signal=signal,steps=21,pid=child.pid,command=command,started=time.time(),
                files={p.name:sha(p)for p in d.iterdir()if p.name!='proof.log'})
            s['actions'].append(row);save();code=child.wait()
        marker='no model found: SUCCESS!'if positive else'model found: FAIL!'
        row.update(returncode=code,passed=code==0 and marker in(d/'proof.log').read_text(),finished=time.time(),log_sha256=sha(d/'proof.log'));save()
        print(label,row['passed'],flush=True)
        if not row['passed']:break
    passed=len(s['actions'])==3 and all(a['passed']for a in s['actions'])
    s.update(status='PASS_ACTUAL_NATIVE_FLAG_FILE_WINDOW_TRANSFER'if passed else'TERMINAL_NATIVE_FLAG_FILE_WINDOW_NOT_ESTABLISHED',
        finished=time.time(),native_owner_selected=False,physical_arrivals_or_FPGA_timing_qualified=False)
    save();print(s['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output','yosys']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.stage.resolve(),a.output.resolve(),a.yosys.resolve()))
